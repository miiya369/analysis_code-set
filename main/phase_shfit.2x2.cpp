//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup observable
 * @brief   Main part for observable
 * @author  Takaya Miyamoto
 * @since   Tue Jul  5 04:52:04 JST 2016
 */
//--------------------------------------------------------------------------

#include <observable/phase_shift.h>

#define PROJECT CALC_PHASE_SHIFT_2x2   // <- Project name

static double lattice_space;
static double mass[4];
static double ang_mom;
static double E_min;
static double E_max;
static double E_dev;
static double delta;
static double max_r;

static char infile_path[4][MAX_LEN_PATH];
static char outfile_path1[MAX_LEN_PATH];
static char outfile_path2[MAX_LEN_PATH];

static bool use_JK_data = false;

static bool arguments_check = false;
static int  set_args(int, char**);
static int  set_args_from_file(char*);

//========================================================================//
int main(int argc, char **argv)
{
   if (set_args(argc, argv) == 1) return 0;
   
   time_t start_time, end_time;
   time( &start_time );
   
   double mu[2], mass_threashold[2];
   for (int i=0; i<2; i++) {
      mu[i] = (mass[0+2*i] * mass[1+2*i]) / (mass[0+2*i] + mass[1+2*i]);
      mass_threashold[i] = mass[0+2*i] + mass[1+2*i];
   }
   
   int Nparam[4], func_type[4], SumNparam = 0;
   for (int i=0; i<4; i++) {
      observable::input_param(  infile_path[i], analysis::Nconf
                              , Nparam[i], func_type[i] );
      SumNparam += Nparam[i];
   }
   double *param[4];
   for (int i=0; i<4; i++) {
      param[i] = new double[analysis::Nconf * Nparam[i]];
      observable::input_param(infile_path[i], param[i]);
   }
   CONFIG<PHASE_SHIFT> phase[2];
   
   int counter = 0;
   printf(" @ phase shift calculating       |   0%%");
#ifdef _OPENMP
#pragma omp parallel for
#endif
   for (int conf=0; conf<analysis::Nconf; conf++) {
      double      *tmp_param = new double[SumNparam];
      PHASE_SHIFT  tmp_phase[2];
      tmp_phase[0].mem_alloc(E_min, E_max, E_dev);
      tmp_phase[1].mem_alloc(E_min, E_max, E_dev);
      
      int tmpN = 0;
      for (int i=0; i<4; i++) {
         for (int loop=0; loop<Nparam[i]; loop++)
            tmp_param[loop + tmpN] = param[i][conf+analysis::Nconf*loop];
         tmpN += Nparam[i];
      }
      observable::calc_phase_shift_dif(  tmp_phase, tmp_param, mu, mass_threashold
                                       , 2, Nparam, func_type
                                       , lattice_space, delta, max_r );
      
      phase[0](conf).mem_alloc(E_min, E_max, E_dev);
      phase[1](conf).mem_alloc(E_min, E_max, E_dev);
      for (size_t i=0; i<tmp_phase[0].data_size(); i++) {
         phase[0](conf)(i) = tmp_phase[0](i);
         phase[1](conf)(i) = tmp_phase[1](i);
      }
      
      delete [] tmp_param;
      counter++;
      printf("\b\b\b\b%3.0lf%%",double(counter)/double(analysis::Nconf)*100);
      fflush(stdout);
   } // conf
   printf("\n");
   
   observable::output_phase_shift_err(outfile_path1, phase[0], use_JK_data);
   observable::output_phase_shift_err(outfile_path2, phase[1], use_JK_data);
   
   for (int i=0; i<4; i++) delete [] param[i];
   
   time( &end_time );
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
         else if (strcmp(argv[loop],"-E_min" )==0)
            E_min = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-E_max" )==0)
            E_max = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-E_dev" )==0)
            E_dev = atof(argv[loop+1]);
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
   printf(" @ mass ch1-1 = %lf\n",mass[0]);
   printf(" @ mass ch1-2 = %lf\n",mass[1]);
   printf(" @ mass ch2-1 = %lf\n",mass[2]);
   printf(" @ mass ch2-2 = %lf\n",mass[3]);
   printf(" @ ang mom    = %lf\n",ang_mom);
   printf(" @ E min      = %lf\n",E_min);
   printf(" @ E max      = %lf\n",E_max);
   printf(" @ E dev      = %lf\n",E_dev);
   printf(" @ delta      = %lf\n",delta);
   printf(" @ max_r      = %lf\n",max_r);
   printf(" @ use JK data= %s\n",analysis::bool_to_str(use_JK_data).c_str());
   printf(" @ infile 00  = %s\n",infile_path[0]);
   printf(" @ infile 01  = %s\n",infile_path[1]);
   printf(" @ infile 10  = %s\n",infile_path[2]);
   printf(" @ infile 11  = %s\n",infile_path[3]);
   printf(" @ outfile 1  = %s\n",outfile_path1);
   printf(" @ outfile 2  = %s\n\n",outfile_path2);
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
      if (     strcmp(tmp_c1,"PH2_Parameter_list00")==0)
         snprintf(infile_path[0],sizeof(infile_path[0]),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"PH2_Parameter_list01"     )==0)
         snprintf(infile_path[1],sizeof(infile_path[1]),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"PH2_Parameter_list10"     )==0)
         snprintf(infile_path[2],sizeof(infile_path[2]),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"PH2_Parameter_list11"     )==0)
         snprintf(infile_path[3],sizeof(infile_path[3]),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"PH2_Output_file_name1"     )==0)
         snprintf(outfile_path1,sizeof(outfile_path1),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"PH2_Output_file_name2"     )==0)
         snprintf(outfile_path2,sizeof(outfile_path2),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"PH2_Lattice_spacing"     )==0)
         lattice_space = atof(tmp_c2);
      else if (strcmp(tmp_c1,"PH2_mass_ch1-1"     )==0)
         mass[0] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"PH2_mass_ch1-2"     )==0)
         mass[1] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"PH2_mass_ch2-1"     )==0)
         mass[2] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"PH2_mass_ch2-2"     )==0)
         mass[3] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"PH2_Angular_momentum")==0)
         ang_mom = atof(tmp_c2);
      else if (strcmp(tmp_c1,"PH2_Calc_energy_min"  )==0)
         E_min = atof(tmp_c2);
      else if (strcmp(tmp_c1,"PH2_Calc_energy_max")==0     )
         E_max = atof(tmp_c2);
      else if (strcmp(tmp_c1,"PH2_Calc_energy_dev")==0     )
         E_dev = atof(tmp_c2);
      else if (strcmp(tmp_c1,"PH2_Runge_kutta_delta"     )==0)
         delta = atof(tmp_c2);
      else if (strcmp(tmp_c1,"PH2_Runge_kutta_max_r"     )==0)
         max_r = atof(tmp_c2);
      else if (strcmp(tmp_c1,"PH2_Use_jack_knife_data")==0)
         use_JK_data = analysis::str_to_bool(tmp_c2);
      //***************************************************//
      else{
         printf("\n @@@@@@ invalid option \"%s\" in arguments file \"%s\"\n\n"
                , tmp_c1, file_name);
         return 1;
      }
   }
   return 0;
}
