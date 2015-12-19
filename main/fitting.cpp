//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Main part for fitting analysis
 * @author  Takaya Miyamoto
 * @since   Thu Jul 23 12:47:13 JST 2015
 */
//--------------------------------------------------------------------------

#include <fitting/fitting.h>

#define PROJECT FITTING_DATA   // <- Project name

int  analysis::Nconf;
int  analysis::xSIZE;
int  analysis::ySIZE;
int  analysis::zSIZE;
int  analysis::tSIZE;
char analysis::data_list[MAX_N_DATA][MAX_LEN_PATH];

int     n_param;
double* param_initial;
double  stp_cnd;
string  fit_func_name;
int     fit_range_min;
int     fit_range_max;

char infile_path[MAX_LEN_PATH];
char outfile_path[MAX_LEN_PATH];

bool arguments_check = false;
int  set_args(int, char**);
int  set_args_from_file(char*);

//========================================================================//
int main(int argc, char **argv) {
   if (set_args(argc, argv) == 1) return 0;
   
   time_t start_time, end_time;
   time( &start_time );
   
   FIT *fit = new FIT;
   
   fit->input_data    ( infile_path );
   fit->set_func      ( fit_func_name.c_str(), param_initial, n_param );
   fit->print_func_gnu( true );
   fit->fit_data_NR   ( fit_range_min, fit_range_max, stp_cnd );
   fit->print_param   ( );
   fit->print_func_gnu( false );
   fit->output_param  ( outfile_path );
   
   delete [] param_initial;
   delete fit;
   
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
