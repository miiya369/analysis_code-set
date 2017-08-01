//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBS wave function
 * @brief   Main part for the projection of wave function of Octet-Octet baryon
 * @author  Takaya Miyamoto
 * @since   Wed Oct 12 02:13:11 JST 2016
 */
//--------------------------------------------------------------------------

#include <NBSwave/NBSwave.h>
#include <sys/stat.h>

#define PROJECT PROJECTION_WAVE // <- Project name

static int time_min;
static int time_max;

static char conf_list[MAX_LEN_PATH];
static char outfile_path[MAX_LEN_PATH];

static bool proj_S1_Iso_flg = false;
static bool proj_S2_Iso_flg = false;
static bool proj_S3_Iso_flg = false;
static bool proj_S1_Flv_flg = false;
static bool proj_S2_Flv_flg = false;
static bool proj_S3_Flv_flg = false;

static bool arguments_check = false;
static int  set_args(int, char**);
static int  set_args_from_file(char*);
static void set_dir();

//========================================================================//
int main(int argc, char **argv)
{
   if (set_args(argc, argv) == 1) return 0;
   
   time_t start_time, end_time;
   time(&start_time);
   
   NBSwave::rot_matrix_init();
   
   NBS_WAVE_ORG *Wave_org = NULL;
   
   if (channel.flg_spin_proj) spin.set(channel.spin_name);
   else
      Wave_org = new NBS_WAVE_ORG;
   
   CONFIG<NBS_WAVE>     *Wave  = new CONFIG<NBS_WAVE>;
   
   CONFIG<CORRELATOR>   *Corr1 = NULL;
   CONFIG<CORRELATOR>   *Corr2 = NULL;
   CONFIG<R_CORRELATOR> *Rcorr = NULL;
   
   if (calc_flg_Rcorr) {
      Corr1 = new CONFIG<CORRELATOR>;
      Corr2 = new CONFIG<CORRELATOR>;
      Rcorr = new CONFIG<R_CORRELATOR>;
   }
   
   char outfile_name[1024];
   
   for (int it=time_min; it<=time_max; it++)
   {
      for (int conf=0; conf<analysis::Nconf; conf++) { // Read Data
	 if (channel.flg_spin_proj) (*Wave)(conf).set(channel, it, conf);
	 else {
	    Wave_org -> set(channel, it, conf);
	    NBSwave::spin_projection(*Wave_org, (*Wave)(conf), spin);
	 }
	 NBSwave::LP_projection((*Wave)(conf));
         
         if (calc_flg_Rcorr)
            if (it == time_min) {
               (*Corr1)(conf).set(channel.hadron1, conf, 0, "PS");
               (*Corr2)(conf).set(channel.hadron2, conf, 0, "PS");
            }
      } // conf
      
      if (take_JK_flg) Wave -> make_JK_sample();
      
      if (calc_flg_Rcorr) {
         if (it == time_min)
            if (take_JK_flg) {
               Corr1 -> make_JK_sample();
               Corr2 -> make_JK_sample();
            }
         for (int conf=0; conf<analysis::Nconf; conf++)
            (*Rcorr)(conf).set((*Wave)(conf),(*Corr1)(conf),(*Corr2)(conf), it);
         
	 snprintf(  outfile_name, sizeof(outfile_name)
		  , "%s/%s_Rcorrelator_%s_err_t%d"
		  , outfile_path, channel.name.c_str(), spin.name.c_str(), it );
         
         analysis::output_data_err(*Rcorr, outfile_name, use_JK_data);
      }
      
      snprintf(  outfile_name, sizeof(outfile_name)
	       , "%s/%s_NBSwave_%s_err_t%d"
               , outfile_path, channel.name.c_str(), spin.name.c_str(), it );
      
      analysis::output_data_err(*Wave, outfile_name, use_JK_data);
      
   } // it
   
   if (Wave_org != NULL) delete Wave_org;
   delete Wave;
   
   if (calc_flg_Rcorr) {
      delete Corr1;
      delete Corr2;
      delete Rcorr;
   }
   
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
         if (     strcmp(argv[loop],"-f"        )==0){}
         //****** You may set additional potion in here ******//
         else if (strcmp(argv[loop],"-t_max"    )==0)
            time_max  = atoi(argv[loop+1]);
         else if (strcmp(argv[loop],"-conf_list" )==0)
            snprintf(conf_list,sizeof(conf_list),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-t_min"    )==0)
            time_min  = atoi(argv[loop+1]);
         else if (strcmp(argv[loop],"-channel"  )==0)
            channel.set(argv[loop+1]);
         else if (strcmp(argv[loop],"-spin")==0)
            spin.set(argv[loop+1]);
         else if (strcmp(argv[loop],"-take_JK" )==0)
            take_JK_flg = true;
         else if (strcmp(argv[loop],"-check"    )==0)
            arguments_check = true;
         //***************************************************//
         else {
            printf("\n @@@@@@ invalid option \"%s\"\n", argv[loop]);
            analysis::usage(PROJECT);
            return 1;
         }
      }
   }
   if (take_JK_flg) use_JK_data = true;

   analysis::Nconf = analysis::set_data_list(conf_list);
   
   printf("\n @ Arguments set :\n");
   printf(" @ conf list  = %s\n",conf_list);
   printf(" @ #. conf    = %d\n",analysis::Nconf);
   printf(" @ time size  = %d\n",analysis::tSIZE);
   printf(" @ space size = %d\n",analysis::xSIZE);
   printf(" @ infile     = %s\n",analysis::data_list[MAIN_PATH]);
   printf(" @ channel    = %s\n",channel.name.c_str());
   printf(" @ t shift    = %s\n",analysis::data_list[N_T_SHIFT]);
   printf(" @ x shift    = %s\n",analysis::data_list[N_X_SHIFT]);
   printf(" @ y shift    = %s\n",analysis::data_list[N_Y_SHIFT]);
   printf(" @ z shift    = %s\n",analysis::data_list[N_Z_SHIFT]);
   printf(" @ snk rela   = %s\n",analysis::data_list[SNK_RELA]);
   printf(" @ src rela   = %s\n",analysis::data_list[SRC_RELA]);
   printf(" @ outfile    = %s\n",outfile_path);
   printf(" @ time_min   = %d\n",time_min);
   printf(" @ time_max   = %d\n",time_max);
   printf(" @ spin       = %s\n",spin.name.c_str());
   printf(" @ use JK data= %s\n",analysis::bool_to_str(use_JK_data).c_str());
   printf(" @ take JK    = %s\n",analysis::bool_to_str(take_JK_flg).c_str());
   printf(" @ out Rcorr  = %s\n\n",analysis::bool_to_str(calc_flg_Rcorr).c_str());
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
      if (     strcmp(tmp_c1,"WAVE_Size_of_space"     )==0){
         analysis::xSIZE = atoi(tmp_c2);
         analysis::ySIZE = atoi(tmp_c2);
         analysis::zSIZE = atoi(tmp_c2);
      }
      else if (strcmp(tmp_c1,"WAVE_Min_time_slice"    )==0)
         time_min = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"WAVE_Max_time_slice"    )==0)
         time_max = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"WAVE_Channel_name"      )==0)
         channel.set(tmp_c2);
      else if (strcmp(tmp_c1,"WAVE_Spin_projection"   )==0)
         spin.set(tmp_c2);
      else if (strcmp(tmp_c1,"WAVE_Gauge_confs_list"  )==0)
         snprintf(conf_list,sizeof(conf_list),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"WAVE_Path_to_output_dir")==0)
         snprintf(outfile_path,sizeof(outfile_path),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"WAVE_Path_to_input_dir" )==0)
         analysis::set_data_list(MAIN_PATH, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"WAVE_T_shift"           )==0)
         analysis::set_data_list(N_T_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"WAVE_X_shift"           )==0)
         analysis::set_data_list(N_X_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"WAVE_Y_shift"           )==0)
         analysis::set_data_list(N_Y_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"WAVE_Z_shift"           )==0)
         analysis::set_data_list(N_Z_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"WAVE_Size_of_time"      )==0)
         analysis::tSIZE = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"WAVE_Snk_relativistic"  )==0)
         analysis::set_data_list(SNK_RELA, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"WAVE_Src_relativistic"  )==0)
         analysis::set_data_list(SRC_RELA, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"WAVE_Take_jack_knife"    )==0)
         take_JK_flg = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"WAVE_Use_jack_knife_data")==0)
         use_JK_data = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"WAVE_Out_R_correlator"  )==0)
         calc_flg_Rcorr = analysis::str_to_bool(tmp_c2);
      //***************************************************//
      else{
         printf("\n @@@@@@ invalid option \"%s\" in arguments file \"%s\"\n\n"
                , tmp_c1, file_name);
         return 1;
      }
   }
   return 0;
}

void set_dir(const char* Dir_num)
{
   char dir_name[2048];
   char conf_dir_name[2048];
   
   snprintf(  dir_name, sizeof(dir_name), "%s/BBwave.prj.S%s"
	    , outfile_path, Dir_num );
   mkdir(dir_name, 0755);
   
   for (int conf_loop=0; conf_loop<analysis::Nconf; conf_loop++) {
      snprintf(  conf_dir_name, sizeof(conf_dir_name), "%s/%s"
	       , dir_name, analysis::data_list[conf_loop+MAX_PATH_ARG] );
      mkdir(conf_dir_name, 0755);
   }
}
