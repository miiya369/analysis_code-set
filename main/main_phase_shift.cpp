//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup observable
 * @brief   Main part for observable
 * @author  Takaya Miyamoto
 * @since   Sun Jul 17 06:07:42 JST 2016
 */
//--------------------------------------------------------------------------

#include <observable/phase_shift.h>

#define PROJECT CALC_PHASE_SHIFT   // <- Project name

static double lattice_space;
static double mass;
static double ang_mom;
static double E_min;
static double E_max;
static double E_div;
static double delta;
static double max_r;
static double V_0;
static double r_0;
static double energy_scatt_len;

static char infile_path[MAX_LEN_PATH];
static char outfile_path[MAX_LEN_PATH];

static bool test_flg    = false;
static bool use_JK_data = false;

static bool arguments_check = false;
static int  set_args(int, char**);
static int  set_args_from_file(char*);

//========================================================================//
int main(int argc, char **argv)
{
   if (set_args(argc, argv) == 1) return 0;
   
   time_t start_time, end_time;
   time(&start_time);
   
   if (test_flg) /// For test (1 Dim. Potential wall) ///
   {
      T_MATRIX test_T(E_min, E_max, E_div, 1);
      double params[2] = {r_0, V_0};
      lattice_space   = 1;
      int tmp_ftype   = 0;
      int tmp_Nparam  = 2;
      
      printf(" @ phase shift calculating ... ");
      test_T.calc_Tmat_dif(  params, &mass, &mass, &tmp_Nparam, &tmp_ftype
                           , lattice_space, ang_mom, delta, max_r );
      printf("DONE\n");
      
      CONFIG<PHASE_SHIFT> test(1);
      phase_shift::calc_phase_shift(test_T, &test(0));
      analysis::output_data_err(test, outfile_path, true);
   }
   else
   {
      /// Input the miyamoto-format parameter data ///
      int Nparam, func_type;
      observable::input_param(infile_path, analysis::Nconf, Nparam, func_type);
      
      double *param = new double[analysis::Nconf * Nparam];
      observable::input_param(infile_path, param);
      
      /// Calculate phase shifts by using differential equation ///
      CONFIG<PHASE_SHIFT> phase(analysis::Nconf);
      double *scattering_length = new double[analysis::Nconf];
      
      int counter = 0;
      printf(" @ phase shift calculating       |   0%%");
      
#ifdef _OPENMP
#pragma omp parallel for
#endif
      for (int iconf=0; iconf<analysis::Nconf; iconf++) {
         double *param_tmp = new double[Nparam];
         for (int iparam=0; iparam<Nparam; iparam++)
            param_tmp[iparam] = param[iconf + analysis::Nconf*iparam];
         
         T_MATRIX tmpT(E_min, E_max, E_div, 1);
         tmpT.calc_Tmat_dif(  param_tmp, &mass, &mass, &Nparam, &func_type
                            , lattice_space, ang_mom, delta, max_r );
         
         phase_shift::calc_phase_shift(tmpT, &phase(iconf));
         
         scattering_length[iconf]
         = observable::calc_scatt_length(  param_tmp, Nparam, func_type
                                         , energy_scatt_len, lattice_space
                                         , mass, ang_mom, delta, max_r );
         delete [] param_tmp;
         counter++;
         printf("\b\b\b\b%3.0lf%%",double(counter)/double(analysis::Nconf)*100);
         fflush(stdout);
         
      } // conf
      printf("\n");
      
      /// Output phase shift ///
      analysis::output_data_err(phase, outfile_path, use_JK_data);
      
      /// Output the scattering lengths ///
      double mean, err;
      analysis::make_mean_err(  scattering_length, mean, err
                              , analysis::Nconf, use_JK_data );
      
      printf(" @ scatt. length = %lf +/- %lf\n", mean, err);
      
      delete [] scattering_length;
      delete [] param;
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
         else if (strcmp(argv[loop],"-ifile")==0)
            snprintf(infile_path,sizeof(infile_path),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-ofile")==0)
            snprintf(outfile_path,sizeof(outfile_path),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-mass" )==0)
            mass = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-E_min" )==0)
            E_min = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-E_max" )==0)
            E_max = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-E_div" )==0)
            E_div = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-test" )==0)
            test_flg = true;
         else if (strcmp(argv[loop],"-V0"   )==0)
            V_0 = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-r0"   )==0)
            r_0 = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-check")==0)
            arguments_check = true;
         //***************************************************//
         else {
            printf("\n @@@@@@ invalid option \"%s\"\n", argv[loop]);
            analysis::usage(PROJECT);
            return 1;
         }
      }
   }
   
   printf("\n @ Arguments set :\n");
   printf(" @ infile     = %s\n",infile_path);
   printf(" @ outfile    = %s\n",outfile_path);
   printf(" @ lat space  = %lf\n",lattice_space);
   printf(" @ mass       = %lf\n",mass);
   printf(" @ ang mom    = %lf\n",ang_mom);
   printf(" @ E min      = %lf\n",E_min);
   printf(" @ E max      = %lf\n",E_max);
   printf(" @ E div      = %lf\n",E_div);
   printf(" @ delta      = %lf\n",delta);
   printf(" @ max_r      = %lf\n",max_r);
   printf(" @ E scat len = %lf\n",energy_scatt_len);
   printf(" @ use JK data= %s\n @\n",analysis::bool_to_str(use_JK_data).c_str());
   printf(" @ calc test      = %s\n",analysis::bool_to_str(test_flg).c_str());
   printf(" @ V_0 (For test) = %lf\n",V_0);
   printf(" @ r_0 (For test) = %lf\n\n",r_0);
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
      if (     strcmp(tmp_c1,"OBS_Parameter_list")==0)
         snprintf(infile_path,sizeof(infile_path),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Output_file_name"     )==0)
         snprintf(outfile_path,sizeof(outfile_path),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Lattice_spacing"     )==0)
         lattice_space = atof(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Reduced_mass"     )==0)
         mass = atof(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Angular_momentum")==0)
         ang_mom = atof(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Calc_energy_min"  )==0)
         E_min = atof(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Calc_energy_max")==0     )
         E_max = atof(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Calc_energy_div")==0     )
         E_div = atof(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Runge_kutta_delta"     )==0)
         delta = atof(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Runge_kutta_max_r"     )==0)
         max_r = atof(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_energy_scatt_len"     )==0)
         energy_scatt_len = atof(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Use_jack_knife_data")==0)
         use_JK_data = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Calc_squ_wall_pot"     )==0)
         test_flg = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Squ_wall_pot_V0")==0)
         V_0 = atof(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Squ_wall_pot_r0"  )==0)
         r_0 = atof(tmp_c2);
      //***************************************************//
      else{
         printf("\n @@@@@@ invalid option \"%s\" in arguments file \"%s\"\n\n"
                , tmp_c1, file_name);
         return 1;
      }
   }
   return 0;
}
