//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup correlator
 * @brief   Main part for chake the bin size dependence of Jack Knife sample
 * @author  Takaya Miyamoto
 * @since   Fri Dec 11 03:42:56 JST 2015
 */
//--------------------------------------------------------------------------

#include <correlator/correlator.h>

#define PROJECT JACK_KNIFE_BIN_CHECK    // <- Project name

static HADRON_TYPE hadron_type;

static int  t_min, t_max;

static char conf_list[MAX_LEN_PATH];
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

   CONFIG<CORRELATOR> *Corr = new CONFIG<CORRELATOR>;
   for (int conf=0; conf<analysis::Nconf; conf++)
      (*Corr)(conf).set( hadron_type, conf, 0, "PS" );

   cdouble *mean = new cdouble[t_max-t_min+1];
   cdouble *err  = new cdouble[t_max-t_min+1];
   
   cdouble  mean_err, err_err;
   
   ofstream ofs(outfile_path, ios::out);
   
   for (int bin_size = 1; bin_size < analysis::Nconf; bin_size++) {
      if (analysis::Nconf % bin_size != 0) continue;
      
      int N_bin = analysis::Nconf / bin_size;
      
      CONFIG<CORRELATOR> Corr_bin(N_bin);
      Corr->make_JK_sample(Corr_bin);
      
      CONFIG<CORRELATOR> eff_mass(N_bin);
      for (int conf=0; conf<N_bin; conf++) {
         eff_mass(conf).mem_alloc(t_max-t_min+1);
         for (int it=t_min; it<=t_max; it++)
            eff_mass(conf)(it-t_min) = -log(  Corr_bin(conf)(it+1)
                                            / Corr_bin(conf)(it)  );
      }
      
      eff_mass.make_mean_err( mean, err, true );
      analysis::make_mean_err( err, mean_err, err_err, t_max-t_min+1, false );
      ofs << bin_size << " " << mean_err.real() << " " << err_err.real()
                      << " " << mean_err.imag() << " " << err_err.imag() << endl;
   }
   ofs.close();
   
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
         if (     strcmp(argv[loop],"-f"     )==0){}
         //****** You may set additional potion in here ******//
	 else if (strcmp(argv[loop],"-conf_list" )==0)
	   snprintf(conf_list,sizeof(conf_list),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-idir"  )==0)
            analysis::set_data_list(MAIN_PATH, "%s", argv[loop+1]);
         else if (strcmp(argv[loop],"-ofile"  )==0)
            snprintf(outfile_path,sizeof(outfile_path),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-hadron")==0)
            hadron_type.set(argv[loop+1]);
         else if (strcmp(argv[loop],"-t_min")==0)
            t_min = atoi(argv[loop+1]);
         else if (strcmp(argv[loop],"-t_max")==0)
            t_max = atoi(argv[loop+1]);
         else if (strcmp(argv[loop],"-src_t" )==0)
            analysis::set_data_list(N_T_SHIFT, "%s", argv[loop+1]);
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
   printf(" @ conf list  = %s\n",conf_list);
   printf(" @ #. conf    = %d\n",analysis::Nconf);
   printf(" @ t_size     = %d\n",analysis::tSIZE);
   printf(" @ input dir  = %s\n",analysis::data_list[MAIN_PATH]);
   printf(" @ t shift    = %s\n",analysis::data_list[N_T_SHIFT]);
   printf(" @ x shift    = %s\n",analysis::data_list[N_X_SHIFT]);
   printf(" @ y shift    = %s\n",analysis::data_list[N_Y_SHIFT]);
   printf(" @ z shift    = %s\n",analysis::data_list[N_Z_SHIFT]);
   printf(" @ snk rela   = %s\n",analysis::data_list[SNK_RELA]);
   printf(" @ src rela   = %s\n",analysis::data_list[SRC_RELA]);
   printf(" @ outfile    = %s\n",outfile_path);
   printf(" @ hadron     = %s\n",hadron_type.name_only.c_str());
   printf(" @ min t      = %d\n",t_min);
   printf(" @ max t      = %d\n\n",t_max);
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
      if (     strcmp(tmp_c1,"JK_Gauge_confs_list"   )==0)
         snprintf(conf_list,sizeof(conf_list),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"JK_Path_to_input_dir"  )==0)
         analysis::set_data_list(MAIN_PATH, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"JK_Path_to_output_file")==0)
         snprintf(outfile_path,sizeof(outfile_path),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"JK_Size_of_time"       )==0)
         analysis::tSIZE = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"JK_T_shift"            )==0)
         analysis::set_data_list(N_T_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"JK_X_shift"            )==0)
         analysis::set_data_list(N_X_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"JK_Y_shift"            )==0)
         analysis::set_data_list(N_Y_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"JK_Z_shift"            )==0)
         analysis::set_data_list(N_Z_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"JK_hadron_name"        )==0)
         hadron_type.set(tmp_c2);
      else if (strcmp(tmp_c1,"JK_time_min"           )==0)
         t_min = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"JK_time_max"           )==0)
         t_max = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"JK_Snk_relativistic"  )==0)
         analysis::set_data_list(SNK_RELA, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"JK_Src_relativistic"  )==0)
         analysis::set_data_list(SRC_RELA, "%s", tmp_c2);
      //***************************************************//
      else{
         printf("\n @@@@@@ invalid option \"%s\" in arguments file \"%s\"\n\n"
                , tmp_c1, file_name);
         return 1;
      }
   }
   return 0;
}
