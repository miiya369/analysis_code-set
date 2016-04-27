//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup correlator
 * @brief   Main part for dispersion relation calculation
 * @author  Takaya Miyamoto
 * @since   Tue Apr 26 21:15:13 JST 2016
 */
//--------------------------------------------------------------------------

#include <fitting/fitting.h>
#include <correlator/correlator.h>

#define PROJECT DISPERSION_RELATION   // <- Project name

static int           Nhad;
static HADRON_TYPE  *hadron_type  = NULL;
static int           fit_range_min;
static int           fit_range_max;

static char conf_list[MAX_LEN_PATH];
static char outfile_path[MAX_LEN_PATH];

static bool take_JK_flg    = false;
static bool use_JK_data    = false;
static bool input_fit_flg  = false;

static bool arguments_check = false;
static int  set_args(int, char**);
static int  set_args_from_file(char*);

//========================================================================//
int main(int argc, char **argv)
{
   if (set_args(argc, argv) == 1) return 0;
   
   time_t start_time, end_time;
   time( &start_time );
   
   if (take_JK_flg) use_JK_data = true;

   CONFIG<CORRELATOR> *Corr     = new CONFIG<CORRELATOR>[5];
   CONFIG<CORRELATOR> *eff_mass = new CONFIG<CORRELATOR>[5];
   CONFIG<CORRELATOR> *Disp     = new CONFIG<CORRELATOR>[5];
   
   char    outfile_name[1024];
   char    infile_name [1024];
   double *data         = new double[analysis::tSIZE];
   double *cood         = new double[analysis::tSIZE];
   double *err          = new double[analysis::tSIZE];
   double *param        = new double[analysis::Nconf];
   
   double data_mom[5], err_mom[5];
   
   double *data_all = NULL;
   if (input_fit_flg) data_all = new double[analysis::tSIZE * analysis::Nconf];
   
   for (int it=0; it<analysis::tSIZE; it++) cood[it] = it;
   
   for (int loop=0; loop<Nhad; loop++)
   {
      for (int mom=0; mom<=4; mom++)
      {
         if (!input_fit_flg)
         {
            for (int conf=0; conf<analysis::Nconf; conf++)
               Corr[mom](conf).set(hadron_type[loop], conf, mom, "PS");
         
            if (take_JK_flg) Corr[mom].make_JK_sample();
         }
         else
         {
            snprintf(  infile_name, sizeof(infile_name)
                     , "%s/%s_correlator_mom%d_fit"
                     , analysis::data_list[MAIN_PATH]
                     , hadron_type[loop].name.c_str(), mom );
            
            int tmp_Ndata, tmp_Nconf;
            fitting::input_data_binary(infile_name, tmp_Nconf, tmp_Ndata);
            
            if (tmp_Nconf != analysis::Nconf || tmp_Ndata != analysis::tSIZE)
            {
               printf(" @@@ ERROR : Different Nconf or Ndata\n"
                      "           : Nconf in binary = %d\n"
                      "           : Ndata in binary = %d\n\n"
                      , tmp_Nconf, tmp_Ndata);
               return -1;
            }
            fitting::input_data_binary(infile_name, cood, data_all, err);
            
            for (int conf=0; conf<analysis::Nconf; conf++)
            {
               Corr[mom](conf).mem_alloc();
               
               for (int it=0; it<analysis::tSIZE; it++)
                  Corr[mom](conf)(it) = data_all[conf+analysis::Nconf*it];
            }
            if (take_JK_flg) Corr[mom].make_JK_sample();
         }
         
         for (int conf=0; conf<analysis::Nconf; conf++)
         {
            eff_mass[mom](conf).mem_alloc(analysis::tSIZE-1);
            
            for (int it=0; it<analysis::tSIZE-1; it++)
               eff_mass[mom](conf)(it) = -log(  Corr[mom](conf)(it+1)
                                              / Corr[mom](conf)(it) );
         }
      }
      
      for (int mom=0; mom<=4; mom++)
      {
         for (int conf=0; conf<analysis::Nconf; conf++)
         {
            Disp[mom](conf).mem_alloc(analysis::tSIZE-1);
            
            for (int it=0; it<analysis::tSIZE-1; it++)
               Disp[mom](conf)(it) = (  pow(eff_mass[mom](conf)(it), 2)
                                      - pow(eff_mass[0]  (conf)(it), 2) );
         }
         Disp[mom].make_mean_err(data, err, use_JK_data);
         
         for (int conf=0; conf<analysis::Nconf; conf++)
         {
            double nume = 0.0, deno = 0.0, tmp_d;
            for (int it=fit_range_min; it<=fit_range_max; it++)
            {
               tmp_d = pow(err[it], 2);
               nume += Disp[mom](conf)(it).real() / tmp_d;
               deno +=             1.0            / tmp_d;
            }
            param[conf] = nume/deno;
         }
         analysis::make_mean_err(  param, data_mom[mom], err_mom[mom]
                                 , analysis::Nconf, use_JK_data );
      }
      
//      FILE *fp;
//      fp = fopen("results/test", "w");   // For Debug !
//      for (int mom=0; mom<=4; mom++)
//      {
//         int conf = 6;
//         for (int it=0; it<analysis::tSIZE-1; it++)
//            fprintf(fp, "%d %e\n", it, Disp[mom](conf)(it).real());
//      }
//      fclose(fp);
      
      snprintf(  outfile_name, sizeof(outfile_name)
               , "%s/%s_dispersion_relation"
               , outfile_path, hadron_type[loop].name.c_str());
      
      ofstream ofs(outfile_name);
      ofs << "0.0 0.0 0.0" << endl;
      
      for (int mom=1; mom<=4; mom++)
         ofs << mom * pow(2*PI/analysis::xSIZE, 2) << " " << data_mom[mom]
         << " " << err_mom[mom] << endl;
      ofs.close();
      
   } // Nhad
   
   delete [] data;
   delete [] cood;
   delete [] err;
   delete [] param;
   
   if (input_fit_flg) delete [] data_all;

   delete [] Corr;
   delete [] eff_mass;
   delete [] Disp;

   if (hadron_type  != NULL) delete [] hadron_type;
   
   time( &end_time );
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
         if (     strcmp(argv[loop],"-f"     )==0){}
         //****** You may set additional potion in here ******//
         else if (strcmp(argv[loop],"-idir"  )==0)
            analysis::set_data_list(MAIN_PATH, "%s", argv[loop+1]);
         else if (strcmp(argv[loop],"-conf_list" )==0)
            snprintf(conf_list,sizeof(conf_list),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-odir"  )==0)
            snprintf(outfile_path,sizeof(outfile_path),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-t_min" )==0)
            fit_range_min = atoi(argv[loop+1]);
         else if (strcmp(argv[loop],"-t_max" )==0)
            fit_range_max = atoi(argv[loop+1]);
         else if (strcmp(argv[loop],"-src_t" )==0)
            analysis::set_data_list(N_T_SHIFT, "%s", argv[loop+1]);
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
         else if (strcmp(argv[loop],"-take_JK" )==0)
            take_JK_flg = true;
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
   printf(" @ s_size     = %d\n",analysis::xSIZE);
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
   printf(" @ input dir  = %s\n",analysis::data_list[MAIN_PATH]);
   printf(" @ output dir = %s\n @\n",outfile_path);
   printf(" @ input  bin = %s\n",analysis::bool_to_str(input_fit_flg).c_str());
   printf(" @ use JK data= %s\n",analysis::bool_to_str(use_JK_data).c_str());
   printf(" @ take JK    = %s\n",analysis::bool_to_str(take_JK_flg).c_str());
   printf(" @ range min  = %d\n",fit_range_min);
   printf(" @ range max  = %d\n",fit_range_max);
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
      if (     strcmp(tmp_c1,"DISP_Gauge_confs_list"  )==0)
         snprintf(conf_list,sizeof(conf_list),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"DISP_Path_to_input_dir" )==0)
         analysis::set_data_list(MAIN_PATH, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"DISP_Path_to_output_dir")==0)
         snprintf(outfile_path,sizeof(outfile_path),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"DISP_Size_of_time"      )==0)
         analysis::tSIZE = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"DISP_Size_of_space"     )==0)
         analysis::xSIZE = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"DISP_T_shift"           )==0)
         analysis::set_data_list(N_T_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"DISP_X_shift"           )==0)
         analysis::set_data_list(N_X_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"DISP_Y_shift"           )==0)
         analysis::set_data_list(N_Y_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"DISP_Z_shift"           )==0)
         analysis::set_data_list(N_Z_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"DISP_Calc_hadron_name"  )==0) {
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
      else if (strcmp(tmp_c1,"DISP_Snk_relativistic"  )==0)
         analysis::set_data_list(SNK_RELA, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"DISP_Src_relativistic"  )==0)
         analysis::set_data_list(SRC_RELA, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"DISP_Input_fit_data"    )==0)
         input_fit_flg = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"DISP_Take_jack_knife"    )==0)
         take_JK_flg = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"DISP_Use_jack_knife_data")==0)
         use_JK_data = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"DISP_Fit_range_min"     )==0)
         fit_range_min = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"DISP_Fit_range_max"     )==0)
         fit_range_max = atoi(tmp_c2);
      //***************************************************//
      else{
         printf("\n @@@@@@ invalid option \"%s\" in arguments file \"%s\"\n\n"
                , tmp_c1, file_name);
         return 1;
      }
   }
   return 0;
}
