//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Main part for fitting analysis
 * @author  Takaya Miyamoto
 * @since   Wed Apr 27 01:19:22 JST 2016
 */
//--------------------------------------------------------------------------

#include <fitting/fitting.h>

#define PROJECT FITTING_DATA   // <- Project name

static int     n_param;
static double* param_initial = NULL;
static double  stp_cnd;
static string  fit_func_name;
static int     fit_range_min;
static int     fit_range_max;

static char infile_path[MAX_LEN_PATH];
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
   
   int  Ndata;
   FIT *tmp = new FIT;
   tmp->set_func(fit_func_name.c_str(), param_initial);
   tmp->print_func_gnu();
   
   fitting::input_data_binary(infile_path, analysis::Nconf, Ndata);
   
   double *data_all = new double[Ndata * analysis::Nconf];
   double *cood     = new double[Ndata];
   double *err      = new double[Ndata];
   
   fitting::input_data_binary(infile_path, cood, data_all, err);
   
   CONFIG<FIT> *fit   = new CONFIG<FIT>;
   double      *chisq = new double[analysis::Nconf];
   double      *param = new double[analysis::Nconf * n_param];
   
   int counter = 0;
   printf(" @ data fitting |   0%%");
//#pragma omp parallel for
   for (int conf=0; conf<analysis::Nconf; conf++)
   {
      double *data = new double[Ndata];
      for (int loop=0; loop<Ndata; loop++)
         data[loop] = data_all[conf+analysis::Nconf*loop];
      
      (*fit)(conf).set_func(fit_func_name.c_str(), param_initial);
      chisq[conf] = (*fit)(conf).fit_data_NR(cood, data, err, Ndata, 0.000001);
      
      for (int loop=0; loop<n_param; loop++)
         param[conf+analysis::Nconf*loop] = (*fit)(conf)(loop);
      
      delete [] data;
      
      counter++;
      printf("\b\b\b\b%3.0lf%%",double(counter)/double(analysis::Nconf)*100);
      fflush(stdout);
   } // conf
   printf("\n\n");
   
   delete [] data_all;
   delete [] cood;
   delete [] err;
   
   double *chisq_new = new double[analysis::Nconf];
   double *param_new = new double[analysis::Nconf * n_param];
   
   counter = 0;
   for (int conf=0; conf<analysis::Nconf; conf++)
   {
      if (chisq[conf] != -999)
      {
         chisq_new[counter] = chisq[conf];
         counter++;
      }
//      else
//         printf("FITTING FAILURE: Num conf = %d\n", conf);   // For Debug
   }
   printf(" @ #.successful of fit = %d/%d\n\n", counter, analysis::Nconf);
   int Nconf_org   = analysis::Nconf;
   analysis::Nconf = counter; // Change Nconf to #.successful of fit
   
   double param_mean, param_err, chisq_mean, chisq_err;
   analysis::make_mean_err(  chisq_new, chisq_mean, chisq_err
                           , analysis::Nconf, true );
   printf(" @ Fitting Results  : chisq/dof = %lf +/- %lf\n",chisq_mean,chisq_err);
   
   counter = 0;
   for (int conf=0; conf<Nconf_org; conf++)
      if (chisq[conf] != -999)
      {
         for (int loop=0; loop<n_param; loop++)
            param_new[counter+analysis::Nconf*loop] = param[conf+Nconf_org*loop];
         counter++;
      }
   for (int loop=0; loop<n_param; loop++)
   {
      analysis::make_mean_err(  &param_new[analysis::Nconf*loop]
                              , param_mean, param_err, analysis::Nconf, true );
      printf(" @                  : param[%1d]  = %lf +/- %lf\n"
             , loop, param_mean, param_err);
      
      (*tmp)(loop) = param_mean;
   }
   tmp->print_param();
   tmp->print_func_gnu();
   
   fit->mem_alloc(analysis::Nconf);
   for (int conf=0; conf<analysis::Nconf; conf++)
   {
      (*fit)(conf).set_func(fit_func_name.c_str(), param_initial);
      
      for (int loop=0; loop<n_param; loop++)
         (*fit)(conf)(loop) = param_new[conf+analysis::Nconf*loop];
   }
   if (chisq_mean < 2.0) fitting::output_param(*fit, outfile_path);
   else printf("\n @@@@@@ CHI SQUARE > 2.0 !\n %s will not be output.\n"
               , outfile_path);
   
   delete [] param_initial;
   delete fit;
   delete tmp;
   
   time(&end_time);
   printf("\n @ JOB END : ELAPSED TIME [s] = %d\n\n"
          ,(int)difftime(end_time,start_time) );
   return 0;
}
//========================================================================//

