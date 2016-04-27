//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup observable
 * @brief   Main part for analysis of fit parameter
 * @author  Takaya Miyamoto
 * @since   Tue Apr 26 21:15:13 JST 2016
 */
//--------------------------------------------------------------------------

#include <observable/observable_base.h>
#include <fitting/fitting.h>

#define PROJECT ANALYSIS_FIT_PARAM   // <- Project name

static double max_r;
static double div_r;
static double lat_space;

static char infile_path[MAX_LEN_PATH];
static char outfile_path[MAX_LEN_PATH];

static bool physical_unit = false;
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
   
   int Nparam, func_type;
   observable::input_param(infile_path, analysis::Nconf, Nparam, func_type);
   
   double *param = new double[analysis::Nconf * Nparam];
   observable::input_param(infile_path, param);
   
   FIT_FUNCTION fit_func(func_type);
   printf(" @ #.confs       = %d\n", analysis::Nconf);
   printf(" @ #.param       = %d\n", Nparam);
   printf(" @ function type = %s\n\n", fit_func.name.c_str());
   
   double *mean_param = new double[Nparam];
   for (int i=0; i<Nparam; i++)
   {
      mean_param[i] = 0.0;
      
      for (int conf=0; conf<analysis::Nconf; conf++)
         mean_param[i] += param[conf + analysis::Nconf* i];
      
      mean_param[i] /= double(analysis::Nconf);
   }
   FIT *tmp = new FIT;
   tmp->set_func(fit_func.number, mean_param);
   tmp->print_param();
   tmp->print_func_gnu();
   
   delete [] mean_param;
   delete tmp;
   
   int Npoint = int(max_r / div_r);
   
   double *max_func = new double[Npoint];
   double *min_func = new double[Npoint];
   double *mean_func = new double[Npoint];
   double *err_func = new double[Npoint];
   
   double hc_api = hbar_c / lat_space;
   double factor;
   
   if (!physical_unit)
   {
      lat_space = 1.0;
      hc_api    = 1.0;
   }
   if (use_JK_data) factor = analysis::Nconf - 1;
   else             factor = 1 / (analysis::Nconf - 1);
   
   int counter = 0;
   printf("\n @ Analysing fit parameter file ... |   0%%");
#ifdef _OPENMP
#pragma omp parallel for
#endif
   for (int Np=0; Np<Npoint; Np++)
   {
      double r = Np * div_r;
      double tmpV, tmpV_max = -10e10, tmpV_min = 10e10;
      
      double tmp_2mean = 0.0;
      mean_func[Np]    = 0.0;
      
      double *tmp_param = new double[Nparam];
      
      for (int conf=0; conf<analysis::Nconf; conf++)
      {
         for (int i=0; i<Nparam; i++)
            tmp_param[i] = param[conf + analysis::Nconf* i];
         tmpV = observable::V(r/lat_space, tmp_param, Nparam, func_type)* hc_api;
         
         if (tmpV_max < tmpV) tmpV_max = tmpV;
         if (tmpV_min > tmpV) tmpV_min = tmpV;
         
         mean_func[Np] += tmpV;
         tmp_2mean     += tmpV * tmpV;
      }
      delete [] tmp_param;
      
      max_func[Np] = tmpV_max;
      min_func[Np] = tmpV_min;
      
      mean_func[Np] /= (double)analysis::Nconf;
      tmp_2mean     /= (double)analysis::Nconf;
      
      err_func[Np] = sqrt(factor * (tmp_2mean - pow(mean_func[Np], 2)));
      
      counter++;
      printf("\b\b\b\b%3.0lf%%",double(counter)/double(Npoint)*100);
      fflush(stdout);
   }
   printf("\n");
   
   ofstream ofs(outfile_path);
   ofs << "# function type = " << fit_func.name << endl;
   for (int Np=0; Np<Npoint; Np++)
   {
      double r = Np * div_r;
      ofs << r << " " << max_func[Np] << " " << min_func[Np]
      << " " << mean_func[Np] << " " << err_func[Np] << endl;
   }
   ofs.close();
   
   delete [] param;
   delete [] max_func;
   delete [] min_func;
   delete [] mean_func;
   delete [] err_func;
   
   time(&end_time);
   printf("\n @ JOB END : ELAPSED TIME [s] = %d\n\n"
          ,(int)difftime(end_time,start_time) );
   return 0;
}
//========================================================================//

