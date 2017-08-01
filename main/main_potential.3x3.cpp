//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup potential
 * @brief   Main part for calculate 2x2 coupled channel potential (No Z factor)
 * @author  Takaya Miyamoto
 * @since   Mon Jan 30 08:58:23 JST 2017
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>
#include <common/complex_matrix.h>

#define PROJECT COUPLED_CHANNEL_POT_3x3   // <- Project name

#define CCP_Nch 3

#define idx(iii,jjj) ((jjj) + CCP_Nch * (iii))

static int          time_slice;
static SPIN_TYPE    spin;
static CHANNEL_TYPE channels[CCP_Nch*CCP_Nch];
static double       mass    [CCP_Nch*2];
static double       Zfactor [CCP_Nch*CCP_Nch];

static char conf_list[MAX_LEN_PATH];
static char outfile_path[MAX_LEN_PATH];

static bool calc_flg_fit = false;
static bool calc_flg_all = false;
static bool take_JK_flg  = false;
static bool use_JK_data  = false;

static bool arguments_check = false;
static int  set_args(int, char**);
static int  set_args_from_file(char*);

//========================================================================//
int main(int argc, char **argv)
{
   if (set_args(argc, argv) == 1) return 0;
   
   time_t start_time, start_time2, end_time;
   time(&start_time);
   
   for (int ich=0; ich<CCP_Nch*CCP_Nch; ich++)
      if (channels[ich].flg_spin_proj) {
         printf("ERROR: This code cannot calculate spin projected data.\n");
         return -1;
      }
   
   int itmp;
   
   /// Input 2pt-correlators ///
   
   string             *hadron_name = new string            [CCP_Nch * 2];
   CONFIG<CORRELATOR> *Corr        = new CONFIG<CORRELATOR>[CCP_Nch * 2];
   
   for (int ich=0; ich<CCP_Nch; ich++) {
      hadron_name[0 + 2*ich] = channels[CCP_Nch * ich].hadron1.name_only;
      hadron_name[1 + 2*ich] = channels[CCP_Nch * ich].hadron2.name_only;
   }
   
   HADRON_TYPE hadron;
   
   for (int ihad=0; ihad<CCP_Nch*2; ihad++) {
      itmp = 0;
      for (int jhad=0; jhad<ihad; jhad++)
         if (hadron_name[ihad] == hadron_name[jhad]) {
            itmp = 1;
            Corr[ihad] = Corr[jhad];
            break;
         }
      if (itmp == 0)
         for (int conf=0; conf<analysis::Nconf; conf++) {
            hadron.set(hadron_name[ihad]);
            Corr[ihad](conf).set(hadron, conf, 0, "PS");
         }
      if (take_JK_flg) Corr[ihad].make_JK_sample();
   }
   delete [] hadron_name;
   
   /// Input NBS wave functions ///
   
   NBSwave::rot_matrix_init();
   
   NBS_WAVE_ORG tmp_Wave;
   CONFIG<NBS_WAVE> *Wave0 = new CONFIG<NBS_WAVE>[CCP_Nch * CCP_Nch];
   CONFIG<NBS_WAVE> *Wave1 = new CONFIG<NBS_WAVE>[CCP_Nch * CCP_Nch];
   CONFIG<NBS_WAVE> *Wave2 = new CONFIG<NBS_WAVE>[CCP_Nch * CCP_Nch];
   
   for (int ich=0; ich<CCP_Nch*CCP_Nch; ich++) {
      for (int conf=0; conf<analysis::Nconf; conf++) {
         tmp_Wave.set(channels[ich], time_slice - 1, conf);
         NBSwave::spin_projection(tmp_Wave, Wave0[ich](conf), spin);
         NBSwave::LP_projection(Wave0[ich](conf));
         
         tmp_Wave.set(channels[ich], time_slice    , conf);
         NBSwave::spin_projection(tmp_Wave, Wave1[ich](conf), spin);
         NBSwave::LP_projection(Wave1[ich](conf));
         
         tmp_Wave.set(channels[ich], time_slice + 1, conf);
         NBSwave::spin_projection(tmp_Wave, Wave2[ich](conf), spin);
         NBSwave::LP_projection(Wave2[ich](conf));
      }
      if (take_JK_flg) {
         Wave0[ich].make_JK_sample();
         Wave1[ich].make_JK_sample();
         Wave2[ich].make_JK_sample();
      }
   }
   
   /// Construct R-correlators ///
   
   CONFIG<R_CORRELATOR> *Rcorr0 = new CONFIG<R_CORRELATOR>[CCP_Nch * CCP_Nch];
   CONFIG<R_CORRELATOR> *Rcorr1 = new CONFIG<R_CORRELATOR>[CCP_Nch * CCP_Nch];
   CONFIG<R_CORRELATOR> *Rcorr2 = new CONFIG<R_CORRELATOR>[CCP_Nch * CCP_Nch];
   
   for (int i=0; i<CCP_Nch; i++) for (int j=0; j<CCP_Nch; j++)
      for (int conf=0; conf<analysis::Nconf; conf++) {
         Rcorr0[idx(i,j)](conf).set(  Wave0[idx(i,j)](conf)
                               , Corr[0 + 2*i](conf), Corr[1 + 2*i](conf)
                               , time_slice - 1 );
         
         Rcorr1[idx(i,j)](conf).set(  Wave1[idx(i,j)](conf)
                               , Corr[0 + 2*i](conf), Corr[1 + 2*i](conf)
                               , time_slice     );
         
         Rcorr2[idx(i,j)](conf).set(  Wave2[idx(i,j)](conf)
                               , Corr[0 + 2*i](conf), Corr[1 + 2*i](conf)
                               , time_slice + 1 );
      }
   delete [] Wave0;
   delete [] Wave1;
   delete [] Wave2;

   /// Construct kernels ///
   
   CONFIG<R_CORRELATOR> *K_Rcorr = new CONFIG<R_CORRELATOR>[CCP_Nch * CCP_Nch];
   string pot_type;
   
   for (int ich=0; ich<CCP_Nch; ich++) for (int jch=0; jch<CCP_Nch; jch++)
      for (int conf=0; conf<analysis::Nconf; conf++) {
         K_Rcorr[idx(ich,jch)](conf).mem_alloc();
         pot_type = potential::kernel( K_Rcorr[idx(ich,jch)](conf)
                                      , Rcorr0[idx(ich,jch)](conf)
                                      , Rcorr1[idx(ich,jch)](conf)
                                      , Rcorr2[idx(ich,jch)](conf)
                                      , mass[2*ich+0], mass[2*ich+1] );
      }
   delete [] Rcorr0;
   delete [] Rcorr2;
   
   /// Calculate coupled channel potentials ///
   
   time(&start_time2);
   
   CONFIG<POTENTIAL> *ccp_pot = new CONFIG<POTENTIAL>[CCP_Nch * CCP_Nch];
   CONFIG<cdouble  > *Delta   = new CONFIG<cdouble  >[CCP_Nch * CCP_Nch];
   
   for (int i=0; i<CCP_Nch; i++) for (int j=0; j<CCP_Nch; j++)
      for (int conf=0; conf<analysis::Nconf; conf++) {
         ccp_pot[idx(i,j)](conf).mem_alloc();
         Delta  [idx(i,j)](conf)
         = ((  (Corr[0+2*j](conf)(time_slice) * (Corr[1+2*j](conf)(time_slice)))
             / (Corr[0+2*i](conf)(time_slice) * (Corr[1+2*i](conf)(time_slice))))
            ) * Zfactor[idx(i,j)];
   }
   
   int nsize = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   
#ifdef _OPENMP
#pragma omp parallel for
#endif
   for (int n=0; n<nsize; n++) {
      cmatrix Rc (CCP_Nch);
      cmatrix KRc(CCP_Nch);
      cmatrix Pot(CCP_Nch);
      
      for (int conf=0; conf<analysis::Nconf; conf++) {
         for (int ich=0; ich<CCP_Nch*CCP_Nch; ich++) {
            Rc (ich) = Rcorr1 [ich](conf)(n);
            KRc(ich) = K_Rcorr[ich](conf)(n);
         }
         Pot = KRc * Rc.inverce();
         
         for (int ich=0; ich<CCP_Nch*CCP_Nch; ich++)
            ccp_pot[ich](conf)(n) = Pot(ich) / Delta[ich](conf);
      }
   } // n
   
   delete [] Corr;
   delete [] Rcorr1;
   delete [] K_Rcorr;
   
   time(&end_time);
   printf(" @ Coupled channel potential has been calculated :"
          " ELAPSED TIME [s] = %d\n", (int)difftime(end_time, start_time2));
   
   /// Output coupled channel potentials ///
   
   char outfile_name[1024];
   for (int ich=0; ich<CCP_Nch*CCP_Nch; ich++) {
      snprintf(  outfile_name, sizeof(outfile_name)
               , "%s/%s_%s_%s_ccp_err_t%d"
               , outfile_path, channels[ich].name.c_str()
               , pot_type.c_str(), spin.name.c_str(), time_slice );
      
      analysis::output_data_err(ccp_pot[ich], outfile_name, use_JK_data);
   }
   if (calc_flg_fit)
      for (int ich=0; ich<CCP_Nch*CCP_Nch; ich++) {
         snprintf(  outfile_name, sizeof(outfile_name)
                  , "%s/%s_%s_%s_ccp_fit_t%d"
                  , outfile_path, channels[ich].name.c_str()
                  , pot_type.c_str(), spin.name.c_str(), time_slice );
         
         analysis::output_data_fit(ccp_pot[ich], outfile_name, use_JK_data);
      }
   if (calc_flg_all)
      for (int ich=0; ich<CCP_Nch*CCP_Nch; ich++) {
         snprintf(  outfile_name, sizeof(outfile_name)
                  , "%s/%s_%s_%s_ccp_all_t%d"
                  , outfile_path, channels[ich].name.c_str()
                  , pot_type.c_str(), spin.name.c_str(), time_slice );
         
         analysis::output_data_all(ccp_pot[ich], outfile_name);
      }
   delete [] ccp_pot;
   
   time(&end_time);
   printf( "\n @ JOB END : ELAPSED TIME [s] = %d\n\n"
          , (int)difftime(end_time, start_time) );
   
   return 0;
}
//========================================================================//

