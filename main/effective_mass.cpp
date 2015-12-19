//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup correlator
 * @brief   Main part for effective mass calculation
 * @author  Takaya Miyamoto
 * @since   Mon Sep  7 17:31:47 JST 2015
 */
//--------------------------------------------------------------------------

#include <fitting/fitting.h>
#include <correlator/correlator.h>

#define PROJECT EFFECTIVEMASS    // <- Project name

int  analysis::Nconf;
int  analysis::xSIZE;
int  analysis::ySIZE;
int  analysis::zSIZE;
int  analysis::tSIZE;

int          Nhad;
HADRON_TYPE *hadron_type = NULL;
int          fit_range_min;
int          fit_range_max;
double       lat_space;

char conf_list[MAX_LEN_PATH];
char outfile_path[MAX_LEN_PATH];

bool calc_flg_fit = false;

bool arguments_check = false;
int  set_args(int, char**);
int  set_args_from_file(char*);

//========================================================================//
int main(int argc, char **argv) {
   if (set_args(argc, argv) == 1) return 0;
   
   time_t start_time, end_time;
   time( &start_time );

   CONFIG<CORRELATOR> *Corr     = new CONFIG<CORRELATOR>;
   CONFIG<CORRELATOR> *eff_mass = new CONFIG<CORRELATOR>;
   CONFIG<FIT>        *fit      = new CONFIG<FIT>;
   
   char    outfile_name[1024];
   double  param_ini[2] = { 0.001, 1.0 };   // <- init. value of effmass fit
   double *data         = new double[analysis::tSIZE];
   double *cood         = new double[analysis::tSIZE];
   double *err          = new double[analysis::tSIZE];
   double *chisq        = new double[analysis::Nconf];
   double *param        = new double[analysis::Nconf];
   double *mass         = new double[Nhad*4];
   
   for (int it=0; it<analysis::tSIZE; it++) cood[it] = it;
   
   for (int loop=0; loop<Nhad; loop++) {
      
      for (int conf=0; conf<analysis::Nconf; conf++)
         (*Corr)(conf).set( hadron_type[loop], conf, "PS" );
      
      Corr->make_JK_sample();
      
      for (int conf=0; conf<analysis::Nconf; conf++) {
         (*eff_mass)(conf).mem_alloc();
         for (int it=0; it<analysis::tSIZE-1; it++)
            (*eff_mass)(conf)(it) = -log(  (*Corr)(conf)(it+1)
                                         / (*Corr)(conf)(it) );
      }
      snprintf(  outfile_name, sizeof(outfile_name)
               , "%s/%s_effmass_err"
               , outfile_path, hadron_type[loop].name.c_str() );
      
      analysis::output_data_err( *eff_mass, outfile_name );
      
      if (calc_flg_fit) {
         Corr->make_mean_err( data, err, true );
         for (int conf=0; conf<analysis::Nconf; conf++) {
            for (int it=0; it<analysis::tSIZE; it++)
               data[it] = (*Corr)(conf)(it).real();
            
            (*fit)(conf).set_func( "1EXP", param_ini );
            chisq[conf] =
             (*fit)(conf).fit_data_NR(  cood, data, err
                                      , fit_range_min, fit_range_max, 0.000001 );
            param[conf] = (*fit)(conf)(1);
         }
         analysis::make_mean_err( param, mass[0+4*loop], mass[1+4*loop], true  );
         analysis::make_mean_err( chisq, mass[2+4*loop], mass[3+4*loop], false );
      }
      
   } // Nhad
   
   delete [] data;
   delete [] cood;
   delete [] err;
   delete [] chisq;
   delete [] param;

   delete fit;
   delete Corr;
   delete eff_mass;
   
   if (calc_flg_fit) {
      printf("\n @ =================================================== @\n");
      printf(" @           EFFECTIVE MASS FITTING RESULTS            @\n");
      printf(" @ =================================================== @\n\n");
      for (int loop=0; loop<Nhad; loop++)
         printf(" %9s : [Lattice Unit] %lf +/- %lf\n"
                "           : [   MeV/c2   ] %lf +/- %lf\n"
                "           : [ chisq/conf ] %lf +/- %lf\n\n"
                , hadron_type[loop].name_only.c_str()
                , mass[0+4*loop], mass[1+4*loop]
                , mass[0+4*loop]*197.327/lat_space
                , mass[1+4*loop]*197.327/lat_space
                , mass[2+4*loop], mass[3+4*loop] );
      printf(" @ =================================================== @\n");
   }
   delete [] hadron_type;
   delete [] mass;
   
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
         if (     strcmp(argv[loop],"-f"     )==0){}
         //****** You may set additional potion in here ******//
         else if (strcmp(argv[loop],"-ifile" )==0)
            snprintf(         analysis::data_list[MAIN_PATH]
                     , sizeof(analysis::data_list[MAIN_PATH])
                     , "%s", argv[loop+1]);
         else if (strcmp(argv[loop],"-ofile" )==0)
            snprintf(outfile_path,sizeof(outfile_path),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-t_min" )==0)
            fit_range_min = atoi(argv[loop+1]);
         else if (strcmp(argv[loop],"-t_max" )==0)
            fit_range_max = atoi(argv[loop+1]);
         else if (strcmp(argv[loop],"-fit"   )==0)
            calc_flg_fit = true;
         else if (strcmp(argv[loop],"-hadron")==0) {
            int count_tmp = 0;
            do {
               if (loop+count_tmp == argc-1) {
                  printf("\n @@@@@@ Need \"@\" at the end of -hadron option\n\n");
                  return 1;
               }
               count_tmp++;
            } while (strcmp(argv[loop+count_tmp],"@") != 0);
            Nhad = count_tmp-1;
            if (hadron_type != NULL) delete [] hadron_type;
            hadron_type = new HADRON_TYPE[Nhad];
            for (int n=0; n<Nhad; n++){
               loop++;
               hadron_type[n].set(argv[loop]);
            }
         }
         else if (strcmp(argv[loop],"-check" )==0)
            arguments_check = true;
         //***************************************************//
         else {
            printf("\n @@@@@@ invalid option \"%s\"\n", argv[loop]);
            analysis::usage(PROJECT);
            return 1;
         }
      }
   }
   analysis::Nconf = analysis::set_data_list(conf_list);
   
   printf("\n @ Arguments set :\n");
   printf(" @ #. conf    = %d\n",analysis::Nconf);
   printf(" @ t_size     = %d\n",analysis::tSIZE);
   printf(" @ t shift    = %s\n",analysis::data_list[N_T_SHIFT]);
   printf(" @ x shift    = %s\n",analysis::data_list[N_X_SHIFT]);
   printf(" @ y shift    = %s\n",analysis::data_list[N_Y_SHIFT]);
   printf(" @ z shift    = %s\n",analysis::data_list[N_Z_SHIFT]);
   printf(" @ snk rela   = %s\n",analysis::data_list[SNK_RELA]);
   printf(" @ src rela   = %s\n",analysis::data_list[SRC_RELA]);
   printf(" @ #. hadron  = %d\n @ hadron     = { ",Nhad);
   for(int loop=0; loop<Nhad; loop++)
      printf("%s ",hadron_type[loop].name_only.c_str());
   printf("}\n @ conf list  = %s\n",conf_list);
   printf(" @ infile     = %s\n",analysis::data_list[MAIN_PATH]);
   printf(" @ outfile    = %s\n @\n",outfile_path);
   printf(" @ calc fit   = %s\n",analysis::bool_to_str(calc_flg_fit).c_str());
   printf(" @ range min  = %d\n",fit_range_min);
   printf(" @ range max  = %d\n",fit_range_max);
   printf(" @ lat space  = %lf\n\n",lat_space);
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
      if (     strcmp(tmp_c1,"MAS_Gauge_confs_list"  )==0)
         snprintf(conf_list,sizeof(conf_list),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Path_to_input_dir" )==0)
         snprintf(         analysis::data_list[MAIN_PATH]
                  , sizeof(analysis::data_list[MAIN_PATH])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Path_to_output_dir")==0)
         snprintf(outfile_path,sizeof(outfile_path),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Size_of_time"      )==0)
         analysis::tSIZE = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"MAS_T_shift"           )==0)
         snprintf(         analysis::data_list[N_T_SHIFT]
                  , sizeof(analysis::data_list[N_T_SHIFT])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"MAS_X_shift"           )==0)
         snprintf(         analysis::data_list[N_X_SHIFT]
                  , sizeof(analysis::data_list[N_X_SHIFT])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Y_shift"           )==0)
         snprintf(         analysis::data_list[N_Y_SHIFT]
                  , sizeof(analysis::data_list[N_Y_SHIFT])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Z_shift"           )==0)
         snprintf(         analysis::data_list[N_Z_SHIFT]
                  , sizeof(analysis::data_list[N_Z_SHIFT])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Calc_hadron_name"  )==0     ) {
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
         Nhad        = count_tmp;
         hadron_type = new HADRON_TYPE[Nhad];
         for (int n=0; n<Nhad; n++){
            hadron_type[n].set(tmp_tmp_str[n]);
         }
      }
      else if (strcmp(tmp_c1,"MAS_Snk_relativistic"  )==0)
         snprintf(         analysis::data_list[SNK_RELA]
                  , sizeof(analysis::data_list[SNK_RELA])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Src_relativistic"  )==0)
         snprintf(         analysis::data_list[SRC_RELA]
                  , sizeof(analysis::data_list[SRC_RELA])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Calc_fit_data"     )==0)
         calc_flg_fit = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Fit_range_min"     )==0)
         fit_range_min = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Fit_range_max"     )==0)
         fit_range_max = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Lattice_spacing"   )==0)
         lat_space = atof(tmp_c2);
      //***************************************************//
      else{
         printf("\n @@@@@@ invalid option \"%s\" in arguments file \"%s\"\n\n"
                , tmp_c1, file_name);
         return 1;
      }
   }
   return 0;
}
