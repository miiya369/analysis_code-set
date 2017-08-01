//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup potential
 * @brief   Main part for tensor potential calculation
 * @author  Takaya Miyamoto
 * @since   Mon Jan 30 08:58:23 JST 2017
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>

#define PROJECT CALC_TENSOR_FORCE // <- Project name

static CHANNEL_TYPE channel;
static int          time_slice;
static double       HAD1_mass;
static double       HAD2_mass;

static char conf_list[MAX_LEN_PATH];
static char outfile_path[MAX_LEN_PATH];

static bool out_all_datas = false;

static bool take_JK_flg   = false;
static bool use_JK_data   = false;

static bool arguments_check = false;
static int  set_args(int, char**);
static int  set_args_from_file(char*);

//========================================================================//
int main(int argc, char **argv)
{
   if (set_args(argc, argv) == 1) return 0;
   
   time_t start_time, end_time;
   time(&start_time);
   
   if (channel.flg_spin_proj) {
     printf("ERROR: This code cannot calculate spin projected data.\n");
     return -1;
   }
   
   /// Input 2pt-correlators ///
   
   CONFIG<CORRELATOR> Corr1, Corr2;
   
   for (int conf=0; conf<analysis::Nconf; conf++) {
      Corr1(conf).set(channel.hadron1, conf, 0, "PS");
      Corr2(conf).set(channel.hadron2, conf, 0, "PS");
   }
   if (take_JK_flg) {
      Corr1.make_JK_sample();
      Corr2.make_JK_sample();
   }
   
   /// Input NBS wave functions & Construct R-correlators ///
   
   NBSwave::rot_matrix_init();
   
   SPIN_TYPE spin("S1_Sz0");
   
   NBS_WAVE_ORG             Wave_org;
   NBS_WAVE_SRC_PRJ         S12Wave_sprj;
   CONFIG<NBS_WAVE_SRC_PRJ> Wave_sprj;
   
   CONFIG<R_CORRELATOR_SRC_PRJ> Rcorr_sprj[3], S12_Rcorr_sprj;
   
   int count = 0;
   for (int it=time_slice-1; it<=time_slice+1; it++) {
      for (int conf=0; conf<analysis::Nconf; conf++) {
         Wave_org.set(channel, it, conf);
         NBSwave::spin_projection(Wave_org, Wave_sprj(conf), spin);
      }
      if (take_JK_flg) Wave_sprj.make_JK_sample();

      for (int conf=0; conf<analysis::Nconf; conf++) {
         Rcorr_sprj[count](conf).set(Wave_sprj(conf),Corr1(conf),Corr2(conf), it);
         
         if (it==time_slice) {
            NBSwave::S12_psi(Wave_sprj(conf), S12Wave_sprj);
            S12_Rcorr_sprj(conf).set(S12Wave_sprj, Corr1(conf), Corr2(conf), it);
         }
      }
      count++;
   }
   
   /// Calculate the center & tensor potentials ///
   
   R_CORRELATOR_SRC_PRJ K_Rcorr_sprj, tmp;
   R_CORRELATOR         Rcorr[2], S12_Rcorr[2], K_Rcorr[2];
   CONFIG<POTENTIAL>    V_C, V_T;
   
   string pot_type;
   char   outfile_name[1024];
   
   for (int conf=0; conf<analysis::Nconf; conf++) {
      pot_type = potential::kernel(  K_Rcorr_sprj,        Rcorr_sprj[0](conf)
                                   , Rcorr_sprj[1](conf), Rcorr_sprj[2](conf)
                                   , HAD1_mass, HAD2_mass );
      
      Rcorrelator::Swave_division (Rcorr_sprj[1](conf), tmp);
      Rcorrelator::spin_projection(Rcorr_sprj[1](conf), Rcorr[0], spin);
      Rcorrelator::mult_YDstar    (tmp, spin);
      Rcorrelator::spin_projection(tmp, Rcorr[1], SPIN_1_p1);
         
      Rcorrelator::Swave_division (S12_Rcorr_sprj(conf), tmp);
      Rcorrelator::spin_projection(S12_Rcorr_sprj(conf), S12_Rcorr[0], spin);
      Rcorrelator::mult_YDstar    (tmp, spin);
      Rcorrelator::spin_projection(tmp, S12_Rcorr[1], SPIN_1_p1);
      
      Rcorrelator::Swave_division (K_Rcorr_sprj, tmp);
      Rcorrelator::spin_projection(K_Rcorr_sprj, K_Rcorr[0], spin);
      Rcorrelator::mult_YDstar    (tmp, spin);
      Rcorrelator::spin_projection(tmp, K_Rcorr[1], SPIN_1_p1);
      
      potential::tensor_potential(  V_C(conf), V_T(conf)
                                  , Rcorr[0], S12_Rcorr[0], K_Rcorr[0]
                                  , Rcorr[1], S12_Rcorr[1], K_Rcorr[1] );
   }
   
   /// Output the center & tensor potentials ///
   
   snprintf(  outfile_name, sizeof(outfile_name)
            , "%s/%s_center_%s_%s_err_t%d"
            , outfile_path, channel.name.c_str()
            , pot_type.c_str(), spin.name.c_str(), time_slice );
   
   analysis::output_data_err(V_C, outfile_name, use_JK_data);
   
   snprintf(  outfile_name, sizeof(outfile_name)
            , "%s/%s_tensor_%s_%s_err_t%d"
            , outfile_path, channel.name.c_str()
            , pot_type.c_str(), spin.name.c_str(), time_slice );
   
   analysis::output_data_err(V_T, outfile_name, use_JK_data);
   
   if (out_all_datas) {
      snprintf(  outfile_name, sizeof(outfile_name)
               , "%s/%s_center_%s_%s_all_t%d"
               , outfile_path, channel.name.c_str()
               , pot_type.c_str(), spin.name.c_str(), time_slice );
      
      analysis::output_data_all(V_C, outfile_name);
      
      snprintf(  outfile_name, sizeof(outfile_name)
               , "%s/%s_tensor_%s_%s_all_t%d"
               , outfile_path, channel.name.c_str()
               , pot_type.c_str(), spin.name.c_str(), time_slice );
      
      analysis::output_data_all(V_T, outfile_name);
   }
   
   time(&end_time);
   printf("\n @ JOB END : ELAPSED TIME [s] = %d\n\n"
          ,(int)difftime(end_time,start_time) );
   
   return 0;
}
//========================================================================//

