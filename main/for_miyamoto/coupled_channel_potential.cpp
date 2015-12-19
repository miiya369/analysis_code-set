//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup potential
 * @brief   Main part for calculate coupled channel potential
 * @author  Takaya Miyamoto
 * @since   Sun Oct 18 03:45:59 JST 2015
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>

#define PROJECT COUPLED_CHANNEL_POT   // <- Project name

#define IT(iii,ttt) ((iii)+ 4* (ttt))
#define RC(Row,Colomn) ((Colomn)+ 2* (Row))

int    time_slice;
int    spin;
int    spin_z;
int    ang_mom;
double mass[4];

char conf_list[MAX_LEN_PATH];
char outfile_path[MAX_LEN_PATH];

bool endian_flg, read_cmp_flg, calc_flg_fit;

bool arguments_check = false;
int  set_args(int, char**);
int  set_args_from_file(char*);

//========================================================================//
int main(int argc, char **argv) {
   if (set_args(argc, argv) == 1) return 0;
   
   size_t N_size = analysis::xSIZE*analysis::ySIZE*analysis::zSIZE * 4*4;
   size_t n_size = analysis::xSIZE*analysis::ySIZE*analysis::zSIZE;
   
   time_t start_time, start_time2, end_time;
   time( &start_time );
   
   NBSwave::rot_matrix_init();
   
   string channel_name[9]
   = {  "Pro-Lam_Pro-Lam" , "Pro-Lam_Pro-SigZ" , "Pro-Lam_Neu-SigP"
      , "Pro-SigZ_Pro-Lam", "Pro-SigZ_Pro-SigZ", "Pro-SigZ_Neu-SigP"
      , "Neu-SigP_Pro-Lam", "Neu-SigP_Pro-SigZ", "Neu-SigP_Neu-SigP" };
   
   string channel_prj_name[4]
   = {  "Nuc-Lam12_Nuc-Lam12", "Nuc-Lam12_Nuc-Sig12"
      , "Nuc-Sig12_Nuc-Lam12", "Nuc-Sig12_Nuc-Sig12" };
   
   string hadron_name[3]
   = { "proton" , "Lambda" , "Sigma" };
   
   CONFIG<NBS_WAVE>     *Wave  = new CONFIG<NBS_WAVE>[4];
   CONFIG<CORRELATOR>   *Corr  = new CONFIG<CORRELATOR>[3];
   CONFIG<R_CORRELATOR> *Rcorr = new CONFIG<R_CORRELATOR>[4*3];
   
   NBS_WAVE_ORG *tmp_Wave     = new NBS_WAVE_ORG[9];
   NBS_WAVE_ORG *tmp_Wave_prj = new NBS_WAVE_ORG[4];
   for (int loop=0; loop<4; loop++) tmp_Wave_prj[loop].mem_alloc();
   
   CHANNEL_TYPE  channel;
   HADRON_TYPE   hadron;
   
   for (int it=-1; it<=1; it++) {
      for (int conf=0; conf<analysis::Nconf; conf++) {
      
         for (int loop=0; loop<9; loop++) {
            channel.set(channel_name[loop]);
            tmp_Wave[loop].set(  channel, time_slice+it, conf
                               , endian_flg, read_cmp_flg );
         }
         for (size_t n=0; n<N_size; n++) {   // isospin projection (I=1/2)
//================== I_z = 1/2, Lambda N - 1/2, Lambda N =================//
            tmp_Wave_prj[0](n) = tmp_Wave[0](n);
            
//================== I_z = 1/2, Lambda N - 1/2, Sigma  N =================//
            tmp_Wave_prj[1](n) =   sqrt(2.0/3.0)*tmp_Wave[2](n)
                                 - sqrt(1.0/3.0)*tmp_Wave[1](n);
            
//================== I_z = 1/2, Sigma  N - 1/2, Lambda N =================//
            tmp_Wave_prj[2](n) =   sqrt(2.0/3.0)*tmp_Wave[6](n)
                                 - sqrt(1.0/3.0)*tmp_Wave[3](n);
            
//================== I_z = 1/2, Sigma  N - 1/2, Sigma  N =================//
            tmp_Wave_prj[3](n) = sqrt(2.0/3.0)*(  sqrt(2.0/3.0)*tmp_Wave[8](n)
                                                - sqrt(1.0/3.0)*tmp_Wave[5](n) )
                               - sqrt(1.0/3.0)*(  sqrt(2.0/3.0)*tmp_Wave[7](n)
                                                - sqrt(1.0/3.0)*tmp_Wave[4](n) );
         } // n
         
         for (int loop=0; loop<4; loop++) {
            channel.set(channel_prj_name[loop]);
            Wave[loop](conf).set( tmp_Wave_prj[loop], spin, spin_z );
            Wave[loop](conf).projection(ang_mom);
         }
         if (it==-1) {
            for (int loop=0; loop<3; loop++) {
               hadron.set(hadron_name[loop]);
               Corr[loop](conf).set(hadron, conf, "PS");
            }
         }
      } // conf
      
      for (int loop=0; loop<4; loop++) Wave[loop].make_JK_sample();
      
      if (it==-1)
         for (int loop=0; loop<3; loop++) Corr[loop].make_JK_sample();
      
      for (int conf=0; conf<analysis::Nconf; conf++) {
         Rcorr[IT(0,it+1)](conf).set(  Wave[0](conf)
                                     , Corr[0](conf),Corr[1](conf),time_slice+it);
         Rcorr[IT(1,it+1)](conf).set(  Wave[1](conf)
                                     , Corr[0](conf),Corr[1](conf),time_slice+it);
         Rcorr[IT(2,it+1)](conf).set(  Wave[2](conf)
                                     , Corr[0](conf),Corr[2](conf),time_slice+it);
         Rcorr[IT(3,it+1)](conf).set(  Wave[3](conf)
                                     , Corr[0](conf),Corr[2](conf),time_slice+it);
      }
   } // it
   
   delete [] tmp_Wave;
   delete [] tmp_Wave_prj;
   delete [] Wave;
   
   CONFIG<R_CORRELATOR> *K_Rcorr = new CONFIG<R_CORRELATOR>[4];
   string pot_type;
   
   for (int conf=0; conf<analysis::Nconf; conf++)
      for (int loop=0; loop<4; loop++) {
         K_Rcorr[loop](conf).mem_alloc();
         
         pot_type = potential::kernel(  K_Rcorr[loop](conf)
                                      , Rcorr[IT(loop,0)](conf)
                                      , Rcorr[IT(loop,1)](conf)
                                      , Rcorr[IT(loop,2)](conf)
                                      , mass[loop] );
         
         Rcorr[IT(loop,0)](conf).mem_del();
         Rcorr[IT(loop,2)](conf).mem_del();
      }
   
   time( &start_time2 );
//   printf(" @ calculating coupled channel potental |   0%%");
   
   CONFIG<POTENTIAL> *coupled_ch_pot = new CONFIG<POTENTIAL>[4];
   cdouble           Rcorr_det;
   cdouble           Rcorr_inv[4];
   for (int conf=0; conf<analysis::Nconf; conf++) {
      
      for (int loop=0; loop<4; loop++)
         coupled_ch_pot[loop](conf).mem_alloc();
      
      for (size_t n=0; n<n_size; n++) {
         Rcorr_det = (  Rcorr[IT(RC(0,0),1)](conf)(n)
                      * Rcorr[IT(RC(1,1),1)](conf)(n)
                      - Rcorr[IT(RC(0,1),1)](conf)(n)
                      * Rcorr[IT(RC(1,0),1)](conf)(n) );
         
         Rcorr_inv[RC(0,0)] =  Rcorr[IT(RC(1,1),1)](conf)(n) / Rcorr_det;
         Rcorr_inv[RC(1,0)] = -Rcorr[IT(RC(1,0),1)](conf)(n) / Rcorr_det;
         Rcorr_inv[RC(0,1)] = -Rcorr[IT(RC(0,1),1)](conf)(n) / Rcorr_det;
         Rcorr_inv[RC(1,1)] =  Rcorr[IT(RC(0,0),1)](conf)(n) / Rcorr_det;
   
         for (int R=0; R<2; R++) for(int C=0; C<2; C++) for(int k=0; k<2; k++)
            coupled_ch_pot[RC(R,C)](conf)(n)
                += K_Rcorr[RC(R,k)](conf)(n) * Rcorr_inv[RC(k,C)];
         
         coupled_ch_pot[RC(0,1)](conf)(n) *= (  Corr[1](conf)(time_slice)
                                              / Corr[2](conf)(time_slice) );
         coupled_ch_pot[RC(1,0)](conf)(n) *= (  Corr[2](conf)(time_slice)
                                              / Corr[1](conf)(time_slice) );
      } // n
      
      for (int loop=0; loop<4; loop++) {
         Rcorr[IT(loop,1)](conf).mem_del();
         K_Rcorr[loop](conf).mem_del();
      }
//      printf("\b\b\b\b%3.0lf%%",double(conf+1)/double(analysis::Nconf)*100);
//      fflush(stdout);
      
   } // conf
   
   delete [] Corr;
   delete [] Rcorr;
   delete [] K_Rcorr;
   
//   printf("\n");
   time( &end_time );
   printf(" @ Coupled channel potential has been calculated :"
          " ELAPSED TIME [s] = %d\n", (int)difftime(end_time,start_time2) );
   
   char outfile_name[1024];
   for (int loop=0; loop<4; loop++) {
      
      snprintf(  outfile_name, sizeof(outfile_name)
               , "%s/%s_%s_S%dSz%d_ccp_err_t%d"
               , outfile_path, channel_prj_name[loop].c_str()
               , pot_type.c_str(), spin, spin_z, time_slice );
      
      analysis::output_data_err( coupled_ch_pot[loop], outfile_name );
   }
   if (calc_flg_fit)
      for (int loop=0; loop<4; loop++) {
         
         snprintf(  outfile_name, sizeof(outfile_name)
                  , "%s/%s_%s_S%dSz%d_ccp_fit_t%d"
                  , outfile_path, channel_prj_name[loop].c_str()
                  , pot_type.c_str(), spin, spin_z, time_slice );
         
         analysis::output_data_fit( coupled_ch_pot[loop], outfile_name );
      }
   delete [] coupled_ch_pot;
   
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
         else if (strcmp(argv[loop],"-time"    )==0)
            time_slice = atoi(argv[loop+1]);
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
   printf(" @ t size     = %d\n",analysis::tSIZE);
   printf(" @ cood size  = %d\n",analysis::xSIZE);
   printf(" @ t shift    = %s\n",analysis::data_list[N_T_SHIFT]);
   printf(" @ x shift    = %s\n",analysis::data_list[N_X_SHIFT]);
   printf(" @ y shift    = %s\n",analysis::data_list[N_Y_SHIFT]);
   printf(" @ z shift    = %s\n",analysis::data_list[N_Z_SHIFT]);
   printf(" @ snk rela   = %s\n",analysis::data_list[SNK_RELA]);
   printf(" @ src rela   = %s\n",analysis::data_list[SRC_RELA]);
   printf(" @ spin       = %d\n",spin);
   printf(" @ spin z cmp = %d\n",spin_z);
   printf(" @ ang mom    = %d\n",ang_mom);
   for(int i=0; i<4; i++) printf(" @ mass %d      = %lf\n",i,mass[i]);
   printf(" @ conf list  = %s\n",conf_list);
   printf(" @ infile     = %s\n",analysis::data_list[MAIN_PATH]);
   printf(" @ outfile    = %s\n @\n",outfile_path);
   printf(" @ out fit    = %s\n",analysis::bool_to_str(calc_flg_fit).c_str());
   printf(" @ endian cnv = %s\n",analysis::bool_to_str(endian_flg).c_str());
   printf(" @ read cmp   = %s\n\n",analysis::bool_to_str(read_cmp_flg).c_str());
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
      if (     strcmp(tmp_c1,"CCP_Size_of_space"     )==0){
         analysis::xSIZE = atoi(tmp_c2);
         analysis::ySIZE = atoi(tmp_c2);
         analysis::zSIZE = atoi(tmp_c2);
      }
      else if (strcmp(tmp_c1,"CCP_Size_of_time"      )==0)
         analysis::tSIZE = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"CCP_calc_time_slice"   )==0)
         time_slice = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Spin_projection"   )==0)
         spin = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Spin_z_component"  )==0)
         spin_z = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Ang_mom_projection")==0)
         ang_mom = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Gauge_confs_list"  )==0)
         snprintf(conf_list,sizeof(conf_list),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Path_to_output_dir")==0)
         snprintf(outfile_path,sizeof(outfile_path),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Endian_convert"    )==0)
         endian_flg = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Path_to_input_dir" )==0)
         snprintf(         analysis::data_list[MAIN_PATH]
                  , sizeof(analysis::data_list[MAIN_PATH])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"CCP_T_shift"           )==0) {
         int tmp_i = atoi(tmp_c2);
         snprintf(         analysis::data_list[N_T_SHIFT]
                  , sizeof(analysis::data_list[N_T_SHIFT])
                  , "%03d", tmp_i);
      }
      else if (strcmp(tmp_c1,"CCP_X_shift"           )==0) {
         int tmp_i = atoi(tmp_c2);
         snprintf(         analysis::data_list[N_X_SHIFT]
                  , sizeof(analysis::data_list[N_X_SHIFT])
                  , "%03d", tmp_i);
      }
      else if (strcmp(tmp_c1,"CCP_Y_shift"           )==0) {
         int tmp_i = atoi(tmp_c2);
         snprintf(         analysis::data_list[N_Y_SHIFT]
                  , sizeof(analysis::data_list[N_Y_SHIFT])
                  , "%03d", tmp_i);
      }
      else if (strcmp(tmp_c1,"CCP_Z_shift"           )==0) {
         int tmp_i = atoi(tmp_c2);
         snprintf(         analysis::data_list[N_Z_SHIFT]
                  , sizeof(analysis::data_list[N_Z_SHIFT])
                  , "%03d", tmp_i);
      }
      else if (strcmp(tmp_c1,"CCP_Snk_relativistic"  )==0)
         snprintf(         analysis::data_list[SNK_RELA]
                  , sizeof(analysis::data_list[SNK_RELA])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Src_relativistic"  )==0)
         snprintf(         analysis::data_list[SRC_RELA]
                  , sizeof(analysis::data_list[SRC_RELA])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Reduced_mass_00"   )==0)
         mass[0] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Reduced_mass_01"   )==0)
         mass[1] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Reduced_mass_10"   )==0)
         mass[2] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Reduced_mass_11"   )==0)
         mass[3] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"CCP_read_compress_NBS" )==0)
         read_cmp_flg = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Out_fit_data"      )==0)
         calc_flg_fit = analysis::str_to_bool(tmp_c2);
      //***************************************************//
      else{
         printf("\n @@@@@@ invalid option \"%s\" in arguments file \"%s\"\n\n"
                , tmp_c1, file_name);
         return 1;
      }
   }
   return 0;
}

#undef IT
#undef RC
