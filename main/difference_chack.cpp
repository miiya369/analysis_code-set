//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup potential
 * @brief   Main part for difference chack of results of hf-code
 * @author  Takaya Miyamoto
 * @since   Thu Jun  2 00:10:23 JST 2016
 */
//--------------------------------------------------------------------------

#include <NBSwave/NBSwave.h>

#define PROJECT DIFFERENCE_CHACK // <- Project name

static int    time_min, time_max;
static double comp_eps, precision;

static HADRON_TYPE *hadron_type = NULL;
static int          N_had;

static CHANNEL_TYPE *channel_type = NULL;
static int           N_ch;

static string *hadron_dir = NULL;
static int     N_had_dir;

static char conf_list1[MAX_LEN_PATH];
static char conf_list2[MAX_LEN_PATH];
static char input_dir1[MAX_LEN_PATH];
static char input_dir2[MAX_LEN_PATH];

static char tmp_data_lst1[MAX_N_DATA][MAX_LEN_PATH];
static char tmp_data_lst2[MAX_N_DATA][MAX_LEN_PATH];

static bool endian_flg     = false;
static bool read_cmp_flg   = false;
static bool calc_2had_corr = false;

static bool arguments_check = false;
static int  set_args(int, char**);
static int  set_args_from_file(char*);

static void input_corr_all(string, int, double*);

