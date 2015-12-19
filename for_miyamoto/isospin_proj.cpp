//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup potential
 * @brief   Main part for isospin projection for S=-1 OCTET baryon
 * @author  Takaya Miyamoto
 * @since   Thu Jul 23 21:41:01 JST 2015
 */
//--------------------------------------------------------------------------

#include <common/analysis.h>
#include <sys/stat.h>

#define PROJECT ISOSPIN_PROJECTION   // <- Project name

int  analysis::Nconf;
int  analysis::xSIZE;
int  analysis::ySIZE;
int  analysis::zSIZE;
int  analysis::tSIZE;
char analysis::data_list[MAX_N_DATA][MAX_LEN_PATH];

int time_min;
int time_max;

char conf_list[MAX_LEN_PATH];

bool endian_flg;

bool arguments_check = false;
int  set_args(int, char**);
int  set_args_from_file(char*);
void set_dir();

//========================================================================//
int main(int argc, char **argv) {
   if (set_args(argc, argv) == 1) return 0;
   
   time_t start_time, start_time2, end_time;
   time( &start_time );

   size_t n_size = analysis::xSIZE*analysis::ySIZE*analysis::zSIZE*4*4;
   string infile_name, outfile_name;
   int t1, t2, t3;
   
   set_dir();
   
#define idx(n,c) n+ n_size* c
   for (int conf=0; conf<analysis::Nconf; conf++) {
      t1 = 0;   t2 = 0;   t3 = 0;
      
      for (int it=time_min; it<=time_max; it++) {
         time( &start_time2 );
         
         cdouble *NBS_in   = new cdouble[ n_size * 9 ];
         cdouble *NBS_proj = new cdouble[ n_size * 7 ];
         snprintf(         analysis::data_list[N_TIME]
                  , sizeof(analysis::data_list[N_TIME]), "%d", it);
         snprintf(         analysis::data_list[NBS_DIRECTORY]
                  , sizeof(analysis::data_list[NBS_DIRECTORY]), "BBwave.dir");
         
         for (int ch=0; ch<9; ch++) {
            snprintf(         analysis::data_list[N_CHANNEL]
                     , sizeof(analysis::data_list[N_CHANNEL]), "%d",ch);
            
            infile_name = analysis::set_path( conf );
            ifstream ifs(infile_name.c_str(), ios::in | ios::binary);
            if(!ifs) analysis::error(2, infile_name.c_str());
            ifs.read((char*)&NBS_in[idx(0,ch)], sizeof(cdouble) * n_size);
            ifs.close();
         }
         if (endian_flg) analysis::endian_convert( NBS_in, n_size * 9 );
         time( &end_time );
         t1 += (int)difftime(end_time,start_time2);
         
         time( &start_time2 );
         for (size_t n=0; n<n_size; n++) {
//================== I_z = 1/2, Lambda N - 1/2, Lambda N =================//
            NBS_proj[idx(n,0)] = NBS_in[idx(n,0)];
            
//================== I_z = 1/2, Lambda N - 1/2, Sigma  N =================//
            NBS_proj[idx(n,1)] =   sqrt(2.0/3.0)*NBS_in[idx(n,2)]
                                 - sqrt(1.0/3.0)*NBS_in[idx(n,1)];
            
//================== I_z = 1/2, Sigma  N - 1/2, Lambda N =================//
            NBS_proj[idx(n,2)] =   sqrt(2.0/3.0)*NBS_in[idx(n,6)]
                                 - sqrt(1.0/3.0)*NBS_in[idx(n,3)];
            
//================== I_z = 1/2, Sigma  N - 1/2, Sigma  N =================//
            NBS_proj[idx(n,3)] = sqrt(2.0/3.0)*(  sqrt(2.0/3.0)*NBS_in[idx(n,8)]
                                                - sqrt(1.0/3.0)*NBS_in[idx(n,5)])
                               - sqrt(1.0/3.0)*(  sqrt(2.0/3.0)*NBS_in[idx(n,7)]
                                                - sqrt(1.0/3.0)*NBS_in[idx(n,4)]);
            
//================== I_z = 1/2, Lambda N - 3/2, Sigma  N =================//
            NBS_proj[idx(n,4)] =   sqrt(1.0/3.0)*NBS_in[idx(n,2)]
                                 + sqrt(2.0/3.0)*NBS_in[idx(n,1)];
            
//================== I_z = 3/2, Sigma  N - 1/2, Lambda N =================//
            NBS_proj[idx(n,5)] =   sqrt(1.0/3.0)*NBS_in[idx(n,6)]
                                 + sqrt(2.0/3.0)*NBS_in[idx(n,3)];
            
//================== I_z = 3/2, Sigma  N - 3/2, Sigma  N =================//
            NBS_proj[idx(n,6)] = sqrt(1.0/3.0)*(  sqrt(1.0/3.0)*NBS_in[idx(n,8)]
                                                + sqrt(2.0/3.0)*NBS_in[idx(n,5)])
                               + sqrt(2.0/3.0)*(  sqrt(1.0/3.0)*NBS_in[idx(n,7)]
                                                + sqrt(2.0/3.0)*NBS_in[idx(n,4)]);
         }
         time( &end_time );
         t2 += (int)difftime(end_time,start_time2);
         
         time( &start_time2 );
         snprintf(         analysis::data_list[NBS_DIRECTORY]
                  , sizeof(analysis::data_list[NBS_DIRECTORY]), "BBwave.prj");
         
         for (int ch=0; ch<7; ch++) {
            snprintf(         analysis::data_list[N_CHANNEL]
                     , sizeof(analysis::data_list[N_CHANNEL]), "%d",ch);
            
            outfile_name = analysis::set_path( conf );
            ofstream ofs(outfile_name.c_str(), ios::out | ios::binary);
            if(!ofs) analysis::error(2, infile_name.c_str());
            ofs.write((char*)&NBS_proj[idx(0,ch)],sizeof(cdouble) * n_size);
            ofs.close();
         }
         time( &end_time );
         t3 += (int)difftime(end_time,start_time2);
         
         delete [] NBS_in;
         delete [] NBS_proj;
      } // it
      printf(" @ conf = %3d || INPUT = %5d [s] |"
             " CALCULATE = %5d [s] | OUTPUT = %5d [s]\n", conf+1, t1, t2, t3 );
      fflush(stdout);
   } // conf
#undef idx
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
   analysis::Nconf = analysis::set_data_list(conf_list);
   
   printf("\n @ Arguments set :\n");
   printf(" @ #. conf    = %d\n",analysis::Nconf);
   printf(" @ x,y,z_size = %d\n",analysis::xSIZE);
   printf(" @ t shift    = %s\n",analysis::data_list[N_T_SHIFT]);
   printf(" @ x shift    = %s\n",analysis::data_list[N_X_SHIFT]);
   printf(" @ y shift    = %s\n",analysis::data_list[N_Y_SHIFT]);
   printf(" @ z shift    = %s\n",analysis::data_list[N_Z_SHIFT]);
   printf(" @ snk rela   = %s\n",analysis::data_list[SNK_RELA]);
   printf(" @ src rela   = %s\n",analysis::data_list[SRC_RELA]);
   printf(" @ time_min   = %d\n",time_min);
   printf(" @ time_max   = %d\n",time_max);
   printf(" @ endian cnv = %s\n",analysis::bool_to_str(endian_flg).c_str());
   printf(" @ conf list  = %s\n",conf_list);
   printf(" @ data file  = %s\n\n",analysis::data_list[MAIN_PATH]);
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
      if (     strcmp(tmp_c1,"ISO_Size_of_space"     )==0){
         analysis::xSIZE = atoi(tmp_c2);
         analysis::ySIZE = atoi(tmp_c2);
         analysis::zSIZE = atoi(tmp_c2);
      }
      else if (strcmp(tmp_c1,"ISO_Min_time_slice"    )==0)
         time_min = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"ISO_Max_time_slice"    )==0)
         time_max = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"ISO_Gauge_confs_list"  )==0)
         snprintf(conf_list,sizeof(conf_list),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"ISO_Endian_convert"    )==0)
         endian_flg = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"ISO_Path_to_directory" )==0)
         snprintf(         analysis::data_list[MAIN_PATH]
                  , sizeof(analysis::data_list[MAIN_PATH])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"ISO_T_shift"           )==0)
         snprintf(         analysis::data_list[N_T_SHIFT]
                  , sizeof(analysis::data_list[N_T_SHIFT])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"ISO_X_shift"           )==0)
         snprintf(         analysis::data_list[N_X_SHIFT]
                  , sizeof(analysis::data_list[N_X_SHIFT])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"ISO_Y_shift"           )==0)
         snprintf(         analysis::data_list[N_Y_SHIFT]
                  , sizeof(analysis::data_list[N_Y_SHIFT])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"ISO_Z_shift"           )==0)
         snprintf(         analysis::data_list[N_Z_SHIFT]
                  , sizeof(analysis::data_list[N_Z_SHIFT])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"ISO_Snk_relativistic"  )==0)
         snprintf(         analysis::data_list[SNK_RELA]
                  , sizeof(analysis::data_list[SNK_RELA])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"ISO_Src_relativistic"  )==0)
         snprintf(         analysis::data_list[SRC_RELA]
                  , sizeof(analysis::data_list[SRC_RELA])
                  , "%s", tmp_c2);
      //***************************************************//
      else{
         printf("\n @@@@@@ invalid option \"%s\" in arguments file \"%s\"\n\n"
                , tmp_c1, file_name);
         return 1;
      }
   }
   return 0;
}

void set_dir() {
   
   char dir_name[2048];
   char conf_dir_name[2048];
   
   printf(" @ output directory setting... |     ");
   for (int channel = 0; channel < 7; channel++) {
      snprintf( dir_name, sizeof(dir_name)
               ,"%s/BBwave.prj.S1.%02d", analysis::data_list[MAIN_PATH], channel);
      mkdir( dir_name, 0755 );
      
      for (int conf_loop=0; conf_loop<analysis::Nconf; conf_loop++){
         snprintf( conf_dir_name, sizeof(conf_dir_name)
                  ,"%s/%s", dir_name, analysis::data_list[conf_loop+MAX_PATH_ARG]);
         mkdir( conf_dir_name, 0755 );
      }
      printf("\b\b\b\b%3.0lf%%",double(channel+1)/double(7)*100);
      fflush(stdout);
   }
   printf("\n");
}
