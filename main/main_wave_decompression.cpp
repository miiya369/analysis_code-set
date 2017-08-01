//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBS wave function
 * @brief   Main part for de-compression of NBSwave data
 * @author  Takaya Miyamoto
 * @since   Wed Oct 12 03:36:32 JST 2016
 */
//--------------------------------------------------------------------------

#include <sys/stat.h> // To make directory

#include <NBSwave/NBSwave.h>

#define PROJECT WAVE_DECOMPRESSION    // <- Project name

static int time_slice_min, time_slice_max;

static CHANNEL_TYPE *channel_type = NULL;
static int           N_ch;

static char conf_list[MAX_LEN_PATH];
static char ipath    [MAX_LEN_PATH];
static char opath    [MAX_LEN_PATH];

static bool arguments_check = false;
static int  set_args(int, char**);
static int  set_args_from_file(char*);

static int  make_directory(int);

//========================================================================//
int main(int argc, char **argv)
{
   if (set_args(argc, argv) == 1) return 0;
   
   time_t start_time, end_time, timer;
   time(&start_time);
   
   NBS_WAVE_ORG NBSwave;
   
   for (int ich=0; ich<N_ch; ich++) {
      if (channel_type[ich].flg_spin_proj) {
         printf( "The channel '%s' is already decompressed.\n"
                , channel_type[ich].name.c_str() );
         continue;
      }
      for (int iconf=0; iconf<analysis::Nconf; iconf++) {
         for (int Tslice=time_slice_min; Tslice<=time_slice_max; Tslice++) {
            analysis::set_data_list(MAIN_PATH, "%s", ipath);
            NBSwave.set(channel_type[ich], Tslice, iconf); // Input
            
            if (make_directory(iconf) != 0) { // Make directory
               if (channel_type != NULL) delete [] channel_type;
               return -1;
            }
            analysis::set_data_list(MAIN_PATH, "%s", opath);
            NBSwave.output(channel_type[ich], Tslice, iconf); // Output
         }
      }
      time(&timer);
      printf(" @@@ channel = %s END || %s"
             , channel_type[ich].name.c_str(), ctime(&timer) );
      fflush(stdout);
   }

   if (channel_type != NULL) delete [] channel_type;
   
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
      if (argv[loop][0] == '-'){
         if (     strcmp(argv[loop],"-f"        )==0){}
         //****** You may set additional potion in here ******//
         else if (strcmp(argv[loop],"-ipath" )==0)
            snprintf(ipath,sizeof(ipath),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-opath" )==0)
            snprintf(opath,sizeof(opath),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-channel")==0)
         {
            int count_tmp = 0;
            do
            {
               if (loop+count_tmp == argc-1)
               {
                  printf("\n @@@@@@ Need \"@\" at the end of -channel option\n\n");
                  return 1;
               }
               count_tmp++;
            } while (strcmp(argv[loop+count_tmp],"@") != 0);
            N_ch = count_tmp-1;
            if (channel_type != NULL) delete [] channel_type;
            channel_type = new CHANNEL_TYPE[N_ch];
            for (int n=0; n<N_ch; n++)
            {
               loop++;
               channel_type[n].set(argv[loop]);
            }
         }
         else if (strcmp(argv[loop],"-check" )==0)
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
   analysis::Nconf = analysis::set_data_list(conf_list);
   
   printf("\n @ Arguments set :\n");
   printf(" @ conf list  = %s\n",conf_list);
   printf(" @ #. conf    = %d\n",analysis::Nconf);
   printf(" @ time  size = %d\n",analysis::tSIZE);
   printf(" @ space size = %d\n",analysis::xSIZE);
   printf(" @ input  dir = %s\n",ipath);
   printf(" @ #. channel = %d\n @ channel    = { ",N_ch);
   for(int loop=0; loop<N_ch; loop++)
      printf("%s ",channel_type[loop].name.c_str());
   printf("}\n @ t shift    = %s\n",analysis::data_list[N_T_SHIFT]);
   printf(" @ x shift    = %s\n",analysis::data_list[N_X_SHIFT]);
   printf(" @ y shift    = %s\n",analysis::data_list[N_Y_SHIFT]);
   printf(" @ z shift    = %s\n",analysis::data_list[N_Z_SHIFT]);
   printf(" @ snk rela   = %s\n",analysis::data_list[SNK_RELA]);
   printf(" @ src rela   = %s\n",analysis::data_list[SRC_RELA]);
   printf(" @ output dir = %s\n",opath);
   printf(" @ time_min   = %d\n",time_slice_min);
   printf(" @ time_max   = %d\n\n",time_slice_max);
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
      if (     strcmp(tmp_c1,"DECO_Size_of_space"     )==0){
         analysis::xSIZE = atoi(tmp_c2);
         analysis::ySIZE = atoi(tmp_c2);
         analysis::zSIZE = atoi(tmp_c2);
      }
      else if (strcmp(tmp_c1,"DECO_Size_of_time"      )==0)
         analysis::tSIZE = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"DECO_Min_time_slice"    )==0)
         time_slice_min = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"DECO_Max_time_slice"    )==0)
         time_slice_max = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"DECO_Gauge_confs_list"  )==0)
         snprintf(conf_list,sizeof(conf_list),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"DECO_Path_to_input_dir")==0)
         snprintf(ipath,sizeof(ipath),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"DECO_Path_to_output_dir")==0)
         snprintf(opath,sizeof(opath),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"DECO_T_shift"           )==0)
         analysis::set_data_list(N_T_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"DECO_X_shift"           )==0)
         analysis::set_data_list(N_X_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"DECO_Y_shift"           )==0)
         analysis::set_data_list(N_Y_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"DECO_Z_shift"           )==0)
         analysis::set_data_list(N_Z_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"DECO_Snk_relativistic"  )==0)
         analysis::set_data_list(SNK_RELA, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"DECO_Src_relativistic"  )==0)
         analysis::set_data_list(SRC_RELA, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"DECO_Calc_channel_name"  )==0)
      {
         char *tmp_tok;
         char  tmp_tmp_str[MAX_N_DATA][MAX_LEN_PATH];
         int   count_tmp = 0;
         tmp_tok = strtok(tmp_str[loop]," \t");
         for (int iii=0; iii<3; iii++) tmp_tok = strtok(NULL," \t");
         while (strcmp(tmp_tok,"}") != 0)
         {
            snprintf(tmp_tmp_str[count_tmp],sizeof(tmp_tmp_str[count_tmp])
                     ,"%s",tmp_tok);
            count_tmp++;
            tmp_tok = strtok(NULL," \t");
         }
         N_ch         = count_tmp;
         channel_type = new CHANNEL_TYPE[N_ch];
         for (int n=0; n<N_ch; n++)
         {
            channel_type[n].set(tmp_tmp_str[n]);
         }
      }
      //***************************************************//
      else
      {
         printf("\n @@@@@@ invalid option \"%s\" in arguments file \"%s\"\n\n"
                , tmp_c1, file_name);
         return 1;
      }
   }
   return 0;
}

static int make_directory(int iconf)
{
   char dir_tmp[1024];
   snprintf(  dir_tmp, sizeof(dir_tmp), "%s/%s", opath
            , analysis::data_list[NBS_DIRECTORY] );
   
   mkdir(dir_tmp, 0755);
   
   snprintf(  dir_tmp, sizeof(dir_tmp), "%s/%s/%s", opath
            , analysis::data_list[NBS_DIRECTORY]
            , analysis::data_list[iconf+MAX_PATH_ARG] );
   
   if(mkdir(dir_tmp, 0755) != 0)
   {
      printf("ERROR: Cannot create the directory, %s\n", dir_tmp);
      return -1;
   }
   return 0;
}