//========================================================================//
int main(int argc, char** argv)
{
   if (set_args(argc, argv) == 1) return 0;
   
   time_t start_time, end_time;
   time(&start_time);
   
   NBS_WAVE_ORG Wave1, Wave2;
   double *Corr1 = new double[10*analysis::tSIZE];
   double *Corr2 = new double[10*analysis::tSIZE];
   
   string fdiff[MAX_N_DATA];
   int    fdiff_swich = 0;
   int    fdiff_count = 0;
   
   size_t count_z       = 0;
   size_t count_CompEps = 0;
   size_t count_m[1000];
   size_t count_p[1000];
   for (int i=0; i<1000; i++)
   {
      count_m[i] = 0;
      count_p[i] = 0;
   }
   double  tmp_f;
   size_t n_size    = Wave1.data_size();
   size_t tot_count = 0;
   
   for (int conf=0; conf<analysis::Nconf; conf++)
   {
      analysis::set_data_list(N_TIME, "-1");
      
      for (int ihad=0; ihad<N_had; ihad++)
      {
         analysis::set_data_list(  HADRON_NAME, "%s"
                                 , hadron_type[ihad].name.c_str() );
         
         for (int idir=0; idir<N_had_dir; idir++)
         {
            analysis::set_data_list(MAIN_PATH, "%s", input_dir1);
            analysis::set_data_list(MAX_PATH_ARG+conf, "%s", tmp_data_lst1[conf]);
            input_corr_all(hadron_dir[idir].c_str(), conf, Corr1);
         
            analysis::set_data_list(MAIN_PATH, "%s", input_dir2);
            analysis::set_data_list(MAX_PATH_ARG+conf, "%s", tmp_data_lst2[conf]);
            input_corr_all(hadron_dir[idir].c_str(), conf, Corr2);
            
            fdiff_swich = 0;
            for (int i=0; i<10*analysis::tSIZE; i++)
            {
               if      (abs(Corr1[i]+Corr2[i]) == 0.0     ) count_z++;
               else if (abs(Corr1[i]+Corr2[i]) <  comp_eps) count_CompEps++;
               else
               {
                  tmp_f = abs(2.0 * (Corr1[i]-Corr2[i]) / (Corr1[i]+Corr2[i]));
                  if      (tmp_f == 0.0    ) count_z++;
                  else if (log10(tmp_f) > 0) count_p[ int(log10(tmp_f))]++;
                  else                       count_m[-int(log10(tmp_f))]++;
                  
                  if (tmp_f > precision && fdiff_swich == 0)
                  {
                     fdiff[fdiff_count] = (  hadron_dir[idir]
                                            + "/"
                                            + hadron_type[ihad].name
                                            + ", "
                                            + tmp_data_lst1[conf] );
                     fdiff_swich = 1;
                     fdiff_count++;
                     if (fdiff_count == MAX_N_DATA)
                     {
                        printf("ERROR: Too many difference.\n");
                        return -1;
                     }
                  }
               }
               tot_count++;
            }
         }
         
         if (hadron_type[ihad].fb_mean_flg)   // for anti-hadron
         {
            string anti_HAD_name;
            anti_HAD_name = "anti" + hadron_type[ihad].name;
            analysis::set_data_list(HADRON_NAME, "%s", anti_HAD_name.c_str());
            
            for (int idir=0; idir<N_had_dir; idir++)
            {
               analysis::set_data_list(MAIN_PATH, "%s", input_dir1);
               analysis::set_data_list(MAX_PATH_ARG+conf,"%s",tmp_data_lst1[conf]);
               input_corr_all(hadron_dir[idir].c_str(), conf, Corr1);
               
               analysis::set_data_list(MAIN_PATH, "%s", input_dir2);
               analysis::set_data_list(MAX_PATH_ARG+conf,"%s",tmp_data_lst2[conf]);
               input_corr_all(hadron_dir[idir].c_str(), conf, Corr2);
               
               fdiff_swich = 0;
               for (int i=0; i<10*analysis::tSIZE; i++)
               {
                  if      (abs(Corr1[i]+Corr2[i]) == 0.0     ) count_z++;
                  else if (abs(Corr1[i]+Corr2[i]) <  comp_eps) count_CompEps++;
                  else
                  {
                     tmp_f = abs(2.0 * (Corr1[i]-Corr2[i]) / (Corr1[i]+Corr2[i]));
                     if      (tmp_f == 0.0    ) count_z++;
                     else if (log10(tmp_f) > 0) count_p[ int(log10(tmp_f))]++;
                     else                       count_m[-int(log10(tmp_f))]++;
                     
                     if (tmp_f > precision && fdiff_swich == 0)
                     {
                        fdiff[fdiff_count] = (  hadron_dir[idir]
                                               + "/"
                                               + "anti"
                                               + hadron_type[ihad].name
                                               + ", "
                                               + tmp_data_lst1[conf] );
                        fdiff_swich = 1;
                        fdiff_count++;
                        if (fdiff_count == MAX_N_DATA)
                        {
                           printf("ERROR: Too many difference.\n");
                           return -1;
                        }
                     }
                  }
                  tot_count++;
               }
            }
         }
      }
      
      for (int ich=0; ich<N_ch; ich++)
      {
         if (calc_2had_corr)
         {
            analysis::set_data_list(N_TIME, "-1");
            analysis::set_data_list(  HADRON_NAME, "%s"
                                    , channel_type[ich].prop_name.c_str());
            
            analysis::set_data_list(MAIN_PATH, "%s", input_dir1);
            analysis::set_data_list(MAX_PATH_ARG+conf, "%s", tmp_data_lst1[conf]);
            input_corr_all("multi", conf, Corr1);
            
            analysis::set_data_list(MAIN_PATH, "%s", input_dir2);
            analysis::set_data_list(MAX_PATH_ARG+conf, "%s", tmp_data_lst2[conf]);
            input_corr_all("multi", conf, Corr2);
            
            fdiff_swich = 0;
            for (int i=0; i<10*analysis::tSIZE/2; i++)
            {
               if      (abs(Corr1[i]+Corr2[i]) == 0.0     ) count_z++;
               else if (abs(Corr1[i]+Corr2[i]) <  comp_eps) count_CompEps++;
               else
               {
                  tmp_f = abs(2.0 * (Corr1[i]-Corr2[i]) / (Corr1[i]+Corr2[i]));
                  if      (tmp_f == 0.0    ) count_z++;
                  else if (log10(tmp_f) > 0) count_p[ int(log10(tmp_f))]++;
                  else                       count_m[-int(log10(tmp_f))]++;
                  
                  if (tmp_f > precision && fdiff_swich == 0)
                  {
                     fdiff[fdiff_count] = (  channel_type[ich].prop_name
                                            + ", "
                                            + tmp_data_lst1[conf] );
                     fdiff_swich = 1;
                     fdiff_count++;
                     if (fdiff_count == MAX_N_DATA)
                     {
                        printf("ERROR: Too many difference.\n");
                        return -1;
                     }
                  }
               }
               tot_count++;
            }
         }
         
         for (int it=time_min; it<=time_max; it++)
         {
            analysis::set_data_list(MAIN_PATH, "%s", input_dir1);
            analysis::set_data_list(MAX_PATH_ARG+conf, "%s", tmp_data_lst1[conf]);
            Wave1.set(channel_type[ich], it, conf, endian_flg, read_cmp_flg);
            
            analysis::set_data_list(MAIN_PATH, "%s", input_dir2);
            analysis::set_data_list(MAX_PATH_ARG+conf, "%s", tmp_data_lst2[conf]);
            Wave2.set(channel_type[ich], it, conf, endian_flg, read_cmp_flg);
            
            int fdiff_swich_r = 0;
            int fdiff_swich_i = 0;
            for (size_t i=0; i<n_size; i++)
            {
               if      (abs(Wave1(i).real() + Wave2(i).real()) == 0.0     )
                  count_z++;
               else if (abs(Wave1(i).real() + Wave2(i).real()) <  comp_eps)
                  count_CompEps++;
               else
               {
                  tmp_f = 2.0 * abs(  (Wave1(i).real() - Wave2(i).real())
                                    / (Wave1(i).real() + Wave2(i).real()) );
                  
                  if      (tmp_f == 0.0    ) count_z++;
                  else if (log10(tmp_f) > 0) count_p[ int(log10(tmp_f))]++;
                  else                       count_m[-int(log10(tmp_f))]++;
                  
                  if (tmp_f > precision && fdiff_swich_r == 0)
                  {
                     char tmp_ch[3];
                     snprintf(tmp_ch, sizeof(tmp_ch), "%d", it);
                     fdiff[fdiff_count] = (  channel_type[ich].name
                                           + " (t="
                                           + tmp_ch
                                           + ")"
                                           + " [Real] , "
                                           + tmp_data_lst1[conf] );
                     fdiff_swich_r = 1;
                     fdiff_count++;
                     if (fdiff_count == MAX_N_DATA)
                     {
                        printf("ERROR: Too many difference.\n");
                        return -1;
                     }
                  }
               }
               tot_count++;
               
               if      (abs(Wave1(i).imag() + Wave2(i).imag()) == 0.0     )
                  count_z++;
               else if (abs(Wave1(i).imag() + Wave2(i).imag()) <  comp_eps)
                  count_CompEps++;
               else
               {
                  tmp_f = 2.0 * abs(  (Wave1(i).imag() - Wave2(i).imag())
                                    / (Wave1(i).imag() + Wave2(i).imag()) );
                  
                  if      (tmp_f == 0.0    ) count_z++;
                  else if (log10(tmp_f) > 0) count_p[ int(log10(tmp_f))]++;
                  else                       count_m[-int(log10(tmp_f))]++;
                  
                  if (tmp_f > precision && fdiff_swich_i == 0)
                  {
                     char tmp_ch[3];
                     snprintf(tmp_ch, sizeof(tmp_ch), "%d", it);
                     fdiff[fdiff_count] = (  channel_type[ich].name
                                           + " (t="
                                           + tmp_ch
                                           + ")"
                                           + " [Imag] , "
                                           + tmp_data_lst1[conf] );
                     fdiff_swich_i = 1;
                     fdiff_count++;
                     if (fdiff_count == MAX_N_DATA)
                     {
                        printf("ERROR: Too many difference.\n");
                        return -1;
                     }
                  }
               }
               tot_count++;
            }
         }
      }
      
   } // conf
   
   size_t count_sum = count_z + count_CompEps;
   printf("\n======================== CHACK RESULTS ========================\n");
   printf("   #.ZERO DIFFERENCE\t=\t%lu\n\n", count_z);
   printf("   #.CompEps DIFFERENCE\t=\t%lu\n\n", count_CompEps);
   for (int i=999; i>=0; i--)
      if (count_p[i] != 0)
      {
         printf("   #.1e%d DIFFERENCE\t=\t%lu\n", i, count_p[i]);
         count_sum += count_p[i];
      }
   for (int i=0; i<1000; i++)
      if (count_m[i] != 0)
      {
         printf("   #.1e%d DIFFERENCE\t=\t%lu\n", -i, count_m[i]);
         count_sum += count_m[i];
      }
   printf("\n   TOTAL SUM\t\t=\t%lu", count_sum);
   printf("\n   TOTAL CHACK\t\t=\t%lu\n\n", tot_count);
   printf("   Different files:\n");
   for (int i=0; i<fdiff_count; i++)
      printf("   \t%s\n", fdiff[i].c_str());
   printf("===============================================================\n");
   
   delete [] Corr1;
   delete [] Corr2;
   
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
         if (     strcmp(argv[loop],"-f"        )==0){}
         //****** You may set additional potion in here ******//
         else if (strcmp(argv[loop],"-idir1"     )==0)
            snprintf(input_dir1,sizeof(input_dir1),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-idir2"     )==0)
            snprintf(input_dir2,sizeof(input_dir2),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-conf_list1")==0)
            snprintf(conf_list1,sizeof(conf_list1),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-conf_list2")==0)
            snprintf(conf_list2,sizeof(conf_list2),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-t_max"    )==0)
            time_max  = atoi(argv[loop+1]);
         else if (strcmp(argv[loop],"-t_min"    )==0)
            time_min  = atoi(argv[loop+1]);
         else if (strcmp(argv[loop],"-hadron")==0)
         {
            int count_tmp = 0;
            do {
               if (loop+count_tmp == argc-1)
               {
                  printf("\n @@@@@@ Need \"@\" at the end of -hadron option\n\n");
                  return 1;
               }
               count_tmp++;
            } while (strcmp(argv[loop+count_tmp],"@") != 0);
            N_had = count_tmp-1;
            if (hadron_type != NULL) delete [] hadron_type;
            hadron_type = new HADRON_TYPE[N_had];
            for (int n=0; n<N_had; n++)
            {
               loop++;
               hadron_type[n].set(argv[loop]);
            }
         }
         else if (strcmp(argv[loop],"-channel")==0)
         {
            int count_tmp = 0;
            do {
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
         else if (strcmp(argv[loop],"-comp"     )==0)
            read_cmp_flg = true;
         else if (strcmp(argv[loop],"-check"    )==0)
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
   int Nconf1 = 0, Nconf2 = 0;
   ifstream tmp_conf1(conf_list1, ios::in);
   if (!tmp_conf1) analysis::error(2, conf_list1);
   ifstream tmp_conf2(conf_list2, ios::in);
   if (!tmp_conf2) analysis::error(2, conf_list2);
   
   while (tmp_conf1.getline(        tmp_data_lst1[Nconf1]
                            ,sizeof(tmp_data_lst1[Nconf1]))) Nconf1++;
   while (tmp_conf2.getline(        tmp_data_lst2[Nconf2]
                            ,sizeof(tmp_data_lst2[Nconf2]))) Nconf2++;
   tmp_conf1.close();
   tmp_conf2.close();
   
   if (Nconf1 != Nconf2)
   {
      printf( "ERROR: #.conf is differ.\nNconf1 = %d\nNconf2 = %d\n"
             , Nconf1, Nconf2 );
      printf( "\nspecified directory is\nNconf1 = %s\nNconf2 = %s\n"
             , conf_list1, conf_list2 );
      return 1;
   }
   analysis::Nconf = Nconf1;
   
   printf("\n @ Arguments set :\n");
   printf(" @ #. conf    = %d\n",analysis::Nconf);
   printf(" @ time size  = %d\n",analysis::tSIZE);
   printf(" @ space size = %d\n",analysis::xSIZE);
   printf(" @ comp eps   = %e\n",comp_eps);
   printf(" @ precision  = %e\n",precision);
   printf(" @ t shift    = %s\n",analysis::data_list[N_T_SHIFT]);
   printf(" @ x shift    = %s\n",analysis::data_list[N_X_SHIFT]);
   printf(" @ y shift    = %s\n",analysis::data_list[N_Y_SHIFT]);
   printf(" @ z shift    = %s\n",analysis::data_list[N_Z_SHIFT]);
   printf(" @ snk rela   = %s\n",analysis::data_list[SNK_RELA]);
   printf(" @ src rela   = %s\n",analysis::data_list[SRC_RELA]);
   printf(" @ time_min   = %d\n",time_min);
   printf(" @ time_max   = %d\n",time_max);
   printf(" @ conf list1 = %s\n",conf_list1);
   printf(" @ conf list2 = %s\n",conf_list2);
   printf(" @ infile 1   = %s\n",input_dir1);
   printf(" @ infile 2   = %s\n",input_dir2);
   printf(" @ hadron dir = { ");
   for(int loop=0; loop<N_had_dir; loop++)
      printf("%s ",hadron_dir[loop].c_str());
   printf("}\n @ #. hadron  = %d\n @ hadron     = { ",N_had);
   for(int loop=0; loop<N_had; loop++)
      printf("%s ",hadron_type[loop].name_only.c_str());
   printf("}\n @ #. channel = %d\n @ channel    = { ",N_ch);
   for(int loop=0; loop<N_ch; loop++)
      printf("%s ",channel_type[loop].name.c_str());
   printf("}\n @ endian cnv = %s\n",analysis::bool_to_str(endian_flg).c_str());
   printf(" @ read comp  = %s\n",analysis::bool_to_str(read_cmp_flg).c_str());
   printf(" @ calc 2had  = %s\n\n",analysis::bool_to_str(calc_2had_corr).c_str());
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
      if (     strcmp(tmp_c1,"DIFF_Size_of_space"     )==0)
      {
         analysis::xSIZE = atoi(tmp_c2);
         analysis::ySIZE = atoi(tmp_c2);
         analysis::zSIZE = atoi(tmp_c2);
      }
      else if (strcmp(tmp_c1,"DIFF_Size_of_time"      )==0)
         analysis::tSIZE = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"DIFF_Min_time_slice"    )==0)
         time_min = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"DIFF_Max_time_slice"    )==0)
         time_max = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"DIFF_Gauge_confs_list1" )==0)
         snprintf(conf_list1,sizeof(conf_list1),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"DIFF_Gauge_confs_list2" )==0)
         snprintf(conf_list2,sizeof(conf_list2),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"DIFF_Computer_eps" )==0)
         comp_eps = atof(tmp_c2);
      else if (strcmp(tmp_c1,"DIFF_Precition" )==0)
         precision = atof(tmp_c2);
      else if (strcmp(tmp_c1,"DIFF_Endian_convert"    )==0)
         endian_flg = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"DIFF_Path_to_input_dir1")==0)
         snprintf(input_dir1,sizeof(input_dir1),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"DIFF_Path_to_input_dir2")==0)
         snprintf(input_dir2,sizeof(input_dir2),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"DIFF_T_shift"           )==0)
         analysis::set_data_list(N_T_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"DIFF_X_shift"           )==0)
         analysis::set_data_list(N_X_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"DIFF_Y_shift"           )==0)
         analysis::set_data_list(N_Y_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"DIFF_Z_shift"           )==0)
         analysis::set_data_list(N_Z_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"DIFF_Snk_relativistic"  )==0)
         analysis::set_data_list(SNK_RELA, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"DIFF_Src_relativistic"  )==0)
         analysis::set_data_list(SRC_RELA, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"DIFF_read_compress_NBS" )==0)
         read_cmp_flg = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"DIFF_Calc_2hadron_corr" )==0)
         calc_2had_corr = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"DIFF_Hadron_directory"  )==0)
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
         N_had_dir  = count_tmp;
         hadron_dir = new string[N_had_dir];
         for (int n=0; n<N_had_dir; n++) hadron_dir[n] = tmp_tmp_str[n];
      }
      else if (strcmp(tmp_c1,"DIFF_Calc_hadron_name"  )==0)
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
         N_had       = count_tmp;
         hadron_type = new HADRON_TYPE[N_had];
         for (int n=0; n<N_had; n++) hadron_type[n].set(tmp_tmp_str[n]);
      }
      else if (strcmp(tmp_c1,"DIFF_Calc_channel_name"  )==0)
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
         for (int n=0; n<N_ch; n++) channel_type[n].set(tmp_tmp_str[n]);
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

static void input_corr_all(string CORRtype, int CONF, double *CORR)
{
   analysis::set_data_list(CORR_DIRECTORY, "%s", CORRtype.c_str());
   
   FILE* fp = fopen(analysis::set_path(CONF).c_str(), "r");
   if(fp == NULL) analysis::error(2, analysis::set_path(CONF).c_str());
   
   int div_t = 1;
   if (CORRtype=="multi") div_t = 2;
   
   int    t_tmp;
   double tmp[10];
   for (int itt=0; itt<analysis::tSIZE/div_t; itt++)
   {
      if ( fscanf(  fp, " %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf ", &t_tmp
                  , &tmp[0], &tmp[1], &tmp[2], &tmp[3], &tmp[4]
                  , &tmp[5], &tmp[6], &tmp[7], &tmp[8], &tmp[9] ) != 11 )
         analysis::error(3, "invalid reading for correlator file");
      if (itt != t_tmp) analysis::error(3, "read a different time slice");
      for (int loop=0; loop<10; loop++) CORR[loop+10*itt] = tmp[loop];
   }
   fclose(fp);
}