static int set_args(int argc, char** argv)
{
   if (argc == 1)
   {
      analysis::usage(PROJECT);
      return 1;
   }
   bool infile_flg = false;
   
   for (int loop=1; loop<argc; loop++)
      if (strcmp(argv[loop],"-f")==0)
      {
         if (set_args_from_file(argv[loop+1]) == 1) return 1;
         infile_flg = true;
      }
   if (!infile_flg)
   {
      printf("\n @@@@@@ no input arguments file\n");
      analysis::usage(PROJECT);
      return 1;
   }
   for (int loop=1; loop<argc; loop++)
   {
      if (argv[loop][0] == '-')
      {
         if (     strcmp(argv[loop],"-f"    )==0){}
         //****** You may set additional potion in here ******//
         else if (strcmp(argv[loop],"-ifile")==0)
            snprintf(infile_path,sizeof(infile_path),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-ofile")==0)
            snprintf(outfile_path,sizeof(outfile_path),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-max_r")==0)
            max_r = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-div_r")==0)
            div_r = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-phys")==0)
            physical_unit = true;
         else if (strcmp(argv[loop],"-check")==0)
            arguments_check = true;
         //***************************************************//
         else
         {
            printf("\n @@@@@@ invalid option \"%s\"\n", argv[loop]);
            analysis::usage(PROJECT);
            return 1;
         }
      }
   }
   
   printf("\n @ Arguments set :\n");
   printf(" @ lat space  = %lf\n",lat_space);
   printf(" @ infile     = %s\n",infile_path);
   printf(" @ outfile    = %s\n @\n",outfile_path);
   printf(" @ max r      = %lf\n",max_r);
   printf(" @ div r      = %lf\n @\n",div_r);
   printf(" @ phys. unit = %s\n",analysis::bool_to_str(physical_unit).c_str());
   printf(" @ use JK data= %s\n\n",analysis::bool_to_str(use_JK_data).c_str());
   fflush(stdout);
   
   if (arguments_check) return 1;
   return 0;
}

static int set_args_from_file(char* file_name)
{
   ifstream ifs(file_name, ios::in);
   if (!ifs)
   {
      printf("\n @@@@@@ the file \"%s\" is not exist\n\n", file_name);
      return 1;
   }
   char tmp_str[MAX_N_DATA][MAX_LEN_PATH];
   char tmp_c1[32];
   char tmp_c2[MAX_LEN_PATH];
   int  count = 0;
   while (ifs.getline(tmp_str[count],sizeof(tmp_str[count]))) count++;
   
   for (int loop=0; loop<count; loop++)
   {
      if (sscanf(tmp_str[loop]," %s = %s ",tmp_c1,tmp_c2) != 2)
         continue;
      //****** You may set additional potion in here ******//
      if (     strcmp(tmp_c1,"PARAM_Parameter_list"     )==0)
         snprintf(infile_path,sizeof(infile_path),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"PARAM_Output_file_name"   )==0)
         snprintf(outfile_path,sizeof(outfile_path),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"PARAM_out_physical_unit"  )==0)
         physical_unit = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"PARAM_Max_r"              )==0)
         max_r = atof(tmp_c2);
      else if (strcmp(tmp_c1,"PARAM_Div_r"              )==0)
         div_r = atof(tmp_c2);
      else if (strcmp(tmp_c1,"PARAM_Lattice_spacing"    )==0)
         lat_space = atof(tmp_c2);
      else if (strcmp(tmp_c1,"PARAM_Use_jack_knife_data")==0)
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
