//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup potential
 * @brief   Main part for calculate coupled channel potential
 * @author  Takaya Miyamoto
 * @since   Thu Jul 23 23:09:41 JST 2015
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>

#define PROJECT COUPLED_CHANNEL_POT   // <- Project name

#define idx(Row,Colomn) (Colomn+ 2* Row)

int  analysis::Nconf;
int  analysis::xSIZE;
int  analysis::ySIZE;
int  analysis::zSIZE;
int  analysis::tSIZE;
char analysis::data_list[MAX_N_DATA][MAX_LEN_PATH];

size_t NBSwave::xyzSIZE;
size_t NBSwave::NBSSIZE;
size_t NBSwave::xyznSIZE;
size_t NBSwave::NBSnSIZE;

CHANNEL_TYPE channel[4];
int    time_slice;
int    spin;
int    spin_z;
int    ang_mom;
double mass[4];

char conf_list[MAX_LEN_PATH];
char outfile_path[MAX_LEN_PATH];

bool endian_flg, calc_fit_flg, read_from_bin_flg;

bool arguments_check = false;
int  set_args(int, char**);
int  set_args_from_file(char*);

//========================================================================//
int main(int argc, char **argv) {
   if (set_args(argc, argv) == 1) return 0;
   
   time_t start_time, start_time2, end_time;
   time( &start_time );
   
   POTENTIAL *pot = new POTENTIAL[4];
   cdouble   *R_corr_ptr[4];
   
   for (int i=0; i<4; i++) {
      
      if (!read_from_bin_flg) {
         pot[i].set_pot(  channel[i], time_slice, endian_flg
                        , false, spin, ang_mom, mass[i] );
         pot[i].calc_pot_kernel();
         
         pot[i].Rcorr[1].output_Rcorr_all("./tmp");
         pot[i].Rcorr[1].output_Rcorr_err("./tmp");
         pot[i].output_single_pot_err("./tmp");
         pot[i].output_couple_pot_all("./tmp");
         R_corr_ptr[i] = pot[i].Rcorr[1].Rcorr;
         
         pot[i].delete_pot_corr();
         pot[i].Rcorr[0].delete_Rcorr();
         pot[i].Rcorr[2].delete_Rcorr();
      }
      else {
         pot[i].set_pot_from_binary( "./tmp" );
         R_corr_ptr[i] = pot[i].Rcorr->Rcorr;
      }
   }
   
   cdouble *coupled_channel_pot = new cdouble[NBSwave::xyznSIZE * 4];
   cdouble R_corr_inv[4];
   cdouble R_corr_det;
   
   time( &start_time2 );
   printf(" @ calculating coupled channel potental |   0%%");
   for (int loop=0; loop<NBSwave::xyznSIZE; loop++) {
      
      R_corr_det = (  R_corr_ptr[idx(0,0)][loop]
                    * R_corr_ptr[idx(1,1)][loop]
                    - R_corr_ptr[idx(0,1)][loop]
                    * R_corr_ptr[idx(1,0)][loop] );
      
      R_corr_inv[idx(0,0)] =  R_corr_ptr[idx(1,1)][loop] / R_corr_det;
      R_corr_inv[idx(1,0)] = -R_corr_ptr[idx(1,0)][loop] / R_corr_det;
      R_corr_inv[idx(0,1)] = -R_corr_ptr[idx(0,1)][loop] / R_corr_det;
      R_corr_inv[idx(1,1)] =  R_corr_ptr[idx(0,0)][loop] / R_corr_det;
      
      for (int R=0; R<2; R++) for( int C=0; C<2; C++) for( int k=0; k<2; k++)
         coupled_channel_pot[loop + NBSwave::xyznSIZE * idx(R,C)]
         += pot[idx(R,k)].potential[loop] * R_corr_inv[idx(k,C)];
      
      printf("\b\b\b\b%3.0lf%%",double(loop+1)/double(NBSwave::xyznSIZE)*100);
      fflush(stdout);
   }
   printf("\n");
   time( &end_time );
   printf(" @ Coupled channel potential has been calculated :"
          "ELAPSED TIME [s] = %d\n", (int)difftime(end_time,start_time2) );
   
   for (int i=0; i<4; i++)
      for(int loop=0; loop<NBSwave::xyznSIZE; loop++)
         pot[i].potential[loop]
         = coupled_channel_pot[loop + NBSwave::xyznSIZE * i];
   delete [] coupled_channel_pot;
   
   for (int i=0; i<4; i++) {
      pot[i].output_couple_pot_err( outfile_path );
      if (calc_fit_flg)
         pot[i].output_couple_pot_fit( outfile_path );
   }
   delete [] pot;
   
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
         else if (strcmp(argv[loop],"-time"    )==0)
            time_slice = atoi(argv[loop+1]);
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
   printf(" @ t_size     = %d\n",analysis::tSIZE);
   printf(" @ x,y,z_size = %d\n",analysis::xSIZE);
   printf(" @ t shift    = %s\n",analysis::data_list[N_T_SHIFT]);
   printf(" @ x shift    = %s\n",analysis::data_list[N_X_SHIFT]);
   printf(" @ y shift    = %s\n",analysis::data_list[N_Y_SHIFT]);
   printf(" @ z shift    = %s\n",analysis::data_list[N_Z_SHIFT]);
   printf(" @ snk rela   = %s\n",analysis::data_list[SNK_RELA]);
   printf(" @ src rela   = %s\n",analysis::data_list[SRC_RELA]);
   printf(" @ spin       = %d\n",spin);
   printf(" @ spin z cmp = %d\n",spin_z);
   printf(" @ ang mom    = %d\n",ang_mom);
   for(int R=0; R<2; R++) for(int C=0; C<2; C++){
      printf(" @ channel %d%d = %s\n"
             ,R,C,channel[idx(R,C)].name.c_str());
      printf(" @ mass    %d%d = %lf\n"
             ,R,C,mass[idx(R,C)]);
   }
   printf(" @ conf list  = %s\n",conf_list);
   printf(" @ infile     = %s\n",analysis::data_list[MAIN_PATH]);
   printf(" @ outfile    = %s\n @\n",outfile_path);
   printf(" @ endian cnv = %s\n",analysis::bool_to_str(endian_flg).c_str());
   printf(" @ calc fit   = %s\n",analysis::bool_to_str(calc_fit_flg).c_str());
   printf(" @ read bin   = %s\n\n"
          , analysis::bool_to_str(read_from_bin_flg).c_str());
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
      if (     strcmp(tmp_c1,"CCP_Size_of_space"     )==0){
         analysis::xSIZE = atoi(tmp_c2);
         analysis::ySIZE = atoi(tmp_c2);
         analysis::zSIZE = atoi(tmp_c2);
      }
      else if (strcmp(tmp_c1,"CCP_Size_of_time"      )==0)
         analysis::tSIZE = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"CCP_calc_time_slice"   )==0)
         time_slice = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Channel_00"        )==0)
         channel[0].set(tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Channel_01"        )==0)
         channel[1].set(tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Channel_10"        )==0)
         channel[2].set(tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Channel_11"        )==0)
         channel[3].set(tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Spin_projection"   )==0)
         spin = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Spin_z_component"  )==0)
         spin_z = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Ang_mom_projection")==0)
         ang_mom = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Gauge_confs_list"  )==0)
         snprintf(conf_list,sizeof(conf_list),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Path_to_output_dir")==0)
         snprintf(outfile_path,sizeof(outfile_path),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Endian_convert"    )==0)
         endian_flg = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"CCP_out_fit_data"      )==0)
         calc_fit_flg = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Path_to_input_dir" )==0)
         snprintf(         analysis::data_list[MAIN_PATH]
                  , sizeof(analysis::data_list[MAIN_PATH])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"CCP_T_shift"           )==0)
         snprintf(         analysis::data_list[N_T_SHIFT]
                  , sizeof(analysis::data_list[N_T_SHIFT])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"CCP_X_shift"           )==0)
         snprintf(         analysis::data_list[N_X_SHIFT]
                  , sizeof(analysis::data_list[N_X_SHIFT])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Y_shift"           )==0)
         snprintf(         analysis::data_list[N_Y_SHIFT]
                  , sizeof(analysis::data_list[N_Y_SHIFT])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Z_shift"           )==0)
         snprintf(         analysis::data_list[N_Z_SHIFT]
                  , sizeof(analysis::data_list[N_Z_SHIFT])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Snk_relativistic"  )==0)
         snprintf(         analysis::data_list[SNK_RELA]
                  , sizeof(analysis::data_list[SNK_RELA])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Src_relativistic"  )==0)
         snprintf(         analysis::data_list[SRC_RELA]
                  , sizeof(analysis::data_list[SRC_RELA])
                  , "%s", tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Reduced_mass_00"   )==0)
         mass[0] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Reduced_mass_01"   )==0)
         mass[1] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Reduced_mass_10"   )==0)
         mass[2] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"CCP_Reduced_mass_11"   )==0)
         mass[3] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"CCP_read_pot_from_bin" )==0)
         read_from_bin_flg = analysis::str_to_bool(tmp_c2);
      //***************************************************//
      else{
         printf("\n @@@@@@ invalid option \"%s\" in arguments file \"%s\"\n\n"
                , tmp_c1, file_name);
         return 1;
      }
   }
   return 0;
}

#undef idx
