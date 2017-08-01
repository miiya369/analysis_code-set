//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup potential
 * @brief   Main part for calculate eigen energy of schrodinger equation
 * @author  Takaya Miyamoto
 * @since   Mon Jan 30 09:16:15 JST 2017
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>
#include <common/matrix_func.h>

#define PROJECT EIGEN_ENERGY   // <- Project name

static CHANNEL_TYPE channel;
static int          time_slice;
static SPIN_TYPE    spin;
static int          Neigen_energy;
static double       HAD1_mass;
static double       HAD2_mass;
static double       lat_a;
static double       stp_cnd;

static char conf_list[MAX_LEN_PATH];

static bool calc_from_fit = false;
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
   
   CONFIG<POTENTIAL> *potential = NULL;
   
   if (calc_from_fit)
   {
      
   }
   else
   {
      /// Input 2pt-correlators ///
      CONFIG<CORRELATOR> *Corr1 = new CONFIG<CORRELATOR>;
      CONFIG<CORRELATOR> *Corr2 = new CONFIG<CORRELATOR>;
      
      for (int conf=0; conf<analysis::Nconf; conf++) {
         (*Corr1)(conf).set(channel.hadron1, conf, 0, "PS");
         (*Corr2)(conf).set(channel.hadron2, conf, 0, "PS");
      }
      if (take_JK_flg) {
         Corr1 -> make_JK_sample();
         Corr2 -> make_JK_sample();
      }
      
      /// Input NBS wave functions & Construct R-correlators ///
      NBSwave::rot_matrix_init();
      
      NBS_WAVE_ORG         *Wave_org = NULL;
      CONFIG<NBS_WAVE>     *Wave     = new CONFIG<NBS_WAVE>;
      CONFIG<R_CORRELATOR> *Rcorr    = new CONFIG<R_CORRELATOR>[3];
      
      if (channel.flg_spin_proj) spin.set(channel.spin_name);
      else
         Wave_org = new NBS_WAVE_ORG;
      
      int count = 0;
      for (int it=time_slice-1; it<=time_slice+1; it++) {
         for (int conf=0; conf<analysis::Nconf; conf++) {
            if (channel.flg_spin_proj) (*Wave)(conf).set(channel, it, conf);
            else {
               Wave_org -> set(channel, it, conf);
               NBSwave::spin_projection(*Wave_org, (*Wave)(conf), spin);
            }
            NBSwave::LP_projection((*Wave)(conf));
         }
         if (take_JK_flg) Wave -> make_JK_sample();
         
         for (int conf=0; conf<analysis::Nconf; conf++)
            Rcorr[count](conf).set((*Wave)(conf),(*Corr1)(conf),(*Corr2)(conf),it);
         count++;
      }
      
      /// Construct the potentials ///
      CONFIG<R_CORRELATOR> *K_Rcorr   = new CONFIG<R_CORRELATOR>;
      potential = new CONFIG<POTENTIAL>;
      
      for (int conf=0; conf<analysis::Nconf; conf++) {
         potential::kernel(  (*K_Rcorr)(conf)
                           , Rcorr[0](conf), Rcorr[1](conf), Rcorr[2](conf)
                           , HAD1_mass, HAD2_mass );
         (*potential)(conf).set((*K_Rcorr)(conf), Rcorr[1](conf));
      }
   
      if (Wave_org != NULL) delete Wave_org;
      delete Wave;
      delete Corr1;
      delete Corr2;
      delete [] Rcorr;
      delete K_Rcorr;
   }
   
   /// Calculate the eigenvalues ///
   
   int     num_dim = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   double  mass    = (HAD1_mass*HAD2_mass)/(HAD1_mass+HAD2_mass) * hbar_c / lat_a;
   double *eigen   = new double[Neigen_energy*analysis::Nconf];
   
   int counter = 0;
   printf(" @ eigen energy calculating |   0%%");
   
