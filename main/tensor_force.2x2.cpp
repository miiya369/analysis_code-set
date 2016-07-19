//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup potential
 * @brief   Main part for 2x2 coupled channel tensor potential calculation
 * @author  Takaya Miyamoto
 * @since   Sun Jul 17 06:07:42 JST 2016
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>
#include <gauge/matrix_func.h>

#define PROJECT CALC_TENSOR_FORCE_2x2 // <- Project name

#define IT(iii,ttt) ((iii)+ 4* (ttt))
#define RC(Row,Colomn) ((Colomn)+ 2* (Row))

static int    time_slice;
static double mass[4];
static double Zfactor01;

static char conf_list[MAX_LEN_PATH];
static char outfile_path[MAX_LEN_PATH];

static bool endian_flg    = false;
static bool read_cmp_flg  = false;
static bool out_all_datas = false;

static bool use_JK_data   = false;

static bool arguments_check = false;
static int  set_args(int, char**);
static int  set_args_from_file(char*);

//========================================================================//
int main(int argc, char **argv)
{
   if (set_args(argc, argv) == 1) return 0;
   
   time_t start_time, end_time;
   time( &start_time );
   
   if (!use_JK_data) {
      printf("ERROR: Jack-Knife bin data should be used for this code.\n");
      return -1;
   }
   
   size_t nsize = analysis::xSIZE*analysis::ySIZE*analysis::zSIZE;
   NBSwave::rot_matrix_init();
   
   string channel_prj_name[4]
   = {  "Nuc-Lam12_Nuc-Lam12", "Nuc-Lam12_Nuc-Sig12"
      , "Nuc-Sig12_Nuc-Lam12", "Nuc-Sig12_Nuc-Sig12" };
   
   string hadron_name[3]
   = { "proton" , "Lambda" , "Sigma" };
   
   SPIN_TYPE spin("S1_Sz0");
   
   CHANNEL_TYPE channel;
   HADRON_TYPE  hadron;
   
   NBS_WAVE_ORG     Wave_org;
   NBS_WAVE_SRC_PRJ S12Wave_sprj;
   NBS_WAVE_SRC_PRJ Wave_sprj;
   CORRELATOR       Corr[3];
   
   R_CORRELATOR_SRC_PRJ K_Rcorr_sprj, tmp;
   R_CORRELATOR_SRC_PRJ Rcorr_sprj[3], S12_Rcorr_sprj;
   
   R_CORRELATOR Rcorr[2*4], S12_Rcorr[2*4], K_Rcorr[2*4];
   
   CONFIG<POTENTIAL> V_C[4], V_T[4];
   
   string pot_type;
   char   outfile_name[1024];
   int    itmp;
   
   for (int conf=0; conf<analysis::Nconf; conf++)
   {
      for (int loop=0; loop<3; loop++) {
         hadron.set (hadron_name[loop]);
         Corr[loop].set(hadron, conf, 0, "PS");
      } // loop
      
      for (int loop=0; loop<4; loop++) {
         V_C[loop](conf).mem_alloc();
         V_T[loop](conf).mem_alloc();
         
         channel.set(channel_prj_name[loop]);
         if (loop < 2) itmp = 1;
         else          itmp = 2;
         
         for (int it=-1; it<=+1; it++) {
            Wave_org.set(channel, time_slice+it, conf, endian_flg, read_cmp_flg);
            NBSwave::spin_projection(Wave_org, Wave_sprj, spin);
         
            Rcorr_sprj[it+1].set(Wave_sprj, Corr[0], Corr[itmp], time_slice+it);
            
            if (it == 0) {
               NBSwave::S12_psi(Wave_sprj, S12Wave_sprj);
               S12_Rcorr_sprj.set(S12Wave_sprj,Corr[0],Corr[itmp],time_slice+it);
            }
         } // it
         
         pot_type = potential::kernel(  K_Rcorr_sprj
                                      , Rcorr_sprj[0], Rcorr_sprj[1], Rcorr_sprj[2]
                                      , mass[loop] );
         
         Rcorrelator::Swave_division (Rcorr_sprj[1], tmp);
         Rcorrelator::spin_projection(Rcorr_sprj[1], Rcorr[IT(loop,0)], spin);
         Rcorrelator::mult_YDstar    (tmp, spin);
         Rcorrelator::spin_projection(tmp, Rcorr[IT(loop,1)], SPIN_1_p1);
         
         Rcorrelator::Swave_division (S12_Rcorr_sprj, tmp);
         Rcorrelator::spin_projection(S12_Rcorr_sprj, S12_Rcorr[IT(loop,0)], spin);
         Rcorrelator::mult_YDstar    (tmp, spin);
         Rcorrelator::spin_projection(tmp, S12_Rcorr[IT(loop,1)], SPIN_1_p1);
         
         Rcorrelator::Swave_division (K_Rcorr_sprj, tmp);
         Rcorrelator::spin_projection(K_Rcorr_sprj, K_Rcorr[IT(loop,0)], spin);
         Rcorrelator::mult_YDstar    (tmp, spin);
         Rcorrelator::spin_projection(tmp, K_Rcorr[IT(loop,1)], SPIN_1_p1);
         
      } // loop
      
#ifdef _OPENMP
#pragma omp parallel for
#endif
      for (size_t n=0; n<nsize; n++) {
         cdouble Vmat[8];
         cdouble Kmat[8];
         cdouble Rmat[16];
         cdouble Rmat_inv[16];
         
         Kmat[0] = K_Rcorr[IT(0,0)](n); Kmat[1] = K_Rcorr[IT(2,0)](n);
         Kmat[2] = K_Rcorr[IT(0,1)](n); Kmat[3] = K_Rcorr[IT(2,1)](n);
         Kmat[4] = K_Rcorr[IT(1,0)](n); Kmat[5] = K_Rcorr[IT(3,0)](n);
         Kmat[6] = K_Rcorr[IT(1,1)](n); Kmat[7] = K_Rcorr[IT(3,1)](n);
         
         Rmat[0]  = Rcorr[IT(0,0)](n); Rmat[1]  = S12_Rcorr[IT(0,0)](n);
         Rmat[2]  = Rcorr[IT(2,0)](n); Rmat[3]  = S12_Rcorr[IT(2,0)](n);
         
         Rmat[4]  = Rcorr[IT(0,1)](n); Rmat[5]  = S12_Rcorr[IT(0,1)](n);
         Rmat[6]  = Rcorr[IT(2,1)](n); Rmat[7]  = S12_Rcorr[IT(2,1)](n);
         
         Rmat[8]  = Rcorr[IT(1,0)](n); Rmat[9]  = S12_Rcorr[IT(1,0)](n);
         Rmat[10] = Rcorr[IT(3,0)](n); Rmat[11] = S12_Rcorr[IT(3,0)](n);
         
         Rmat[12] = Rcorr[IT(1,1)](n); Rmat[13] = S12_Rcorr[IT(1,1)](n);
         Rmat[14] = Rcorr[IT(3,1)](n); Rmat[15] = S12_Rcorr[IT(3,1)](n);
         
         inv_mat_gauss(Rmat, Rmat_inv, 4);
         
         for (int i=0; i<4; i++) for (int j=0; j<2; j++) {
            Vmat[j+2*i] = COMP_ZERO;
            for (int k=0; k<4; k++)
               Vmat[j+2*i] += Rmat_inv[k+4*i] * Kmat[j+2*k];
         }
         
         V_C[0](conf)(n) = Vmat[0]; V_C[2](conf)(n) = Vmat[1];
         V_T[0](conf)(n) = Vmat[2]; V_T[2](conf)(n) = Vmat[3];
         V_C[1](conf)(n) = Vmat[4]; V_C[3](conf)(n) = Vmat[5];
         V_T[1](conf)(n) = Vmat[6]; V_T[3](conf)(n) = Vmat[7];
         
         V_C[1](conf)(n) *= (Corr[1](time_slice) / Corr[2](time_slice));
         V_C[1](conf)(n) *= (1.0 / Zfactor01);
         V_T[1](conf)(n) *= (Corr[1](time_slice) / Corr[2](time_slice));
         V_T[1](conf)(n) *= (1.0 / Zfactor01);
         V_C[2](conf)(n) *= (Corr[2](time_slice) / Corr[1](time_slice));
         V_C[2](conf)(n) *= Zfactor01;
         V_T[2](conf)(n) *= (Corr[2](time_slice) / Corr[1](time_slice));
         V_T[2](conf)(n) *= Zfactor01;
         
      } // n
      
   } // conf
   
   for (int loop=0; loop<4; loop++) {
      snprintf(  outfile_name, sizeof(outfile_name)
               , "%s/%s_center_%s_%s_ccp_err_t%d"
               , outfile_path, channel_prj_name[loop].c_str()
               , pot_type.c_str(), spin.name.c_str(), time_slice );
      
      analysis::output_data_err(V_C[loop], outfile_name, use_JK_data);
      
      snprintf(  outfile_name, sizeof(outfile_name)
               , "%s/%s_tensor_%s_%s_ccp_err_t%d"
               , outfile_path, channel_prj_name[loop].c_str()
               , pot_type.c_str(), spin.name.c_str(), time_slice );
      
      analysis::output_data_err(V_T[loop], outfile_name, use_JK_data);
      
      if (out_all_datas) {
         snprintf(  outfile_name, sizeof(outfile_name)
                  , "%s/%s_center_%s_%s_ccp_all_t%d"
                  , outfile_path, channel_prj_name[loop].c_str()
                  , pot_type.c_str(), spin.name.c_str(), time_slice );
         
         analysis::output_data_all(V_C[loop], outfile_name);
         
         snprintf(  outfile_name, sizeof(outfile_name)
                  , "%s/%s_tensor_%s_%s_ccp_all_t%d"
                  , outfile_path, channel_prj_name[loop].c_str()
                  , pot_type.c_str(), spin.name.c_str(), time_slice );
         
         analysis::output_data_all(V_T[loop], outfile_name);
      }
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
         else if (strcmp(argv[loop],"-idir"     )==0)
            analysis::set_data_list(MAIN_PATH, "%s", argv[loop+1]);
         else if (strcmp(argv[loop],"-conf_list" )==0)
            snprintf(conf_list,sizeof(conf_list),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-odir"     )==0)
            snprintf(outfile_path,sizeof(outfile_path),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-time"    )==0)
            time_slice  = atoi(argv[loop+1]);
         else if (strcmp(argv[loop],"-src_t" )==0)
            analysis::set_data_list(N_T_SHIFT, "%s", argv[loop+1]);
         else if (strcmp(argv[loop],"-comp"     )==0)
            read_cmp_flg = true;
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
   printf(" @ time size  = %d\n",analysis::tSIZE);
   printf(" @ space size = %d\n",analysis::xSIZE);
   printf(" @ t shift    = %s\n",analysis::data_list[N_T_SHIFT]);
   printf(" @ x shift    = %s\n",analysis::data_list[N_X_SHIFT]);
   printf(" @ y shift    = %s\n",analysis::data_list[N_Y_SHIFT]);
   printf(" @ z shift    = %s\n",analysis::data_list[N_Z_SHIFT]);
   printf(" @ snk rela   = %s\n",analysis::data_list[SNK_RELA]);
   printf(" @ src rela   = %s\n",analysis::data_list[SRC_RELA]);
   printf(" @ time slice = %d\n",time_slice);
   for(int i=0; i<4; i++) printf(" @ mass %d     = %lf\n",i,mass[i]);
   printf(" @ Z factor   = %lf\n",Zfactor01);
   printf(" @ conf list  = %s\n",conf_list);
   printf(" @ infile     = %s\n",analysis::data_list[MAIN_PATH]);
   printf(" @ outfile    = %s\n",outfile_path);
   printf(" @ endian cnv = %s\n",analysis::bool_to_str(endian_flg).c_str());
   printf(" @ read comp  = %s\n",analysis::bool_to_str(read_cmp_flg).c_str());
   printf(" @ use JK data= %s\n",analysis::bool_to_str(use_JK_data).c_str());
   printf(" @ out all    = %s\n\n",analysis::bool_to_str(out_all_datas).c_str());
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
      if (     strcmp(tmp_c1,"VT2_Size_of_space"     )==0){
         analysis::xSIZE = atoi(tmp_c2);
         analysis::ySIZE = atoi(tmp_c2);
         analysis::zSIZE = atoi(tmp_c2);
      }
      else if (strcmp(tmp_c1,"VT2_time_slice"    )==0)
         time_slice = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"VT2_Gauge_confs_list"  )==0)
         snprintf(conf_list,sizeof(conf_list),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"VT2_Path_to_output_dir")==0)
         snprintf(outfile_path,sizeof(outfile_path),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"VT2_Endian_convert"    )==0)
         endian_flg = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"VT2_Path_to_input_dir" )==0)
         analysis::set_data_list(MAIN_PATH, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"VT2_T_shift"           )==0)
         analysis::set_data_list(N_T_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"VT2_X_shift"           )==0)
         analysis::set_data_list(N_X_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"VT2_Y_shift"           )==0)
         analysis::set_data_list(N_Y_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"VT2_Z_shift"           )==0)
         analysis::set_data_list(N_Z_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"VT2_Size_of_time"      )==0)
         analysis::tSIZE = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"VT2_Snk_relativistic"  )==0)
         analysis::set_data_list(SNK_RELA, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"VT2_Src_relativistic"  )==0)
         analysis::set_data_list(SRC_RELA, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"VT2_Reduced_mass_00"   )==0)
         mass[0] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"VT2_Reduced_mass_01"   )==0)
         mass[1] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"VT2_Reduced_mass_10"   )==0)
         mass[2] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"VT2_Reduced_mass_11"   )==0)
         mass[3] = atof(tmp_c2);
      else if (strcmp(tmp_c1,"VT2_Z_factor_01"       )==0)
         Zfactor01 = atof(tmp_c2);
      else if (strcmp(tmp_c1,"VT2_read_compress_NBS" )==0)
         read_cmp_flg = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"VT2_Use_jack_knife_data")==0)
         use_JK_data = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"VT2_Out_all_datas" )==0)
         out_all_datas = analysis::str_to_bool(tmp_c2);
      //***************************************************//
      else{
         printf("\n @@@@@@ invalid option \"%s\" in arguments file \"%s\"\n\n"
                , tmp_c1, file_name);
         return 1;
      }
   }
   return 0;
}

#undef IT
#undef RC