static int set_args(int argc, char** argv)
{
   for (int i=0; i<CCP_Nch*CCP_Nch; i++)
      Zfactor[i] = 1.0;
   
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
         else if (strcmp(argv[loop],"-conf_list")==0)
            snprintf(conf_list,sizeof(conf_list),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-idir"     )==0)
            analysis::set_data_list(MAIN_PATH, "%s", argv[loop+1]);
         else if (strcmp(argv[loop],"-odir"     )==0)
            snprintf(outfile_path,sizeof(outfile_path),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-time"     )==0)
            time_slice = atoi(argv[loop+1]);
         else if (strcmp(argv[loop],"-spin"     )==0)
            spin.set(argv[loop+1]);
         else if (strcmp(argv[loop],"-channel00")==0)
            channels[0].set(argv[loop+1]);
         else if (strcmp(argv[loop],"-channel01")==0)
            channels[1].set(argv[loop+1]);
         else if (strcmp(argv[loop],"-channel02")==0)
            channels[2].set(argv[loop+1]);
         else if (strcmp(argv[loop],"-channel10")==0)
            channels[3].set(argv[loop+1]);
         else if (strcmp(argv[loop],"-channel11")==0)
            channels[4].set(argv[loop+1]);
         else if (strcmp(argv[loop],"-channel12")==0)
            channels[5].set(argv[loop+1]);
         else if (strcmp(argv[loop],"-channel20")==0)
            channels[6].set(argv[loop+1]);
         else if (strcmp(argv[loop],"-channel21")==0)
            channels[7].set(argv[loop+1]);
         else if (strcmp(argv[loop],"-channel22")==0)
            channels[8].set(argv[loop+1]);
         else if (strcmp(argv[loop],"-mass0_0"   )==0)
            mass[0] = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-mass0_1"   )==0)
            mass[1] = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-mass1_0"   )==0)
            mass[2] = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-mass1_1"   )==0)
            mass[3] = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-mass2_0"   )==0)
            mass[4] = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-mass2_1"   )==0)
            mass[5] = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-Zfactor01")==0)
            Zfactor[idx(0,1)] = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-Zfactor02")==0)
            Zfactor[idx(0,2)] = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-Zfactor12")==0)
            Zfactor[idx(1,2)] = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-src_t"    )==0)
            analysis::set_data_list(N_T_SHIFT, "%s", argv[loop+1]);
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
   
   for (int i=1; i<CCP_Nch; i++) for (int j=0; j<i; j++)
      Zfactor[idx(i,j)] = 1.0 / Zfactor[idx(j,i)];
   
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
   for(int i=0; i<CCP_Nch; i++) for(int j=0; j<CCP_Nch; j++)
      printf(" @ channel %d%d  = %s\n",i,j,channels[idx(i,j)].name.c_str());
   printf(" @ output dir  = %s\n",outfile_path);
   printf(" @ time        = %d\n",time_slice);
   for(int i=0; i<CCP_Nch; i++) {
      printf(" @ mass %d_0    = %lf\n",i,mass[2*i+0]);
      printf(" @ mass %d_1    = %lf\n",i,mass[2*i+1]);
   }
   printf(" @ Zfactor 01  = %lf\n",Zfactor[idx(0,1)]);
   printf(" @ Zfactor 02  = %lf\n",Zfactor[idx(0,2)]);
   printf(" @ Zfactor 12  = %lf\n",Zfactor[idx(1,2)]);
   printf(" @ spin        = %s\n",spin.name.c_str());
   printf(" @ use JK data = %s\n",analysis::bool_to_str(use_JK_data).c_str());
   printf(" @ take JK     = %s\n\n",analysis::bool_to_str(take_JK_flg).c_str());
   printf(" @ out fit     = %s\n",analysis::bool_to_str(calc_flg_fit).c_str());
   printf(" @ out all     = %s\n\n",analysis::bool_to_str(calc_flg_all).c_str());
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
      if (     strcmp(tmp_c1,"CCP3_Size_of_space"      )==0){
         analysis::xSIZE = atoi(tmp_c2);
         analysis::ySIZE = atoi(tmp_c2);
         analysis::zSIZE = atoi(tmp_c2);
      }
      else if (strcmp(tmp_c1,"CCP3_Size_of_time"       )==0)
         analysis::tSIZE = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_calc_time_slice"    )==0)
         time_slice = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_Spin_projection"    )==0)
         spin.set(tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_Gauge_confs_list"   )==0)
         snprintf(conf_list,sizeof(conf_list),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_Path_to_output_dir" )==0)
         snprintf(outfile_path,sizeof(outfile_path),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_Path_to_input_dir"  )==0)
         analysis::set_data_list(MAIN_PATH, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_T_shift"            )==0)
         analysis::set_data_list(N_T_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_X_shift"            )==0)
         analysis::set_data_list(N_X_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_Y_shift"            )==0)
         analysis::set_data_list(N_Y_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_Z_shift"            )==0)
         analysis::set_data_list(N_Z_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_Snk_relativistic"   )==0)
         analysis::set_data_list(SNK_RELA, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_Src_relativistic"   )==0)
         analysis::set_data_list(SRC_RELA, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_Take_jack_knife"    )==0)
         take_JK_flg = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_Use_jack_knife_data")==0)
         use_JK_data = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_channel_00"         )==0)
         channels[0].set(tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_channel_01"         )==0)
         channels[1].set(tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_channel_02"         )==0)
         channels[2].set(tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_channel_10"         )==0)
         channels[3].set(tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_channel_11"         )==0)
         channels[4].set(tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_channel_12"         )==0)
         channels[5].set(tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_channel_20"         )==0)
         channels[6].set(tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_channel_21"         )==0)
         channels[7].set(tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_channel_22"         )==0)
         channels[8].set(tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_mass_ch0-0"         )==0)
         mass[0] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_mass_ch0-1"         )==0)
         mass[1] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_mass_ch1-0"         )==0)
         mass[2] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_mass_ch1-1"         )==0)
         mass[3] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_mass_ch2-0"         )==0)
         mass[4] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_mass_ch2-1"         )==0)
         mass[5] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_Z_factor_01"        )==0)
         Zfactor[idx(0,1)] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_Z_factor_02"        )==0)
         Zfactor[idx(0,2)] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_Z_factor_12"        )==0)
         Zfactor[idx(1,2)] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_Out_fit_data"       )==0)
         calc_flg_fit = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"CCP3_Out_all_data"       )==0)
         calc_flg_all = analysis::str_to_bool(tmp_c2);
      //***************************************************//
      else{
         printf("\n @@@@@@ invalid option \"%s\" in arguments file \"%s\"\n\n"
                , tmp_c1, file_name);
         return 1;
      }
   }
   return 0;
}

#undef idx