#ifdef _OPENMP
#pragma omp parallel for
#endif
   for (int conf=0; conf<analysis::Nconf; conf++) {
      double *alpha = new double[num_dim];
      double *beta  = new double[num_dim];
      double *pot   = new double[num_dim];
      
      for (int i=0; i<num_dim; i++) pot[i] = (*potential)(conf)(i).real();
      
      to_tridiag_Lanczos_for_schrodinger(  alpha, beta, pot, analysis::xSIZE
                                         , mass, lat_a );
      
      eigenvalue_bi_section(  alpha, beta, &eigen[conf*Neigen_energy]
                            , num_dim, Neigen_energy, stp_cnd );
      
      delete [] alpha;
      delete [] beta;
      delete [] pot;
      
      counter++;
      printf("\b\b\b\b%3.0lf%%",double(counter)/double(analysis::Nconf)*100);
      fflush(stdout);
   } // conf
   
   printf("\n");
   delete potential;
   
   /// Output the eigenvalues ///
   
   double *err  = new double[Neigen_energy];
   double *mean = new double[Neigen_energy];
   double factor;
   if (use_JK_data)
      factor = double(analysis::Nconf-1);
   else
      factor = 1.0 / double(analysis::Nconf-1);
   
   printf("\n"
          " @ ====================================================== @\n");
   printf(" @    RESULTS OF EIGEN ENERGY FOR SCHRODINGER EQUATION    @\n");
   printf(" @ ====================================================== @\n"
          "\n");
   printf(" [dimension-less]\n");
   for (int i=0; i<Neigen_energy; i++) {
      mean[i] = 0.0;
      double sqr_mean = 0.0;
      for (int conf=0; conf<analysis::Nconf; conf++) {
         mean[i]  +=      eigen[i+conf*Neigen_energy];
         sqr_mean += pow( eigen[i+conf*Neigen_energy], 2 );
      }
      mean[i]  /= double(analysis::Nconf);
      sqr_mean /= double(analysis::Nconf);
      err[i]    = sqrt(factor * (sqr_mean-pow(mean[i],2)));
      printf("%9d %18.9lf +/- %12.9lf\n", i, mean[i], err[i]);
   }
   printf(" [MeV]\n");
   for (int i=0; i<Neigen_energy; i++) {
      mean[i] = 0.0;
      double sqr_mean = 0.0;
      for (int conf=0; conf<analysis::Nconf; conf++) {
         mean[i]  +=      eigen[i+conf*Neigen_energy] * hbar_c / lat_a;
         sqr_mean += pow( eigen[i+conf*Neigen_energy] * hbar_c / lat_a, 2 );
      }
      mean[i]  /= double(analysis::Nconf);
      sqr_mean /= double(analysis::Nconf);
      err[i]    = sqrt(factor * (sqr_mean-pow(mean[i],2)));
      printf("%9d %18.6lf +/- %12.6lf\n", i, mean[i], err[i]);
   }
   printf("\n @ ====================================================== @\n");
   
   delete [] err;
   delete [] mean;
   
   time(&end_time);
   printf("\n @ JOB END : ELAPSED TIME [s] = %d\n\n"
          ,(int)difftime(end_time,start_time) );
   
   return 0;
}
//========================================================================//

