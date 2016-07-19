//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup correlator, NBS wave function
 * @brief   Main part for create Jack-Knife samples
 * @author  Takaya Miyamoto
 * @since   Fri Jun 10 23:53:16 JST 2016
 */
//--------------------------------------------------------------------------

#include <common/analysis.h>

#include <yukawa/PH1_4pt.h>
#include <yukawa/compress48.h>

#define PROJECT MAKE_JACK_KNIFE_SAMPLES    // <- Project name

using namespace yukawa;
typedef four_point_base::BC BC;
typedef PH1                 Core;
typedef four_point<Core>    Four_Point;
typedef compress48<Core>    Compress48;

static int time_slice_min, time_slice_max;
static int bin_size, Nbin;

static HADRON_TYPE *hadron_type = NULL;
static int          N_had;

static CHANNEL_TYPE *channel_type = NULL;
static int           N_ch;

static char conf_list[MAX_LEN_PATH];
static char infile_path[MAX_LEN_PATH];
static char outfile_path[MAX_LEN_PATH];
static char out_base[MAX_LEN_PATH];

static bool arguments_check = false;
static int  set_args(int, char**);
static int  set_args_from_file(char*);

static void input_corr_all(string, double*);
static void make_JK_corr_all( double*, double*, int );
static void output_corr_all(string, double*, int);