static int set_args(int argc, char** argv)
{
   if (argc == 1) {
      analysis::usage(PROJECT);
      return 1;
   }
   bool infile_flg = false;
   
   for (int loop=1; loop<argc; loop++)
      if (strcmp(argv[loop],"-f")==0) {
         if (set_args_from_file(argv[loop+1]) == 1) return 1;
         infile_flg = true;
      }
   if (!infile_flg) {
      printf("\n @@@@@@ no input arguments file\n");
      analysis::usage(PROJECT);
      return 1;
   }
   for (int loop=1; loop<argc; loop++) {
      if (argv[loop][0] == '-'){
         if (     strcmp(argv[loop],"-f"        )==0){}
         //****** You may set additional potion in here ******//
         else if (strcmp(argv[loop],"-idir"     )==0)
            analysis::set_data_list(MAIN_PATH, "%s", argv[loop+1]);
         else if (strcmp(argv[loop],"-conf_list" )==0)
            snprintf(conf_list,sizeof(conf_list),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-odir"     )==0)
            snprintf(outfile_path,sizeof(outfile_path),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-time"    )==0)
            time_slice  = atoi(argv[loop+1]);
         else if (strcmp(argv[loop],"-channel"  )==0)
            channel.set(argv[loop+1]);
         else if (strcmp(argv[loop],"-mass_had1")==0)
            HAD1_mass = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-mass_had2")==0)
            HAD2_mass = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-src_t" )==0)
            analysis::set_data_list(N_T_SHIFT, "%s", argv[loop+1]);
         else if (strcmp(argv[loop],"-take_JK" )==0)
            take_JK_flg = true;
         else if (strcmp(argv[loop],"-check"    )==0)
            arguments_check = true;
         //***************************************************//
         else {
            printf("\n @@@@@@ invalid option \"%s\"\n", argv[loop]);
            analysis::usage(PROJECT);
            return 1;
         }
      }
   }
   if (take_JK_flg) use_JK_data = true;
   
   analysis::Nconf = analysis::set_data_list(conf_list);
   
   printf("\n @ Arguments set :\n");
   printf(" @ conf list   = %s\n",conf_list);
   printf(" @ #. conf     = %d\n",analysis::Nconf);
   printf(" @ time size   = %d\n",analysis::tSIZE);
   printf(" @ space size  = %d\n",analysis::xSIZE);
   printf(" @ input dir   = %s\n",analysis::data_list[MAIN_PATH]);
   printf(" @ t shift     = %s\n",analysis::data_list[N_T_SHIFT]);
   printf(" @ x shift     = %s\n",analysis::data_list[N_X_SHIFT]);
   printf(" @ y shift     = %s\n",analysis::data_list[N_Y_SHIFT]);
   printf(" @ z shift     = %s\n",analysis::data_list[N_Z_SHIFT]);
   printf(" @ snk rela    = %s\n",analysis::data_list[SNK_RELA]);
   printf(" @ src rela    = %s\n",analysis::data_list[SRC_RELA]);
   printf(" @ channel     = %s\n",channel.name.c_str());
   printf(" @ output dir  = %s\n",outfile_path);
   printf(" @ time slice  = %d\n",time_slice);
   printf(" @ HAD1 mass   = %lf\n",HAD1_mass);
   printf(" @ HAD2 mass   = %lf\n",HAD2_mass);
   printf(" @ use JK data = %s\n",analysis::bool_to_str(use_JK_data).c_str());
   printf(" @ take JK     = %s\n",analysis::bool_to_str(take_JK_flg).c_str());
   printf(" @ out all     = %s\n\n",analysis::bool_to_str(out_all_datas).c_str());
   fflush(stdout);
   
   if (arguments_check) return 1;
   return 0;
}

static int set_args_from_file(char* file_name)
{   
   ifstream ifs(file_name, ios::in);
   if (!ifs) {
      printf("\n @@@@@@ the file \"%s\" is not exist\n\n", file_name);
      return 1;
   }
   char tmp_str[MAX_N_DATA][MAX_LEN_PATH];
   char tmp_c1[32];
   char tmp_c2[MAX_LEN_PATH];
   int  count = 0;
   while (ifs.getline(tmp_str[count],sizeof(tmp_str[count]))) count++;
   
   for (int loop=0; loop<count; loop++) {
      if (sscanf(tmp_str[loop]," %s = %s ",tmp_c1,tmp_c2) != 2)
         continue;
      //****** You may set additional potion in here ******//
      if (     strcmp(tmp_c1,"V_T_Size_of_space"     )==0){
         analysis::xSIZE = atoi(tmp_c2);
         analysis::ySIZE = atoi(tmp_c2);
         analysis::zSIZE = atoi(tmp_c2);
      }
      else if (strcmp(tmp_c1,"V_T_calc_time_slice"    )==0)
         time_slice = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"V_T_Channel_name"      )==0)
         channel.set(tmp_c2);
      else if (strcmp(tmp_c1,"V_T_Gauge_confs_list"  )==0)
         snprintf(conf_list,sizeof(conf_list),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"V_T_Path_to_output_dir")==0)
         snprintf(outfile_path,sizeof(outfile_path),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"V_T_Path_to_input_dir" )==0)
         analysis::set_data_list(MAIN_PATH, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"V_T_T_shift"           )==0)
         analysis::set_data_list(N_T_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"V_T_X_shift"           )==0)
         analysis::set_data_list(N_X_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"V_T_Y_shift"           )==0)
         analysis::set_data_list(N_Y_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"V_T_Z_shift"           )==0)
         analysis::set_data_list(N_Z_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"V_T_Size_of_time"      )==0)
         analysis::tSIZE = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"V_T_Snk_relativistic"  )==0)
         analysis::set_data_list(SNK_RELA, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"V_T_Src_relativistic"  )==0)
         analysis::set_data_list(SRC_RELA, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"V_T_Take_jack_knife"    )==0)
         take_JK_flg = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"V_T_Use_jack_knife_data")==0)
         use_JK_data = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"V_T_Out_all_datas" )==0)
         out_all_datas = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"V_T_Had1_mass"         )==0)
         HAD1_mass = atof(tmp_c2);
      else if (strcmp(tmp_c1,"V_T_Had2_mass"         )==0)
         HAD2_mass = atof(tmp_c2);
      //***************************************************//
      else{
         printf("\n @@@@@@ invalid option \"%s\" in arguments file \"%s\"\n\n"
                , tmp_c1, file_name);
         return 1;
      }
   }
   return 0;
}
