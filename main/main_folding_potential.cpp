//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting, potential
 * @brief   Main part for calculation of single-folding potential
 * @author  Takaya Miyamoto
 * @since   Thu Nov 10 11:40:58 JST 2016
 */
//--------------------------------------------------------------------------

#include <potential/folding_potential.h>
#include <observable/phase_shift.h>

#define PROJECT FOLDING_POTENTIAL_GAUSS   // <- Project name

static int A; // #.nucleus

static double lat_space;

static double r0;    // [fm] (Nuclei radias)
static double aA;    // [fm] ( Diffuseness )

static int    Nplot;     // ( #.plot for output potential  )
static double max_r_out; // (Max range for output potential)

static double Ndiv;

static string density_name;
static DENSITY_TYPE_BASE *density = NULL;

static int Nparam;
static int func_type;

static bool use_JK_data   = false;
static bool test_flg      = false;
static bool out_phase_flg = false;

static double mass;
static double E_min;
static double E_max;
static double E_div;
static double delta;
static double max_r;
static double energy_scatt_len;

static char infile_path[MAX_LEN_PATH];
static char outfile_path[MAX_LEN_PATH];

static bool arguments_check = false;
static int  set_args(int, char**);
static int  set_args_from_file(char*);

//========================================================================//
int main(int argc, char **argv)
{
   if (set_args(argc, argv) == 1) return 0;
   
   // TODO: gaussian型のpotentialであれば１重積分で良いが、
   //       一般の（球対称な）ポテンシャルの場合は２重積分（r+theta）
   //       が必要になってくる。
   
   time_t start_time, end_time;
   time(&start_time);
   
   if        (density_name == "Gaussian"    ) {
      density = new DENSITY_GAUSSIAN;
      density -> set(A, r0/lat_space);
      
   } else if (density_name == "Woods-Saxon") {
      density = new DENSITY_WOODS_SAXON;
      density -> set(A, r0/lat_space, aA/lat_space);
      
   } else {
      printf("\nERROR: Invalid density type, '%s'\n", density_name.c_str());
      printf("     : [ Gaussian, Woods-Saxon ]\n");
      return 1;
   }
   
   double *params = new double[analysis::Nconf * Nparam];
   observable::input_param(infile_path, params);
   
   if (!out_phase_flg)
   {
      CONFIG<F_POTENTIAL_GAUSS> fpot(analysis::Nconf);
      
      /// Calculate the folding potentials ///
      int counter = 0;
      printf("# @ Calculate the folding potentials ... |   0%%");
      fflush(stdout);
      
#ifdef _OPENMP
#pragma omp parallel for
#endif
      for (int iconf=0; iconf<analysis::Nconf; iconf++) {
         fpot(iconf).set(Nplot, max_r_out);
         
         double *param_tmp = new double[Nparam];
         for (int iparam=0; iparam<Nparam; iparam++)
            param_tmp[iparam] = params[iconf + analysis::Nconf*iparam];
         
         if (test_flg)
            fpot(iconf).calc_fpot_test(param_tmp, Nparam, A, density -> RA());
         else
            fpot(iconf).calc_fpot(param_tmp, Nparam, density[0], Ndiv);
         
         delete [] param_tmp;
         
         counter++;
         printf("\b\b\b\b%3.0lf%%",double(counter)/double(analysis::Nconf)*100);
         fflush(stdout);
      }
      printf("\n");
      
      /// Output the potentials ///
      char outfile_name[1024];
      snprintf(outfile_name, sizeof(outfile_name), "%s.err", outfile_path);
      analysis::output_data_err(fpot, outfile_name, use_JK_data);
      
      snprintf(outfile_name, sizeof(outfile_name), "%s.fit", outfile_path);
      analysis::output_data_fit(fpot, outfile_name, use_JK_data);
   }
   else
   {
      CONFIG<PHASE_SHIFT> phase(analysis::Nconf);
      double *scattering_length = new double[analysis::Nconf];
      
      /// Calculate the phase shift for the folding potentials ///
      printf(" @ phase shift calculating       |   0%%");
      fflush(stdout);
      
      for (int iconf=0; iconf<analysis::Nconf; iconf++) {
         double *param_tmp = new double[Nparam];
         for (int iparam=0; iparam<Nparam; iparam++)
            param_tmp[iparam] = params[iconf + analysis::Nconf*iparam];
         
         T_MATRIX tmpT(E_min, E_max, E_div, 1);
         tmpT.calc_Tmat_fpot(  density[0], param_tmp, mass, Nparam
                             , func_type, lat_space, delta, max_r, Ndiv );
         // NB. OpenMP is used in the above function.
         
         phase_shift::calc_phase_shift(tmpT, &phase(iconf));
         
         scattering_length[iconf]
         = observable::calc_scatt_length_fpot(  density[0], param_tmp, Nparam
                                              , func_type, energy_scatt_len
                                              , lat_space, mass, delta
                                              , max_r, Ndiv );
         delete [] param_tmp;
         
         printf("\b\b\b\b%3.0lf%%",double(iconf+1)/double(analysis::Nconf)*100);
         fflush(stdout);
      } // conf
      printf("\n");
      
      for (int idata=0; idata<phase(0).data_size(); idata++) {
	printf("%lf", phase(0).E(idata));
	for (int iconf=0; iconf<analysis::Nconf; iconf++)
	  printf(" %lf", phase(iconf)(idata));
	printf("\n");
      }
      /// Output phase shift ///
      analysis::output_data_err(phase, outfile_path, use_JK_data);
      
      /// Output the scattering lengths ///
      double mean, err;
      analysis::make_mean_err(  scattering_length, mean, err
                              , analysis::Nconf, use_JK_data );
      
      printf("# @ scatt. length = %lf +/- %lf\n", mean, err);
      
      delete [] scattering_length;
   }
   
   time(&end_time);
   printf("#\n# @ JOB END : ELAPSED TIME [s] = %d\n#\n"
          ,(int)difftime(end_time,start_time) );
   
   delete params;
   delete density;
   
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
         else if (strcmp(argv[loop],"-A"       )==0)
            A = atoi(argv[loop+1]);
	 else if (strcmp(argv[loop],"-r0"       )==0)
            r0 = atof(argv[loop+1]);
	 else if (strcmp(argv[loop],"-dif"       )==0)
            aA = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-Nplot"   )==0)
            Nplot = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-Ndiv"    )==0)
            Ndiv = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-max_r"   )==0)
            max_r_out = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-DensName")==0)
            density_name = argv[loop+1];
         else if (strcmp(argv[loop],"-ifile"   )==0)
            snprintf(infile_path,sizeof(infile_path),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-ofile"   )==0)
            snprintf(outfile_path,sizeof(outfile_path),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-TEST"   )==0)
            test_flg = true;
         else if (strcmp(argv[loop],"-phase"  )==0)
            out_phase_flg = true;
         else if (strcmp(argv[loop],"-mass" )==0)
            mass = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-E_min" )==0)
            E_min = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-E_max" )==0)
            E_max = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-E_div" )==0)
            E_div = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-check"   )==0)
            arguments_check = true;
         //***************************************************//
         else {
            printf("\n @@@@@@ invalid option \"%s\"\n", argv[loop]);
            analysis::usage(PROJECT);
            return 1;
         }
      }
   }
   
   observable::input_param(infile_path, analysis::Nconf, Nparam, func_type);
   
   if (func_type != 3 && // Now, This code can use only Gaussian-type potentials.
       func_type != 4 &&
       func_type != 5 &&
       func_type != 8) {
      printf("ERROR: ONLY Gaussian-type fitted potentials are compatible.\n");
      return 1;
   }
   
   printf("#\n# @ Arguments set :\n");
   printf("# @ infile       = %s\n",infile_path);
   printf("# @ outfile      = %s\n",outfile_path);
   printf("# @ lat space    = %lf\n",lat_space);
   printf("# @ #.nucleus    = %d\n",A);
   printf("# @ density name = %s\n",density_name.c_str());
   printf("# @ r0           = %lf\n",r0);
   printf("# @ Diffuseness  = %lf\n",aA);
   printf("# @ #.plot       = %d\n",Nplot);
   printf("# @ Max r        = %lf\n",max_r_out);
   printf("# @ #.div int    = %lf\n",Ndiv);
   printf("# @ use JK data  = %s\n# @\n",analysis::bool_to_str(use_JK_data).c_str());
   printf("# @ out phs-sft. = %s\n",analysis::bool_to_str(out_phase_flg).c_str());
   printf("# @ mass         = %lf\n",mass);
   printf("# @ E min        = %lf\n",E_min);
   printf("# @ E max        = %lf\n",E_max);
   printf("# @ E div        = %lf\n",E_div);
   printf("# @ delta        = %lf\n",delta);
   printf("# @ max_r        = %lf\n",max_r);
   printf("# @ E scat len   = %lf\n#\n#",energy_scatt_len);
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
      if      (strcmp(tmp_c1,"FPOT_Input_file_name"   )==0)
         snprintf(infile_path,sizeof(infile_path),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"FPOT_Output_file_name"  )==0)
         snprintf(outfile_path,sizeof(outfile_path),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"FPOT_Number_of_A"       )==0)
         A = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"FPOT_Lattice_spacing"   )==0)
         lat_space = atof(tmp_c2);
      else if (strcmp(tmp_c1,"FPOT_Nucleus_radias_r0" )==0)
         r0 = atof(tmp_c2);
      else if (strcmp(tmp_c1,"FPOT_Diffuseness"       )==0)
         aA = atof(tmp_c2);
      else if (strcmp(tmp_c1,"FPOT_Number_of_plot"    )==0)
         Nplot = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"FPOT_Max_range_for_fpot")==0)
         max_r_out = atof(tmp_c2);
      else if (strcmp(tmp_c1,"FPOT_Type_of_density"   )==0)
         density_name = tmp_c2;
      else if (strcmp(tmp_c1,"FPOT_Use_jack_knife_data")==0)
         use_JK_data = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"FPOT_Ndiv_for_Integral" )==0)
         Ndiv = atof(tmp_c2);
      else if (strcmp(tmp_c1,"FPOT_Out_phase_shift")==0)
         out_phase_flg = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"FPOT_Reduced_mass"     )==0)
         mass = atof(tmp_c2);
      else if (strcmp(tmp_c1,"FPOT_Calc_energy_min"  )==0)
         E_min = atof(tmp_c2);
      else if (strcmp(tmp_c1,"FPOT_Calc_energy_max")==0     )
         E_max = atof(tmp_c2);
      else if (strcmp(tmp_c1,"FPOT_Calc_energy_div")==0     )
         E_div = atof(tmp_c2);
      else if (strcmp(tmp_c1,"FPOT_Runge_kutta_delta"     )==0)
         delta = atof(tmp_c2);
      else if (strcmp(tmp_c1,"FPOT_Runge_kutta_max_r"     )==0)
         max_r = atof(tmp_c2);
      else if (strcmp(tmp_c1,"FPOT_energy_scatt_len"     )==0)
         energy_scatt_len = atof(tmp_c2);
      //***************************************************//
      else{
         printf("\n @@@@@@ invalid option \"%s\" in arguments file \"%s\"\n\n"
                , tmp_c1, file_name);
         return 1;
      }
   }
   return 0;
}
