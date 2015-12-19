//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup potential
 * @brief   Main part for potential calculation
 * @author  Takaya Miyamoto
 * @since   Mon Sep  7 16:57:28 JST 2015
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>

#define PROJECT CALC_POTENTIAL // <- Project name

int  analysis::Nconf;
int  analysis::xSIZE;
int  analysis::ySIZE;
int  analysis::zSIZE;
int  analysis::tSIZE;

CHANNEL_TYPE channel;
int time_min;
int time_max;
int spin;
int spin_z;
int ang_mom;
double HAD1_mass;
double HAD2_mass;

char conf_list[MAX_LEN_PATH];
char outfile_path[MAX_LEN_PATH];

bool endian_flg, dev_corr_flg, read_cmp_flg;
bool calc_flg_fit, calc_flg_pot, calc_flg_lap, calc_flg_t1, calc_flg_t2;

bool arguments_check = false;
int  set_args(int, char**);
int  set_args_from_file(char*);

//========================================================================//
int main(int argc, char **argv) {
   if (set_args(argc, argv) == 1) return 0;
   
   time_t start_time, end_time;
   time( &start_time );
   
   NBSwave::rot_matrix_init();
   
   NBS_WAVE_ORG         *Wave_org  = new NBS_WAVE_ORG;
   CONFIG<NBS_WAVE>     *Wave      = new CONFIG<NBS_WAVE>;
   CONFIG<CORRELATOR>   *Corr1     = new CONFIG<CORRELATOR>;
   CONFIG<CORRELATOR>   *Corr2     = new CONFIG<CORRELATOR>;
   CONFIG<R_CORRELATOR> *Rcorr     = new CONFIG<R_CORRELATOR>[3];
   
   CONFIG<R_CORRELATOR> *K_Rcorr   = new CONFIG<R_CORRELATOR>;
   CONFIG<POTENTIAL>    *potential = new CONFIG<POTENTIAL>;
   
   double reduced_mass = HAD1_mass*HAD2_mass / (HAD1_mass+HAD2_mass);
   string pot_type;
   char   outfile_name[1024];
   
   int count = 0;
   for (int it=time_min-1; it<=time_min; it++) {
      
      for (int conf=0; conf<analysis::Nconf; conf++) {
         Wave_org->set( channel, it, conf, endian_flg, read_cmp_flg );
         (*Wave)(conf).set( *Wave_org, spin, spin_z );
         (*Wave)(conf).projection( ang_mom );
         
         if (it==time_min-1) {
            (*Corr1)(conf).set(channel.hadron1, conf, "PS");
            (*Corr2)(conf).set(channel.hadron2, conf, "PS");
            (*K_Rcorr)(conf).mem_alloc();
         }
      } // conf
      
      Wave->make_JK_sample();
      
      if (it==time_min-1) {
         Corr1->make_JK_sample();
         Corr2->make_JK_sample();
      }
      for (int conf=0; conf<analysis::Nconf; conf++)
         Rcorr[count](conf).set((*Wave)(conf),(*Corr1)(conf),(*Corr2)(conf), it);
      count++;
      
   } // it
   
   for (int it=time_min; it<=time_max; it++) {
      
      for (int conf=0; conf<analysis::Nconf; conf++) {
         Wave_org->set( channel, it+1, conf, endian_flg, read_cmp_flg );
         (*Wave)(conf).set( *Wave_org, spin, spin_z );
         (*Wave)(conf).projection( ang_mom );
      } // conf
      
      Wave->make_JK_sample();
      
      for (int conf=0; conf<analysis::Nconf; conf++)
       Rcorr[count%3](conf).set((*Wave)(conf),(*Corr1)(conf),(*Corr2)(conf),it+1);
//-------------------- Selection of output potential type --------------------//
      if (calc_flg_lap) {
         for (int conf=0; conf<analysis::Nconf; conf++) {
            pot_type = potential::laplacian(  (*K_Rcorr)(conf)
                                            , Rcorr[(count+2)%3](conf)
                                            , reduced_mass );
            (*potential)(conf).set( (*K_Rcorr)(conf), Rcorr[(count+2)%3](conf) );
         }
         snprintf(  outfile_name, sizeof(outfile_name)
                  , "%s/%s_%s_S%dSz%d_err_t%d"
                  , outfile_path, channel.name.c_str()
                  , pot_type.c_str(), spin, spin_z, it );
         analysis::output_data_err( *potential, outfile_name );
      }
      if (calc_flg_t1) {
         for (int conf=0; conf<analysis::Nconf; conf++) {
            pot_type = potential::first_time_diff(  (*K_Rcorr)(conf)
                                                  , Rcorr[(count+1)%3](conf)
                                                  , Rcorr[(count+3)%3](conf) );
            (*potential)(conf).set( (*K_Rcorr)(conf), Rcorr[(count+2)%3](conf) );
         }
         snprintf(  outfile_name, sizeof(outfile_name)
                  , "%s/%s_%s_S%dSz%d_err_t%d"
                  , outfile_path, channel.name.c_str()
                  , pot_type.c_str(), spin, spin_z, it );
         analysis::output_data_err( *potential, outfile_name );
      }
      if (calc_flg_t2) {
         for (int conf=0; conf<analysis::Nconf; conf++) {
            pot_type = potential::second_time_diff(  (*K_Rcorr)(conf)
                                                   , Rcorr[(count+1)%3](conf)
                                                   , Rcorr[(count+2)%3](conf)
                                                   , Rcorr[(count+3)%3](conf)
                                                   , reduced_mass );
            (*potential)(conf).set( (*K_Rcorr)(conf), Rcorr[(count+2)%3](conf) );
         }
         snprintf(  outfile_name, sizeof(outfile_name)
                  , "%s/%s_%s_S%dSz%d_err_t%d"
                  , outfile_path, channel.name.c_str()
                  , pot_type.c_str(), spin, spin_z, it );
         analysis::output_data_err( *potential, outfile_name );
      }
      if (calc_flg_pot) {
         for (int conf=0; conf<analysis::Nconf; conf++) {
            pot_type = potential::kernel(  (*K_Rcorr)(conf)
                                         , Rcorr[(count+1)%3](conf)
                                         , Rcorr[(count+2)%3](conf)
                                         , Rcorr[(count+3)%3](conf)
                                         , reduced_mass );
            (*potential)(conf).set( (*K_Rcorr)(conf), Rcorr[(count+2)%3](conf) );
         }
         snprintf(  outfile_name, sizeof(outfile_name)
                  , "%s/%s_%s_S%dSz%d_err_t%d"
                  , outfile_path, channel.name.c_str()
                  , pot_type.c_str(), spin, spin_z, it );
         analysis::output_data_err( *potential, outfile_name );
      }
      if (calc_flg_fit) {
         for (int conf=0; conf<analysis::Nconf; conf++) {
            pot_type = potential::kernel(  (*K_Rcorr)(conf)
                                         , Rcorr[(count+1)%3](conf)
                                         , Rcorr[(count+2)%3](conf)
                                         , Rcorr[(count+3)%3](conf)
                                         , reduced_mass );
            (*potential)(conf).set( (*K_Rcorr)(conf), Rcorr[(count+2)%3](conf) );
         }
         snprintf(  outfile_name, sizeof(outfile_name)
                  , "%s/%s_%s_S%dSz%d_fit_t%d"
                  , outfile_path, channel.name.c_str()
                  , pot_type.c_str(), spin, spin_z, it );
         analysis::output_data_fit( *potential, outfile_name );
      }
//----------------------------------------------------------------------------//
      count++;
      
   } // it
   
   time( &end_time );
   printf("\n @ JOB END : ELAPSED TIME [s] = %d\n\n"
          ,(int)difftime(end_time,start_time) );
   return 0;
}
//========================================================================//

