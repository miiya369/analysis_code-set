//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup observable
 * @brief   Main part for observable
 * @author  Takaya Miyamoto
 * @since   Mon Sep  7 17:32:12 JST 2015
 */
//--------------------------------------------------------------------------

#include <observable/phase_shift.h>

#define PROJECT CALC_OBSERVABLE   // <- Project name

int  analysis::Nconf;
int  analysis::xSIZE;
int  analysis::ySIZE;
int  analysis::zSIZE;
int  analysis::tSIZE;

double lattice_space;
double mass;
double ang_mom;
double E_min;
double E_max;
double E_dev;
double delta;
double max_r;
double V_0;
double r_0;

char infile_path[MAX_LEN_PATH];
char outfile_path[MAX_LEN_PATH];

bool test_flg           = false;
bool calc_scatt_len_flg = false;

bool arguments_check = false;
int  set_args(int, char**);
int  set_args_from_file(char*);

//========================================================================//
int main(int argc, char **argv) {
   if (set_args(argc, argv) == 1) return 0;
   
   time_t start_time, end_time;
   time( &start_time );
   
   char scatt_length_filename[1024];
   snprintf(scatt_length_filename, sizeof(scatt_length_filename)
            ,"%s_scattering_length",outfile_path);
   
   if (test_flg) {
      CONFIG<PHASE_SHIFT> test(1);
      double param[2] = { r_0, V_0 };
      lattice_space   = 1;
      
      test(0).mem_alloc( E_min, E_max, E_dev );
      
      printf(" @ phase shift calculating ... ");
      observable::calc_phase_shift(  test(0), param, 2, 0, lattice_space
                                   , mass, ang_mom, delta, max_r );
      printf("DONE\n");
      
      observable::output_phase_shift_err( outfile_path, test );
      
      if (calc_scatt_len_flg)
         observable::output_scatt_length( scatt_length_filename, test, mass );
   }
   else {
      int Nparam, func_type;
      observable::input_param( infile_path, analysis::Nconf, Nparam, func_type );
      
      double *param = new double[analysis::Nconf * Nparam];
      observable::input_param( infile_path, param );
      
      CONFIG<PHASE_SHIFT> phase(analysis::Nconf);
      
      int counter = 0;
      printf(" @ phase shift calculating |   0%%");
#ifdef _OPENMP
#pragma omp parallel for
#endif
      for (int conf=0; conf<analysis::Nconf; conf++) {
         double *param_tmp = new double[Nparam];
         
         phase(conf).mem_alloc( E_min, E_max, E_dev );
         
         for (int loop=0; loop<Nparam; loop++)
            param_tmp[loop] = param[conf+analysis::Nconf*loop];
            
         observable::calc_phase_shift(  phase(conf), param_tmp, Nparam, func_type
                                      , lattice_space
                                      , mass, ang_mom, delta, max_r );
         delete [] param_tmp;
         counter++;
         printf("\b\b\b\b%3.0lf%%",double(counter)/double(analysis::Nconf)*100);
         fflush(stdout);
      } // conf
      printf("\n");
      
      observable::output_phase_shift_err( outfile_path, phase );
      
      if (calc_scatt_len_flg)
         observable::output_scatt_length( scatt_length_filename, phase, mass );
      delete [] param;
   }
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
         else if (strcmp(argv[loop],"-ifile")==0)
            snprintf(infile_path,sizeof(infile_path),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-ofile")==0)
            snprintf(outfile_path,sizeof(outfile_path),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-mass" )==0)
            mass = atof(argv[loop+1]);
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
   printf(" @ lat space  = %lf\n",lattice_space);
   printf(" @ mass       = %lf\n",mass);
   printf(" @ ang mom    = %lf\n",ang_mom);
   printf(" @ E min      = %lf\n",E_min);
   printf(" @ E max      = %lf\n",E_max);
   printf(" @ E dev      = %lf\n",E_dev);
   printf(" @ delta      = %lf\n",delta);
   printf(" @ max_r      = %lf\n",max_r);
   printf(" @ infile     = %s\n",infile_path);
   printf(" @ outfile    = %s\n @\n",outfile_path);
   printf(" @ calc test      = %s\n",analysis::bool_to_str(test_flg).c_str());
   printf(" @ V_0 (For test) = %lf\n",V_0);
   printf(" @ r_0 (For test) = %lf\n @\n",r_0);
   printf(" @ calc sctt len  = %s\n\n"
          ,analysis::bool_to_str(calc_scatt_len_flg).c_str());
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
      else if (strcmp(tmp_c1,"OBS_Calc_energy_dev")==0     )
         E_dev = atof(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Runge_kutta_delta"     )==0)
         delta = atof(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Runge_kutta_max_r"     )==0)
         max_r = atof(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Calc_squ_wall_pot"     )==0)
         test_flg = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Squ_wall_pot_V0")==0)
         V_0 = atof(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Squ_wall_pot_r0"  )==0)
         r_0 = atof(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Calc_scatt_length"  )==0)
         calc_scatt_len_flg = analysis::str_to_bool(tmp_c2);
      //***************************************************//
      else{
         printf("\n @@@@@@ invalid option \"%s\" in arguments file \"%s\"\n\n"
                , tmp_c1, file_name);
         return 1;
      }
   }
   return 0;
}