//========================================================================//
int main(int argc, char **argv) {
   if (set_args(argc, argv) == 1) return 0;
   
   if (analysis::Nconf % bin_size != 0)
      analysis::error(3, "#.conf is indivisible by the bin size.");
   
   time_t start_time, end_time, timer;
   time( &start_time );
   
   cubic_group::initialize();
   PH1::initialize();
   analysis::set_data_list(NBS_DIRECTORY, "BBwave.dir");
   analysis::set_data_list(OPER_TYPE,     "NUC");
   string corr_dir[2] = {"PS", "SS"};
   
   Nbin = analysis::Nconf / bin_size;
   
   for (int ibin=0; ibin<Nbin; ibin++)
      analysis::set_data_list(  MAX_PATH_ARG + analysis::Nconf + ibin
                              , "%sbin%03d.%03d", out_base, bin_size, ibin+1);
   
   for (int ihad=0; ihad<N_had; ihad++) {
      double *corr_i = new double[10*analysis::tSIZE *analysis::Nconf];
      double *corr_o = new double[10*analysis::tSIZE *Nbin];
      analysis::set_data_list(N_TIME,      "-1");
      analysis::set_data_list(HADRON_NAME, "%s", hadron_type[ihad].name.c_str());
      
      for (int corr_loop=0; corr_loop<2; corr_loop++) {
         input_corr_all  ( corr_dir[corr_loop], corr_i );
         make_JK_corr_all( corr_i, corr_o, 1 );
         
         for (int ibin=0; ibin<Nbin; ibin++)
            output_corr_all(corr_dir[corr_loop], corr_o, ibin);
      }
      
      if (hadron_type[ihad].fb_mean_flg) {   // for anti-hadron
         string anti_HAD_name;
         anti_HAD_name = "anti" + hadron_type[ihad].name;
         analysis::set_data_list(HADRON_NAME, "%s", anti_HAD_name.c_str());
         
         for (int corr_loop=0; corr_loop<2; corr_loop++) {
            input_corr_all  ( corr_dir[corr_loop], corr_i );
            make_JK_corr_all( corr_i, corr_o, 1 );
            
            for (int ibin=0; ibin<Nbin; ibin++)
               output_corr_all(corr_dir[corr_loop], corr_o, ibin);
         }
      }
      delete [] corr_i;
      delete [] corr_o;
      
      time(&timer);
      printf(" @@@ hadron  = %18s END || %s"
             , hadron_type[ihad].name.c_str(), ctime(&timer));
      fflush(stdout);
   } // ihad
   
   for (int ich=0; ich<N_ch; ich++) {
      double *corr_i = new double[10*analysis::tSIZE/2 *analysis::Nconf];
      double *corr_o = new double[10*analysis::tSIZE/2 *Nbin];
      analysis::set_data_list(  N_TIME,      "-1");
      analysis::set_data_list(  HADRON_NAME, "%s"
                              , channel_type[ich].prop_name.c_str());
      
      input_corr_all  ("multi", corr_i);
      make_JK_corr_all( corr_i, corr_o, 2 );
      
      for (int ibin=0; ibin<Nbin; ibin++)
         output_corr_all("multi", corr_o, ibin);
      
      delete [] corr_i;
      delete [] corr_o;
      
      analysis::set_data_list(  N_CHANNEL, "%s"
                              , channel_type[ich].number.c_str());
      
      for (int Tslice=time_slice_min; Tslice<=time_slice_max; Tslice++) {
         analysis::set_data_list(N_TIME, "%03d", Tslice);
         analysis::set_data_list(MAIN_PATH, "%s", infile_path);
         
         mapping48   map;   map.read(analysis::set_path(0).c_str());
         Compress48 *comp = new Compress48[analysis::Nconf];
         comp[0].read(analysis::set_path(0).c_str(), map);
         
         Compress48 sum(comp[0]);
         for (int conf=1; conf<analysis::Nconf; conf++) {
            comp[conf].read(analysis::set_path(conf).c_str(), map);
            sum += comp[conf];
         }
         analysis::set_data_list(MAIN_PATH, "%s", outfile_path);
         
         for (int ibin=0; ibin<Nbin; ibin++) {
            Compress48 tmp_bin(sum);
            for (int conf=0; conf<bin_size; conf++)
               tmp_bin -= comp[conf + bin_size*ibin];
            tmp_bin /= double(analysis::Nconf - bin_size);
            
            tmp_bin.write(analysis::set_path(analysis::Nconf+ibin).c_str());
         }
         delete [] comp;
      }
      time(&timer);
      printf(" @@@ channel = %18s END || %s"
             , channel_type[ich].name.c_str(), ctime(&timer));
      fflush(stdout);
   } // ich

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
         else if (strcmp(argv[loop],"-idir" )==0)
            snprintf(infile_path,sizeof(infile_path),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-odir"  )==0)
            snprintf(outfile_path,sizeof(outfile_path),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-obase" )==0)
            snprintf(out_base,sizeof(out_base),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-src_t" )==0)
            analysis::set_data_list(N_T_SHIFT, "%s", argv[loop+1]);
         else if (strcmp(argv[loop],"-bin_size" )==0)
            bin_size = atoi(argv[loop+1]);
         else if (strcmp(argv[loop],"-conf_list" )==0)
            snprintf(conf_list,sizeof(conf_list),"%s",argv[loop+1]);
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
   printf(" @ conf list  = %s\n",conf_list);
   printf(" @ #. conf    = %d\n",analysis::Nconf);
   printf(" @ input dir  = %s\n",infile_path);
   printf(" @ output dir = %s\n",outfile_path);
   printf(" @ out base   = %s\n",out_base);
   printf(" @ t size     = %d\n",analysis::tSIZE);
   printf(" @ cood size  = %d\n",analysis::xSIZE);
   printf(" @ bin size   = %d\n",bin_size);
   printf(" @ time_min   = %d\n",time_slice_min);
   printf(" @ time_max   = %d\n",time_slice_max);
   printf(" @ #. hadron  = %d\n @ hadron     = { ",N_had);
   for(int loop=0; loop<N_had; loop++)
      printf("%s ",hadron_type[loop].name_only.c_str());
   printf("}\n @ #. channel = %d\n @ channel    = { ",N_ch);
   for(int loop=0; loop<N_ch; loop++)
      printf("%s ",channel_type[loop].name.c_str());
   printf("}\n @ t shift    = %s\n",analysis::data_list[N_T_SHIFT]);
   printf(" @ x shift    = %s\n",analysis::data_list[N_X_SHIFT]);
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
      if (     strcmp(tmp_c1,"JKS_Size_of_space"     )==0){
         analysis::xSIZE = atoi(tmp_c2);
         analysis::ySIZE = atoi(tmp_c2);
         analysis::zSIZE = atoi(tmp_c2);
      }
      else if (strcmp(tmp_c1,"JKS_Size_of_time"      )==0)
         analysis::tSIZE = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"JKS_bin_size"          )==0)
         bin_size = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"JKS_Min_time_slice"    )==0)
         time_slice_min = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"JKS_Max_time_slice"    )==0)
         time_slice_max = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"JKS_Gauge_confs_list"  )==0)
         snprintf(conf_list,sizeof(conf_list),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"JKS_Path_to_input_dir" )==0)
         snprintf(infile_path,sizeof(infile_path),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"JKS_Path_to_output_dir")==0)
         snprintf(outfile_path,sizeof(outfile_path),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"JKS_output_base_name"  )==0)
         snprintf(out_base,sizeof(out_base),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"JKS_T_shift"           )==0)
         analysis::set_data_list(N_T_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"JKS_X_shift"           )==0)
         analysis::set_data_list(N_X_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"JKS_Y_shift"           )==0)
         analysis::set_data_list(N_Y_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"JKS_Z_shift"           )==0)
         analysis::set_data_list(N_Z_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"JKS_Snk_relativistic"  )==0)
         analysis::set_data_list(SNK_RELA, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"JKS_Src_relativistic"  )==0)
         analysis::set_data_list(SRC_RELA, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"JKS_Calc_hadron_name"  )==0) {
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
      else if (strcmp(tmp_c1,"JKS_Calc_channel_name"  )==0) {
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

static void input_corr_all( string CORRtype, double *CORR ) {

   int div_t = 1;
   if (CORRtype=="multi") div_t = 2;
   analysis::set_data_list(CORR_DIRECTORY, "%s", CORRtype.c_str());
   analysis::set_data_list(MAIN_PATH, "%s", infile_path);
   
   int    t_tmp;
   double tmp[10];
   
   for (int conf=0; conf<analysis::Nconf; conf++) {
      FILE* fp = fopen( analysis::set_path(conf).c_str(), "r" );
      if(fp == NULL) analysis::error(2, analysis::set_path(conf).c_str());
   
      for (int itt=0; itt<analysis::tSIZE/div_t; itt++) {
         if ( fscanf(  fp, " %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf ", &t_tmp
                     , &tmp[0], &tmp[1], &tmp[2], &tmp[3], &tmp[4]
                     , &tmp[5], &tmp[6], &tmp[7], &tmp[8], &tmp[9] ) != 11 )
            analysis::error(3, "invalid reading for correlator file");
         if (itt != t_tmp) analysis::error(3, "read a different time slice");
         
         for (int loop=0; loop<10; loop++)
            CORR[loop + 10*(itt + analysis::tSIZE/div_t * conf)] = tmp[loop];
      }
      fclose(fp);
   }
}

static void make_JK_corr_all( double *CORR, double *CORR_bin, int div_t ) {
   
   for (int n=0; n<10*analysis::tSIZE/div_t; n++) {
      double sum = 0.0;
      
      for (int i=0; i<analysis::Nconf; i++)
         sum += CORR[n + 10*analysis::tSIZE/div_t * i];
      
      for (   int i=0; i<Nbin;  i++) {
         double sum_bin = 0.0;
         for (int b=0; b<bin_size; b++)
            sum_bin += CORR[n + 10*analysis::tSIZE/div_t *(b + bin_size*i)];
         
         CORR_bin[n + 10*analysis::tSIZE/div_t * i]
         = ( sum - sum_bin ) / double(analysis::Nconf-bin_size);
      }
   }
}

static void output_corr_all(string CORRtype, double *CORR_bin, int iBin) {
   
   analysis::set_data_list(CORR_DIRECTORY, "%s", CORRtype.c_str());
   analysis::set_data_list(MAIN_PATH, "%s", outfile_path);
   
   FILE* fp = fopen( analysis::set_path(analysis::Nconf+iBin).c_str(), "w" );
   if(fp == NULL)
      analysis::error(2, analysis::set_path(analysis::Nconf+iBin).c_str());
   
   int div_t = 1;
   if (CORRtype=="multi") div_t = 2;
   
   double *CORR = &CORR_bin[10*analysis::tSIZE/div_t * iBin];

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