int set_args(int argc, char** argv) {
   
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
         else if (strcmp(argv[loop],"-t_max"    )==0)
            time_max  = atoi(argv[loop+1]);
         else if (strcmp(argv[loop],"-t_min"    )==0)
            time_min  = atoi(argv[loop+1]);
         else if (strcmp(argv[loop],"-channel"  )==0)
            channel.set(argv[loop+1]);
         else if (strcmp(argv[loop],"-mass_had1")==0)
            HAD1_mass = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-mass_had2")==0)
            HAD2_mass = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-comp"     )==0)
            read_cmp_flg = true;
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
   analysis::Nconf = analysis::set_data_list(conf_list);
   
   printf("\n @ Arguments set :\n");
   printf(" @ #. conf    = %d\n",analysis::Nconf);
   printf(" @ time size  = %d\n",analysis::tSIZE);
   printf(" @ space size = %d\n",analysis::xSIZE);
   printf(" @ t shift    = %s\n",analysis::data_list[N_T_SHIFT]);
   printf(" @ x shift    = %s\n",analysis::data_list[N_X_SHIFT]);
   printf(" @ y shift    = %s\n",analysis::data_list[N_Y_SHIFT]);
   printf(" @ z shift    = %s\n",analysis::data_list[N_Z_SHIFT]);
   printf(" @ snk rela   = %s\n",analysis::data_list[SNK_RELA]);
   printf(" @ src rela   = %s\n",analysis::data_list[SRC_RELA]);
   printf(" @ time_min   = %d\n",time_min);
   printf(" @ time_max   = %d\n",time_max);
   printf(" @ channel    = %s\n",channel.name.c_str());
   printf(" @ HAD1 mass  = %lf\n",HAD1_mass);
   printf(" @ HAD2 mass  = %lf\n",HAD2_mass);
   printf(" @ spin       = %d\n",spin);
   printf(" @ spin z cmp = %d\n",spin_z);
   printf(" @ ang mom    = %d\n",ang_mom);
   printf(" @ conf list  = %s\n",conf_list);
   printf(" @ infile     = %s\n",analysis::data_list[MAIN_PATH]);
   printf(" @ outfile    = %s\n",outfile_path);
   printf(" @ endian cnv = %s\n",analysis::bool_to_str(endian_flg).c_str());
   printf(" @ dev corr   = %s\n",analysis::bool_to_str(dev_corr_flg).c_str());
   printf(" @ read comp  = %s\n",analysis::bool_to_str(read_cmp_flg).c_str());
   printf(" @ out pot    = %s\n",analysis::bool_to_str(calc_flg_pot).c_str());
   printf(" @ out lap    = %s\n",analysis::bool_to_str(calc_flg_lap).c_str());
   printf(" @ out t1 dif = %s\n",analysis::bool_to_str(calc_flg_t1).c_str());
   printf(" @ out t2 dif = %s\n",analysis::bool_to_str(calc_flg_t2).c_str());
   printf(" @ out fit    = %s\n\n",analysis::bool_to_str(calc_flg_fit).c_str());
   fflush(stdout);
   
   if (arguments_check) return 1;
   return 0;
}

