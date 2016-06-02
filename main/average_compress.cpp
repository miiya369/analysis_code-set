//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup correlator, NBS wave function
 * @brief   Main part for average of correlator & compress NBSwave data
 * @author  Takaya Miyamoto
 * @since   Sat May 28 13:11:41 JST 2016
 */
//--------------------------------------------------------------------------

#include <common/analysis.h>

#include <yukawa/PH1_4pt.h>
#include <yukawa/compress48.h>

#define PROJECT AVERAGE_COMPRESS    // <- Project name

using namespace yukawa;
typedef four_point_base::BC BC;
typedef PH1                 Core;
typedef four_point<Core>    Four_Point;
typedef compress48<Core>    Compress48;

static int time_slice_min, time_slice_max;

static string *Tshift = NULL;
static int     N_Tshift;

static HADRON_TYPE *hadron_type = NULL;
static int          N_had;

static CHANNEL_TYPE *channel_type = NULL;
static int           N_ch;

static char conf_list[MAX_LEN_PATH];
static char work_dir[MAX_LEN_PATH];

static bool arguments_check = false;
static int  set_args(int, char**);
static int  set_args_from_file(char*);

static void input_corr_all( string, int, string, double*);
static void output_corr_all(string, int, int, double*);

//========================================================================//
int main(int argc, char **argv) {
   if (set_args(argc, argv) == 1) return 0;
   
   time_t start_time, end_time, timer;
   time( &start_time );
   
   cubic_group::initialize();
   PH1::initialize();
   analysis::set_data_list(NBS_DIRECTORY, "BBwave.dir");
   string corr_dir[2] = {"PS", "SS"};
   
   for (int conf=0; conf<analysis::Nconf; conf++) {
      
      analysis::set_data_list(N_TIME, "-1");
      for (int ihad=0; ihad<N_had; ihad++) {
         
         analysis::set_data_list(  HADRON_NAME, "%s"
                                 , hadron_type[ihad].name.c_str());
         
         double *corr_o = new double[10*analysis::tSIZE];
         double *corr_i = new double[10*analysis::tSIZE];
         for (int corr_loop=0; corr_loop<2; corr_loop++) {
            for (int loop=0; loop<10*analysis::tSIZE; loop++) corr_o[loop] = 0.0;
            for (int it=0; it<N_Tshift; it++) {
               input_corr_all( corr_dir[corr_loop], conf, Tshift[it], corr_i );
               for (int loop=0; loop<10*analysis::tSIZE; loop++)
                  corr_o[loop] += (corr_i[loop]/(double)N_Tshift); // corr ave.
            } // it
            output_corr_all( corr_dir[corr_loop], conf, N_Tshift, corr_o );
         } // corr_loop
         
         if (hadron_type[ihad].fb_mean_flg) {   // for anti-hadron
            string anti_HAD_name;
            anti_HAD_name = "anti" + hadron_type[ihad].name;
            analysis::set_data_list(  HADRON_NAME, "%s", anti_HAD_name.c_str());
            for (int corr_loop=0; corr_loop<2; corr_loop++) {
               for (int loop=0; loop<10*analysis::tSIZE; loop++)
                  corr_o[loop] = 0.0;
               for (int it=0; it<N_Tshift; it++) {
                  input_corr_all( corr_dir[corr_loop], conf, Tshift[it], corr_i );
                  for (int loop=0; loop<10*analysis::tSIZE; loop++)
                     corr_o[loop] += (corr_i[loop]/(double)N_Tshift); // corr ave.
               } // it
               output_corr_all( corr_dir[corr_loop], conf, N_Tshift, corr_o );
            } // corr_loop
         }
         delete [] corr_o;
         delete [] corr_i;
         
      } // ihad
      
      for (int ich=0; ich<N_ch; ich++) {
         
         analysis::set_data_list(  N_TIME     , "-1");
         analysis::set_data_list(  HADRON_NAME, "%s"
                                 , channel_type[ich].prop_name.c_str());
         
         double *corr_o = new double[10*analysis::tSIZE/2];
         double *corr_i = new double[10*analysis::tSIZE/2];
         for (int loop=0; loop<10*analysis::tSIZE/2; loop++) corr_o[loop] = 0.0;
         for (int it=0; it<N_Tshift; it++) {
            input_corr_all( "multi", conf, Tshift[it], corr_i );
            for (int loop=0; loop<10*analysis::tSIZE/2; loop++)
               corr_o[loop] += (corr_i[loop]/(double)N_Tshift); // corr ave.
         } // it
         output_corr_all( "multi", conf, N_Tshift, corr_o );
         delete [] corr_o;
         delete [] corr_i;
         
         analysis::set_data_list(  N_CHANNEL, "%s"
                                 , channel_type[ich].number.c_str());
         
         for (int Tslice=time_slice_min; Tslice<=time_slice_max; Tslice++) {
            analysis::set_data_list(  N_TIME, "%03d", Tslice);
            analysis::set_data_list(  MAIN_PATH, "%s/Tshift%s"
                                    , work_dir, Tshift[0].c_str());
            analysis::set_data_list(  N_T_SHIFT, "%s", Tshift[0].c_str());
            
            mapping48  map;    map.read( analysis::set_path(conf).c_str());
            Compress48 comp;   comp.read(analysis::set_path(conf).c_str(), map);
            Compress48 tmp;
            for (int it=1; it<N_Tshift; it++) {
               analysis::set_data_list(  MAIN_PATH, "%s/Tshift%s"
                                       , work_dir, Tshift[it].c_str());
               analysis::set_data_list(  N_T_SHIFT, "%s", Tshift[it].c_str());
               tmp.read(analysis::set_path(conf).c_str(), map);
               comp += tmp; // NBSwave ave.
            } // it
            analysis::set_data_list(  MAIN_PATH, "%s/TshiftAve/%dsrc"
                                    , work_dir, N_Tshift);
            analysis::set_data_list(N_T_SHIFT, "A%02d", N_Tshift);
            comp /= double(N_Tshift);
            comp.write(analysis::set_path(conf).c_str());
            
         } // Tslice

      } // ich
      
      time(&timer);
      printf(" @@@ conf=%d END || %s", conf, ctime(&timer));
      fflush(stdout);
   } // conf

   if (Tshift       != NULL) delete [] Tshift;
   if (hadron_type  != NULL) delete [] hadron_type;
   if (channel_type != NULL) delete [] channel_type;
   
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
         else if (strcmp(argv[loop],"-wdir" )==0)
            snprintf(work_dir,sizeof(work_dir),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-t_shift"   )==0) {
            int count_tmp = 0;
            do {
               if (loop+count_tmp == argc-1) {
                  printf("\n @@@@@@ Need \"@\" at the end of -t_shift option\n\n");
                  return 1;
               }
               count_tmp++;
            } while (strcmp(argv[loop+count_tmp],"@") != 0);
            N_Tshift = count_tmp-1;
            if (Tshift != NULL) delete [] Tshift;
            Tshift = new string[N_Tshift];
            for (int n=0; n<N_Tshift; n++){
               loop++;
               Tshift[n] = argv[loop];
            }
         }
         else if (strcmp(argv[loop],"-hadron")==0) {
            int count_tmp = 0;
            do {
               if (loop+count_tmp == argc-1) {
                  printf("\n @@@@@@ Need \"@\" at the end of -hadron option\n\n");
                  return 1;
               }
               count_tmp++;
            } while (strcmp(argv[loop+count_tmp],"@") != 0);
            N_had = count_tmp-1;
            if (hadron_type != NULL) delete [] hadron_type;
            hadron_type = new HADRON_TYPE[N_had];
            for (int n=0; n<N_had; n++){
               loop++;
               hadron_type[n].set(argv[loop]);
            }
         }
         else if (strcmp(argv[loop],"-channel")==0) {
            int count_tmp = 0;
            do {
               if (loop+count_tmp == argc-1) {
                  printf("\n @@@@@@ Need \"@\" at the end of -channel option\n\n");
                  return 1;
               }
               count_tmp++;
            } while (strcmp(argv[loop+count_tmp],"@") != 0);
            N_ch = count_tmp-1;
            if (channel_type != NULL) delete [] channel_type;
            channel_type = new CHANNEL_TYPE[N_ch];
            for (int n=0; n<N_ch; n++){
               loop++;
               channel_type[n].set(argv[loop]);
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
   printf(" @ work dir   = %s\n",work_dir);
   printf(" @ conf list  = %s\n",conf_list);
   printf(" @ #. conf    = %d\n",analysis::Nconf);
   printf(" @ t size     = %d\n",analysis::tSIZE);
   printf(" @ cood size  = %d\n",analysis::xSIZE);
   printf(" @ time_min   = %d\n",time_slice_min);
   printf(" @ time_max   = %d\n",time_slice_max);
   printf(" @ #. t shift = %d\n @ t shift    = { ",N_Tshift);
   for(int loop=0; loop<N_Tshift; loop++)
      printf("%s ",Tshift[loop].c_str());
   printf("}\n @ #. hadron  = %d\n @ hadron     = { ",N_had);
   for(int loop=0; loop<N_had; loop++)
      printf("%s ",hadron_type[loop].name_only.c_str());
   printf("}\n @ #. channel = %d\n @ channel    = { ",N_ch);
   for(int loop=0; loop<N_ch; loop++)
      printf("%s ",channel_type[loop].name.c_str());
   printf("}\n @ x shift    = %s\n",analysis::data_list[N_X_SHIFT]);
   printf(" @ y shift    = %s\n",analysis::data_list[N_Y_SHIFT]);
   printf(" @ z shift    = %s\n",analysis::data_list[N_Z_SHIFT]);
   printf(" @ snk rela   = %s\n",analysis::data_list[SNK_RELA]);
   printf(" @ src rela   = %s\n\n",analysis::data_list[SRC_RELA]);
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
      if (     strcmp(tmp_c1,"AVE_Size_of_space"     )==0){
         analysis::xSIZE = atoi(tmp_c2);
         analysis::ySIZE = atoi(tmp_c2);
         analysis::zSIZE = atoi(tmp_c2);
      }
      else if (strcmp(tmp_c1,"AVE_Size_of_time"      )==0)
         analysis::tSIZE = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"AVE_Min_time_slice"    )==0)
         time_slice_min = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"AVE_Max_time_slice"    )==0)
         time_slice_max = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"AVE_Gauge_confs_list"  )==0)
         snprintf(conf_list,sizeof(conf_list),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"AVE_Path_to_working_dir")==0)
         snprintf(work_dir,sizeof(work_dir),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"AVE_X_shift"           )==0)
         analysis::set_data_list(N_X_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"AVE_Y_shift"           )==0)
         analysis::set_data_list(N_Y_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"AVE_Z_shift"           )==0)
         analysis::set_data_list(N_Z_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"AVE_Snk_relativistic"  )==0)
         analysis::set_data_list(SNK_RELA, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"AVE_Src_relativistic"  )==0)
         analysis::set_data_list(SRC_RELA, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"AVE_T_shift_list"      )==0) {
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
         N_Tshift = count_tmp;
           Tshift = new string[N_Tshift];
         for (int n=0; n<N_Tshift; n++){
            Tshift[n] = tmp_tmp_str[n];
         }
      }
      else if (strcmp(tmp_c1,"AVE_Calc_hadron_name"  )==0) {
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
         N_had       = count_tmp;
         hadron_type = new HADRON_TYPE[N_had];
         for (int n=0; n<N_had; n++){
            hadron_type[n].set(tmp_tmp_str[n]);
         }
      }
      else if (strcmp(tmp_c1,"AVE_Calc_channel_name"  )==0) {
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
         N_ch         = count_tmp;
         channel_type = new CHANNEL_TYPE[N_ch];
         for (int n=0; n<N_ch; n++){
            channel_type[n].set(tmp_tmp_str[n]);
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

static void input_corr_all(  string CORRtype, int CONF
                           , string TSHIFT, double *CORR) {
   
   analysis::set_data_list(MAIN_PATH, "%s/Tshift%s", work_dir, TSHIFT.c_str());
   analysis::set_data_list(N_T_SHIFT, "%s", TSHIFT.c_str());
   analysis::set_data_list(CORR_DIRECTORY, "%s", CORRtype.c_str());
   
   FILE* fp = fopen( analysis::set_path(CONF).c_str(), "r" );
   if(fp == NULL) analysis::error(2, analysis::set_path(CONF).c_str());
   
   int div_t = 1;
   if (CORRtype=="multi") div_t = 2;
   
   int    t_tmp;
   double tmp[10];
   for (int itt=0; itt<analysis::tSIZE/div_t; itt++) {
      if ( fscanf(  fp, " %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf ", &t_tmp
                  , &tmp[0], &tmp[1], &tmp[2], &tmp[3], &tmp[4]
                  , &tmp[5], &tmp[6], &tmp[7], &tmp[8], &tmp[9] ) != 11 )
         analysis::error(3, "invalid reading for correlator file");
      if (itt != t_tmp) analysis::error(3, "read a different time slice");
      for (int loop=0; loop<10; loop++) CORR[loop+10*itt] = tmp[loop];
   }
   fclose(fp);
}

static void output_corr_all(  string CORRtype
                            , int CONF, int N_TSHIFT, double *CORR) {
   
   analysis::set_data_list(MAIN_PATH, "%s/TshiftAve/%dsrc", work_dir, N_TSHIFT);
   analysis::set_data_list(N_T_SHIFT, "A%02d", N_TSHIFT);
   analysis::set_data_list(CORR_DIRECTORY, "%s", CORRtype.c_str());
   
   FILE* fp = fopen( analysis::set_path(CONF).c_str(), "w" );
   if(fp == NULL) analysis::error(2, analysis::set_path(CONF).c_str());
   
   int div_t = 1;
   if (CORRtype=="multi") div_t = 2;

   for (int itt=0; itt<analysis::tSIZE/div_t; itt++)
      if ( fprintf(  fp, "%d %1.16e %1.16e %1.16e %1.16e "
                   "%1.16e %1.16e %1.16e %1.16e %1.16e %1.16e\n", itt
                   , CORR[0+10*itt], CORR[1+10*itt], CORR[2+10*itt]
                   , CORR[3+10*itt], CORR[4+10*itt], CORR[5+10*itt]
                   , CORR[6+10*itt], CORR[7+10*itt]
                   , CORR[8+10*itt], CORR[9+10*itt] ) < 0 )
            analysis::error(3, "invalid writing for correlator file");
   fclose(fp);
}
