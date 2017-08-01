//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup test
 * @brief   Main part for observable
 * @author  Takaya Miyamoto
 * @since   Sat Oct  8 01:24:08 JST 2016
 */
//--------------------------------------------------------------------------

#include <observable/phase_shift.h>

static double lattice_space = 0.0907;
static double mass  = 1000;
static double E_min = 0;
static double E_max = 200;
static double E_div = 1;
static double delta = 0.0005;
static double max_r = 6;
static double energy_scatt_len = 0.000000000001;

static char infile_path[MAX_LEN_PATH];
static char outfile_path[MAX_LEN_PATH];

static bool use_JK_data = true;

static int  set_args(int, char**);
//========================================================================//
int main(int argc, char **argv) {
   if (set_args(argc, argv) == 1) return 0;
   
   time_t start_time, end_time;
   time(&start_time);
   
   int Nparam, func_type;
   observable::input_param(infile_path, analysis::Nconf, Nparam, func_type);
   
   double *param = new double[analysis::Nconf * Nparam];
   observable::input_param(infile_path, param);
   
   CONFIG<PHASE_SHIFT> phase(analysis::Nconf);
   
   int counter = 0;
   printf(" @ phase shift calculating       |   0%%");
#ifdef _OPENMP
#pragma omp parallel for
#endif
   for (int conf=0; conf<analysis::Nconf; conf++) {
     double *param_tmp = new double[Nparam];
     
     phase(conf).mem_alloc(E_min, E_max, E_div);
     
     for (int loop=0; loop<Nparam; loop++)
       param_tmp[loop] = param[conf+analysis::Nconf*loop];
     
     observable::calc_phase_shift_eq(  phase(conf), param_tmp, mass, Nparam, func_type
				     , lattice_space, delta, max_r );
     delete [] param_tmp;
     counter++;
     printf("\b\b\b\b%3.0lf%%",double(counter)/double(analysis::Nconf)*100);
     fflush(stdout);
   } // conf
   printf("\n");
   
   observable::output_phase_shift_err(outfile_path, phase, use_JK_data);
   
   printf(" @ scattering length calculating |   0%%");
   double *param_tmp = new double[Nparam];
   double  mean = 0.0, sqr_mean = 0.0;
   cdouble tmp_a;
   for (int conf=0; conf<analysis::Nconf; conf++) {
     
     for (int loop=0; loop<Nparam; loop++)
       param_tmp[loop] = param[conf+analysis::Nconf*loop];
     
     tmp_a = observable::calc_scatt_length(  param_tmp, Nparam, func_type
					     , energy_scatt_len, lattice_space
					     , mass, delta, max_r);
     mean     += tmp_a.real();
     sqr_mean += tmp_a.real() * tmp_a.real();
     
     printf("\b\b\b\b%3.0lf%%",double(conf+1)/double(analysis::Nconf)*100);
     fflush(stdout);
   }
   printf("\n");
   
   double factor;
   if (use_JK_data)
     factor = double(analysis::Nconf-1);
   else
     factor = 1.0 / double(analysis::Nconf-1);
   
   mean     /= double(analysis::Nconf);
   sqr_mean /= double(analysis::Nconf);
   printf(" @ scatt. length = %lf +/- %lf\n", mean
	  , sqrt(factor * (sqr_mean - pow(mean,2)) ) );
   
   delete [] param_tmp;
   delete [] param;
   
   time(&end_time);
   printf("\n @ JOB END : ELAPSED TIME [s] = %d\n\n"
          ,(int)difftime(end_time,start_time) );
   return 0;
}
//========================================================================//

static int set_args(int argc, char** argv) {   
  bool arguments_check = false;
  snprintf(outfile_path,sizeof(outfile_path),"test_phase_result");

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
         else if (strcmp(argv[loop],"-check")==0)
            arguments_check = true;
         //***************************************************//
         else {
            printf("\n @@@@@@ invalid option \"%s\"\n", argv[loop]);
	    return 1;
         }
      }
   }
   
   printf("\n @ Arguments set :\n");
   printf(" @ lat space  = %lf\n",lattice_space);
   printf(" @ mass       = %lf\n",mass);
   printf(" @ E min      = %lf\n",E_min);
   printf(" @ E max      = %lf\n",E_max);
   printf(" @ E div      = %lf\n",E_div);
   printf(" @ delta      = %lf\n",delta);
   printf(" @ max_r      = %lf\n",max_r);
   printf(" @ E scat len = %lf\n",energy_scatt_len);
   printf(" @ use JK data= %s\n",analysis::bool_to_str(use_JK_data).c_str());
   printf(" @ infile     = %s\n",infile_path);
   printf(" @ outfile    = %s\n\n",outfile_path);
   fflush(stdout);
   
   if (arguments_check) return 1;
   return 0;
}