int set_args_from_file(char* file_name) {
   
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
      if (     strcmp(tmp_c1,"POT_Size_of_space"     )==0){
         analysis::xSIZE = atoi(tmp_c2);
         analysis::ySIZE = atoi(tmp_c2);
         analysis::zSIZE = atoi(tmp_c2);
      }
      else if (strcmp(tmp_c1,"POT_Min_time_slice"    )==0)
         time_min = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"POT_Max_time_slice"    )==0)
         time_max = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"POT_Channel_name"      )==0)
         channel.set(tmp_c2);
      else if (strcmp(tmp_c1,"POT_Spin_projection"   )==0)
         spin = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"POT_Spin_z_component"  )==0)
         spin_z = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"POT_Ang_mom_projection")==0)
         ang_mom = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"POT_Gauge_confs_list"  )==0)
         snprintf(conf_list,sizeof(conf_list),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"POT_Path_to_output_dir")==0)
         snprintf(outfile_path,sizeof(outfile_path),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"POT_Endian_convert"    )==0)
         endian_flg = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"POT_Out_sum_pot"       )==0)
         calc_flg_pot = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"POT_Out_laplacian"     )==0)
         calc_flg_lap = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"POT_Out_1st_t_diff"    )==0)
         calc_flg_t1 = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"POT_Out_2nd_t_diff"    )==0)
         calc_flg_t2 = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"POT_Out_fit_data"      )==0)
         calc_flg_fit = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"POT_Path_to_input_dir" )==0)
         snprintf(         analysis::data_list[MAIN_PATH]
                  , sizeof(analysis::data_list[MAIN_PATH])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"POT_T_shift"           )==0)
         snprintf(         analysis::data_list[N_T_SHIFT]
                  , sizeof(analysis::data_list[N_T_SHIFT])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"POT_X_shift"           )==0)
         snprintf(         analysis::data_list[N_X_SHIFT]
                  , sizeof(analysis::data_list[N_X_SHIFT])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"POT_Y_shift"           )==0)
         snprintf(         analysis::data_list[N_Y_SHIFT]
                  , sizeof(analysis::data_list[N_Y_SHIFT])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"POT_Z_shift"           )==0)
         snprintf(         analysis::data_list[N_Z_SHIFT]
                  , sizeof(analysis::data_list[N_Z_SHIFT])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"POT_Size_of_time"      )==0)
         analysis::tSIZE = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"POT_Snk_relativistic"  )==0)
         snprintf(         analysis::data_list[SNK_RELA]
                  , sizeof(analysis::data_list[SNK_RELA])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"POT_Src_relativistic"  )==0)
         snprintf(         analysis::data_list[SRC_RELA]
                  , sizeof(analysis::data_list[SRC_RELA])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"POT_Rcorr_NBS/corr"    )==0)
         dev_corr_flg = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"POT_read_compress_NBS" )==0)
         read_cmp_flg = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"POT_Had1_mass"         )==0)
         HAD1_mass = atof(tmp_c2);
      else if (strcmp(tmp_c1,"POT_Had2_mass"         )==0)
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