int set_args(int argc, char** argv)
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
         else if (strcmp(argv[loop],"-time"    )==0)
            time_slice = atoi(argv[loop+1]);
         else if (strcmp(argv[loop],"-conf_list" )==0)
            snprintf(conf_list,sizeof(conf_list),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-spin")==0)
            spin.set(argv[loop+1]);
         else if (strcmp(argv[loop],"-mass_had1")==0)
            HAD1_mass = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-mass_had2")==0)
            HAD2_mass = atof(argv[loop+1]);
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
   printf(" @ conf list  = %s\n",conf_list);
   printf(" @ #. conf    = %d\n",analysis::Nconf);
   printf(" @ time size  = %d\n",analysis::tSIZE);
   printf(" @ space size = %d\n",analysis::xSIZE);
   printf(" @ lat space  = %lf\n",lat_a);
   printf(" @ input dir  = %s\n",analysis::data_list[MAIN_PATH]);
   printf(" @ t shift    = %s\n",analysis::data_list[N_T_SHIFT]);
   printf(" @ x shift    = %s\n",analysis::data_list[N_X_SHIFT]);
   printf(" @ y shift    = %s\n",analysis::data_list[N_Y_SHIFT]);
   printf(" @ z shift    = %s\n",analysis::data_list[N_Z_SHIFT]);
   printf(" @ snk rela   = %s\n",analysis::data_list[SNK_RELA]);
   printf(" @ src rela   = %s\n",analysis::data_list[SRC_RELA]);
   printf(" @ channel    = %s\n",channel.name.c_str());
   printf(" @ time slice = %d\n",time_slice);
   printf(" @ HAD1 mass  = %lf\n",HAD1_mass);
   printf(" @ HAD2 mass  = %lf\n",HAD2_mass);
   printf(" @ spin       = %s\n",spin.name.c_str());
   printf(" @ #.eigen E  = %d\n",Neigen_energy);
   printf(" @ stop cnd   = %e\n",stp_cnd);
   printf(" @ use JK data= %s\n",analysis::bool_to_str(use_JK_data).c_str());
   printf(" @ take JK    = %s\n\n",analysis::bool_to_str(take_JK_flg).c_str());
   fflush(stdout);
   
   if (arguments_check) return 1;
   return 0;
}

int set_args_from_file(char* file_name)
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
      if (     strcmp(tmp_c1,"EIGEN_Size_of_space"     )==0){
         analysis::xSIZE = atoi(tmp_c2);
         analysis::ySIZE = atoi(tmp_c2);
         analysis::zSIZE = atoi(tmp_c2);
      }
      else if (strcmp(tmp_c1,"EIGEN_Size_of_time"      )==0)
         analysis::tSIZE = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"EIGEN_calc_time_slice"   )==0)
         time_slice = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"EIGEN_Spin_projection"   )==0)
         spin.set(tmp_c2);
      else if (strcmp(tmp_c1,"EIGEN_Gauge_confs_list"  )==0)
         snprintf(conf_list,sizeof(conf_list),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"EIGEN_Path_to_input_dir" )==0)
         analysis::set_data_list(MAIN_PATH, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"EIGEN_T_shift"           )==0)
         analysis::set_data_list(N_T_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"EIGEN_X_shift"           )==0)
         analysis::set_data_list(N_X_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"EIGEN_Y_shift"           )==0)
         analysis::set_data_list(N_Y_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"EIGEN_Z_shift"           )==0)
         analysis::set_data_list(N_Z_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"EIGEN_Snk_relativistic"  )==0)
         analysis::set_data_list(SNK_RELA, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"EIGEN_Src_relativistic"  )==0)
         analysis::set_data_list(SRC_RELA, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"EIGEN_Lattice_spacing"   )==0)
         lat_a = atof(tmp_c2);
      else if (strcmp(tmp_c1,"EIGEN_Number_of_eigen_energy" )==0)
         Neigen_energy = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"EIGEN_Stopping_condition")==0)
         stp_cnd = atof(tmp_c2);
      else if (strcmp(tmp_c1,"EIGEN_Had1_mass"         )==0)
         HAD1_mass = atof(tmp_c2);
      else if (strcmp(tmp_c1,"EIGEN_Had2_mass"         )==0)
         HAD2_mass = atof(tmp_c2);
      else if (strcmp(tmp_c1,"EIGEN_Take_jack_knife"    )==0)
         take_JK_flg = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"EIGEN_Use_jack_knife_data")==0)
         use_JK_data = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"EIGEN_Channel_name"      )==0)
         channel.set(tmp_c2);
      //***************************************************//
      else{
         printf("\n @@@@@@ invalid option \"%s\" in arguments file \"%s\"\n\n"
                , tmp_c1, file_name);
         return 1;
      }
   }
   return 0;
}
