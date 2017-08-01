//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup observable
 * @brief   Main part for calculation of T-matrix (S = 1 + 2iT)
 * @author  Takaya Miyamoto
 * @since   Tue Jul  5 04:52:04 JST 2016
 */
//--------------------------------------------------------------------------

#include <observable/phase_shift.h>

#define PROJECT CALC_T_MATRIX_2x2   // <- Project name

#define Tmat_Nch 2

static double lattice_space;
static double mass[Tmat_Nch*Tmat_Nch];
static int    ang_mom;
static double E_min;
static double E_max;
static double E_div;
static double delta;
static double max_r;

static char infile_path[Tmat_Nch*Tmat_Nch][MAX_LEN_PATH];
static char outfile_path[MAX_LEN_PATH];

static bool arguments_check = false;
static int  set_args(int, char**);
static int  set_args_from_file(char*);

//========================================================================//
int main(int argc, char **argv)
{
   if (set_args(argc, argv) == 1) return 0;
   
   time_t start_time, end_time;
   time(&start_time);
   
   /// Construct the mass threashold and reduced mass ///
   double mu[Tmat_Nch], mass_threashold[Tmat_Nch];
   for (int ich=0; ich<Tmat_Nch; ich++) {
      mu[ich] = (  (mass[0+2*ich] * mass[1+2*ich])
                 / (mass[0+2*ich] + mass[1+2*ich]) );
      
      mass_threashold[ich] = mass[0+2*ich] + mass[1+2*ich];
   }
   
   /// Input the miyamoto-format parameter data ///
   int Nparam[Tmat_Nch*Tmat_Nch], func_type[Tmat_Nch*Tmat_Nch];
   int tmp_Nconf, SumNparam = 0; analysis::Nconf = -999;
   for (int ich=0; ich<Tmat_Nch*Tmat_Nch; ich++) {
      observable::input_param(  infile_path[ich], tmp_Nconf
                              , Nparam[ich], func_type[ich] );
      
      if (tmp_Nconf != analysis::Nconf && analysis::Nconf != -999) {
         printf("ERROR: Different #.conf.\n");
         return -1;
      }
      analysis::Nconf = tmp_Nconf;
      SumNparam      += Nparam[ich];
   }
   double *param[Tmat_Nch*Tmat_Nch];
   for (int ich=0; ich<Tmat_Nch*Tmat_Nch; ich++) {
      param[ich] = new double[analysis::Nconf * Nparam[ich]];
      observable::input_param(infile_path[ich], param[ich]);
   }
   
   /// Calculate the T matrix ///
   CONFIG<T_MATRIX> Tmatrix;
   
   int counter = 0;
   printf(" @ phase shift calculating       |   0%%");
   
#ifdef _OPENMP
#pragma omp parallel for
#endif
   for (int iconf=0; iconf<analysis::Nconf; iconf++) {
      double *tmp_param = new double[SumNparam];
      Tmatrix(iconf).mem_alloc(E_min, E_max, E_div, Tmat_Nch);
      
      int tmpN = 0;
      for (int ich=0; ich<Tmat_Nch*Tmat_Nch; ich++) {
         for (int loop=0; loop<Nparam[ich]; loop++)
            tmp_param[loop + tmpN] = param[ich][iconf+analysis::Nconf*loop];
         tmpN += Nparam[ich];
      }
      Tmatrix(iconf).calc_Tmat_dif(  tmp_param, mu, mass_threashold
                                   , Nparam, func_type
                                   , lattice_space, ang_mom, delta, max_r );
      delete [] tmp_param;
      counter++;
      printf("\b\b\b\b%3.0lf%%",double(counter)/double(analysis::Nconf)*100);
      fflush(stdout);
      
   } // conf
   printf("\n");
   
   analysis::output_data_all(Tmatrix, outfile_path);
   
   for (int ich=0; ich<Tmat_Nch*Tmat_Nch; ich++) delete [] param[ich];
   
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
         else if (strcmp(argv[loop],"-ifile00"     )==0)
           snprintf(infile_path[0],sizeof(infile_path[0]),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-ifile01"     )==0)
           snprintf(infile_path[1],sizeof(infile_path[1]),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-ifile10"     )==0)
           snprintf(infile_path[2],sizeof(infile_path[2]),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-ifile11"     )==0)
           snprintf(infile_path[3],sizeof(infile_path[3]),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-ofile"     )==0)
           snprintf(outfile_path,sizeof(outfile_path),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-mass1-1"    )==0)
           mass[0] = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-mass1-2"    )==0)
           mass[1] = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-mass2-1"    )==0)
           mass[2] = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-mass2-2"    )==0)
           mass[3] = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-E_min" )==0)
	   E_min = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-E_max" )==0)
	   E_max = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-E_div" )==0)
	   E_div = atof(argv[loop+1]);
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
   printf(" @ ang mom    = %d\n" ,ang_mom);
   printf(" @ E min      = %lf\n",E_min);
   printf(" @ E max      = %lf\n",E_max);
   printf(" @ E div      = %lf\n",E_div);
   printf(" @ delta      = %lf\n",delta);
   printf(" @ max_r      = %lf\n",max_r);
   printf(" @ infile 00  = %s\n",infile_path[0]);
   printf(" @ infile 01  = %s\n",infile_path[1]);
   printf(" @ infile 10  = %s\n",infile_path[2]);
   printf(" @ infile 11  = %s\n",infile_path[3]);
   printf(" @ outfile    = %s\n\n",outfile_path);
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
      if (     strcmp(tmp_c1,"TM2_Parameter_list00")==0)
         snprintf(infile_path[0],sizeof(infile_path[0]),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"TM2_Parameter_list01"     )==0)
         snprintf(infile_path[1],sizeof(infile_path[1]),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"TM2_Parameter_list10"     )==0)
         snprintf(infile_path[2],sizeof(infile_path[2]),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"TM2_Parameter_list11"     )==0)
         snprintf(infile_path[3],sizeof(infile_path[3]),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"TM2_Output_file_name"     )==0)
         snprintf(outfile_path,sizeof(outfile_path),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"TM2_Lattice_spacing"     )==0)
         lattice_space = atof(tmp_c2);
      else if (strcmp(tmp_c1,"TM2_mass_ch1-1"     )==0)
         mass[0] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"TM2_mass_ch1-2"     )==0)
         mass[1] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"TM2_mass_ch2-1"     )==0)
         mass[2] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"TM2_mass_ch2-2"     )==0)
         mass[3] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"TM2_Angular_momentum")==0)
         ang_mom = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"TM2_Calc_energy_min"  )==0)
         E_min = atof(tmp_c2);
      else if (strcmp(tmp_c1,"TM2_Calc_energy_max")==0     )
         E_max = atof(tmp_c2);
      else if (strcmp(tmp_c1,"TM2_Calc_energy_div")==0     )
         E_div = atof(tmp_c2);
      else if (strcmp(tmp_c1,"TM2_Runge_kutta_delta"     )==0)
         delta = atof(tmp_c2);
      else if (strcmp(tmp_c1,"TM2_Runge_kutta_max_r"     )==0)
         max_r = atof(tmp_c2);
      //***************************************************//
      else{
         printf("\n @@@@@@ invalid option \"%s\" in arguments file \"%s\"\n\n"
                , tmp_c1, file_name);
         return 1;
      }
   }
   return 0;
}