static int set_args(int argc, char** argv) {
   
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
         else if (strcmp(argv[loop],"-ifile"   )==0)
            snprintf(infile_path,sizeof(infile_path),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-ofile"   )==0)
            snprintf(outfile_path,sizeof(outfile_path),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-t_min"   )==0)
            fit_range_min = atoi(argv[loop+1]);
         else if (strcmp(argv[loop],"-t_max"   )==0)
            fit_range_max = atoi(argv[loop+1]);
         else if (strcmp(argv[loop],"-fit_func")==0)
            fit_func_name = argv[loop+1];
         else if (strcmp(argv[loop],"-param"   )==0) {
            int count_tmp = 0;
            do {
               if (loop+count_tmp == argc-1) {
                  printf("\n @@@@@@ Need \"@\" at the end of -param option\n\n");
                  return 1;
               }
               count_tmp++;
            } while (strcmp(argv[loop+count_tmp],"@") != 0);
            n_param = count_tmp-1;
            if (param_initial != NULL) delete [] param_initial;
            param_initial = new double[n_param];
            for (int n=0; n<n_param; n++){
               loop++;
               param_initial[n] = atof(argv[loop]);
            }
         }
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
   
   printf("\n @ Arguments set :\n");
   printf(" @ stop cond   = %lf\n",stp_cnd);
   printf(" @ func type   = %s\n",fit_func_name.c_str());
   printf(" @ #. param    = %d\n @ param       = { ",n_param);
   for(int loop=0; loop<n_param; loop++) printf("%lf ",param_initial[loop]);
   printf("}\n @ range min   = %d\n",fit_range_min);
   printf(" @ range max   = %d\n",fit_range_max);
   printf(" @ infile      = %s\n",infile_path);
   printf(" @ outfile     = %s\n\n",outfile_path);
   fflush(stdout);
   
   if (arguments_check) return 1;
   return 0;
}

static int set_args_from_file(char* file_name) {
   
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
      if (     strcmp(tmp_c1,"FIT_Stopping_condition")==0)
         stp_cnd = atof(tmp_c2);
      else if (strcmp(tmp_c1,"FIT_Function_name"     )==0)
         fit_func_name = tmp_c2;
      else if (strcmp(tmp_c1,"FIT_Fit_range_min"     )==0)
         fit_range_min = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"FIT_Fit_range_max"     )==0)
         fit_range_max = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"FIT_Fit_data_file_name")==0)
         snprintf(infile_path,sizeof(infile_path),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"FIT_Output_file_name"  )==0)
         snprintf(outfile_path,sizeof(outfile_path),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"FIT_Parameter")==0     ) {
         char *tmp_tok;
         char  tmp_tmp_str[MAX_N_DATA][MAX_LEN_PATH];
         int   count_tmp = 0;
         tmp_tok = strtok(tmp_str[loop]," \t");
         for (int iii=0; iii<3; iii++) tmp_tok = strtok(NULL," \t");
         while (strcmp(tmp_tok,"}") != 0) {
            snprintf(tmp_tmp_str[count_tmp],sizeof(tmp_tmp_str[count_tmp])
                     ,"%s",tmp_tok);
            count_tmp++;
            tmp_tok = strtok(NULL," \t");
         }
         n_param = count_tmp;
         param_initial = new double[n_param];
         for (int n=0; n<n_param; n++){
            param_initial[n] = atof(tmp_tmp_str[n]);
         }
      }
      //***************************************************//
      else{
         printf("\n @@@@@@ invalid option \"%s\" in arguments file \"%s\"\n\n"
                , tmp_c1, file_name);
         return 1;
      }
   }
   return 0;
}
