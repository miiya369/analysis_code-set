//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup correlator
 * @brief   Main part for effective mass calculation
 * @author  Takaya Miyamoto
 * @since   Mon Oct 17 22:43:51 JST 2016
 */
//--------------------------------------------------------------------------

#include <fitting/fitting.h>
#include <correlator/correlator.h>

#define PROJECT EFFECTIVEMASS    // <- Project name

static int           Nhad;
static int           Nch;
static HADRON_TYPE  *hadron_type  = NULL;
static CHANNEL_TYPE *channel_type = NULL;
static int           fit_range_min;
static int           fit_range_max;
static int           momentum;
static SPIN_TYPE     spin;
static double        lat_space;
static string        dir_type;

static char conf_list[MAX_LEN_PATH];
static char outfile_path[MAX_LEN_PATH];

static bool take_JK_flg    = false;
static bool use_JK_data    = false;
static bool input_fit_flg  = false;
static bool out_fit_flg    = false;
static bool calc_flg_fit   = false;
static bool calc_flg_Rcorr = false;

static bool arguments_check = false;
static int  set_args(int, char**);
static int  set_args_from_file(char*);

//========================================================================//
int main(int argc, char **argv)
{
   if (set_args(argc, argv) == 1) return 0;
   
   time_t start_time, end_time;
   time(&start_time);
   
   CONFIG<CORRELATOR> *Corr     = new CONFIG<CORRELATOR>;
   CONFIG<CORRELATOR> *eff_mass = new CONFIG<CORRELATOR>;
   CONFIG<FIT>        *fit      = new CONFIG<FIT>;
   
   double *data_all = NULL;
   if (input_fit_flg) data_all = new double[analysis::tSIZE * analysis::Nconf];
   
   char outfile_name[1024];
   char infile_name [1024];
   
   double  param_ini[2] = { 0.001, 1.0 };   // <- init. value of effmass fit
   double *data         = new double[analysis::tSIZE];
   double *cood         = new double[analysis::tSIZE];
   double *err          = new double[analysis::tSIZE];
   double *chisq        = new double[analysis::Nconf];
   double *param        = new double[analysis::Nconf];
   double *mass_had     = new double[Nhad*4];
   double *mass_NBS     = new double[Nch *4];
   
   for (int it=0; it<analysis::tSIZE; it++) cood[it] = it;
   
   /// Calculation for 2pt-correlators ///
   for (int ihad=0; ihad<Nhad; ihad++)
   {
      /// Input 2pt-correlators & Output effective masses ///
      
      if (!input_fit_flg) {
         for (int conf=0; conf<analysis::Nconf; conf++)
            (*Corr)(conf).set(hadron_type[ihad], conf, momentum, dir_type.c_str());
         
         if (take_JK_flg) Corr->make_JK_sample();
         
         for (int conf=0; conf<analysis::Nconf; conf++) {
            (*eff_mass)(conf).mem_alloc(analysis::tSIZE-1);
            for (int it=0; it<analysis::tSIZE-1; it++)
               (*eff_mass)(conf)(it) = -log(  (*Corr)(conf)(it+1)
                                            / (*Corr)(conf)(it) );
         }
         snprintf(  outfile_name, sizeof(outfile_name)
                  , "%s/%s_%s_effmass_mom%d_err"
                  , outfile_path, hadron_type[ihad].name.c_str()
                  , dir_type.c_str(), momentum );
         
         analysis::output_data_err(*eff_mass, outfile_name, use_JK_data);
      }
      else {
         snprintf(  infile_name, sizeof(infile_name)
                  , "%s/%s_%s_correlator_mom%d_fit"
                  , analysis::data_list[MAIN_PATH]
                  , hadron_type[ihad].name.c_str()
                  , dir_type.c_str(), momentum );
         
         int tmp_Ndata, tmp_Nconf;
         fitting::input_data_binary(infile_name, tmp_Nconf, tmp_Ndata);
         
         if (tmp_Nconf != analysis::Nconf || tmp_Ndata != analysis::tSIZE) {
            printf(" @@@ ERROR : Different Nconf or Ndata\n"
                   "           : Nconf in binary = %d\n"
                   "           : Ndata in binary = %d\n\n", tmp_Nconf, tmp_Ndata);
            return -1;
         }
         fitting::input_data_binary(infile_name, cood, data_all, err);
      }
      
      /// Output 2pt-correlators with all configurations (For fit) ///
      
      if (out_fit_flg) {
         snprintf(  outfile_name, sizeof(outfile_name)
                  , "%s/%s_%s_correlator_mom%d_fit"
                  , outfile_path, hadron_type[ihad].name.c_str()
                  , dir_type.c_str(), momentum );
         
         analysis::output_data_fit(*Corr, outfile_name, use_JK_data);
      }
      
      /// Calculate the fitting by using A exp(-Bt) ///
      
      if (calc_flg_fit) {
         if (!input_fit_flg) Corr->make_mean_err(data, err, use_JK_data);
         
         for (int conf=0; conf<analysis::Nconf; conf++) {
            if (!input_fit_flg)
               for (int it=0; it<analysis::tSIZE; it++)
                  data[it] = (*Corr)(conf)(it).real();
            else
               for (int it=0; it<analysis::tSIZE; it++)
                  data[it] = data_all[conf+analysis::Nconf*it];
            
            (*fit)(conf).set_func("1EXP", param_ini);
            chisq[conf] =
            (*fit)(conf).fit_data_NR(  cood, data, err, analysis::tSIZE
                                     , fit_range_min, fit_range_max, 0.000001 );
            param[conf] = (*fit)(conf)(1);
         }
         analysis::make_mean_err(  param, mass_had[0+4*ihad], mass_had[1+4*ihad]
                                 , analysis::Nconf, use_JK_data  );
         analysis::make_mean_err(  chisq, mass_had[2+4*ihad], mass_had[3+4*ihad]
                                 , analysis::Nconf, true );
      }
      
   } // Nhad
   
   /// Calculation for NBSwave time-correlators (Or R-correlators) ///
   for (int ich=0; ich<Nch; ich++)
   {
      /// Input NBSwave time-correlators & Output effective masses ///
      
      if (!input_fit_flg) {
         for (int conf=0; conf<analysis::Nconf; conf++)
            (*Corr)(conf).set(channel_type[ich], conf, spin);
         
         if (take_JK_flg) Corr->make_JK_sample();
         
         if (calc_flg_Rcorr) {
            CONFIG<CORRELATOR> *Corr1 = new CONFIG<CORRELATOR>;
            CONFIG<CORRELATOR> *Corr2 = new CONFIG<CORRELATOR>;
            
            for (int conf=0; conf<analysis::Nconf; conf++) {
               (*Corr1)(conf).set(  channel_type[ich].hadron1
                                  , conf, 0, "PS" );
               (*Corr2)(conf).set(  channel_type[ich].hadron2
                                  , conf, 0, "PS" );
            }
            if (take_JK_flg) Corr1->make_JK_sample();
            if (take_JK_flg) Corr2->make_JK_sample();
            
            for (   int conf=0; conf<analysis::Nconf;   conf++)
               for (int it  =0; it  <analysis::tSIZE/2; it++)
                  (*Corr)(conf)(it) /= (*Corr1)(conf)(it) * (*Corr2)(conf)(it);
            
            delete Corr1;
            delete Corr2;
         }
         for (int conf=0; conf<analysis::Nconf; conf++) {
            (*eff_mass)(conf).mem_alloc(analysis::tSIZE/2-1);
            for (int it=0; it<analysis::tSIZE/2-1; it++)
               (*eff_mass)(conf)(it) = -log(  (*Corr)(conf)(it+1)
                                            / (*Corr)(conf)(it) );
         }
         if (calc_flg_Rcorr) snprintf(  outfile_name, sizeof(outfile_name)
                                      , "%s/%s_Rcorrelator_%s_effmass_err", outfile_path
                                      , channel_type[ich].name.c_str()
                                      , spin.name.c_str() );
         else snprintf(  outfile_name, sizeof(outfile_name)
                       , "%s/%s_%s_effmass_err", outfile_path
                       , channel_type[ich].name.c_str(), spin.name.c_str() );
         
         analysis::output_data_err(*eff_mass, outfile_name, use_JK_data);
      }
      else {
         if (calc_flg_Rcorr) snprintf(  infile_name, sizeof(infile_name)
                                      , "%s/%s_Rcorrelator_%s_fit"
                                      , analysis::data_list[MAIN_PATH]
                                      , channel_type[ich].name.c_str()
                                      , spin.name.c_str() );
         else snprintf(  infile_name, sizeof(infile_name)
                       , "%s/%s_correlator_%s_fit"
                       , analysis::data_list[MAIN_PATH]
                       , channel_type[ich].name.c_str(), spin.name.c_str() );
         
         int tmp_Ndata, tmp_Nconf;
         fitting::input_data_binary(infile_name, tmp_Nconf, tmp_Ndata);
         
         if (tmp_Nconf != analysis::Nconf || tmp_Ndata != analysis::tSIZE/2) {
            printf(" @@@ ERROR : Different Nconf or Ndata\n"
                   "           : Nconf in binary = %d\n"
                   "           : Ndata in binary = %d\n\n", tmp_Nconf, tmp_Ndata);
            return -1;
         }
         fitting::input_data_binary(infile_name, cood, data_all, err);
      }
      
      /// Output NBSwave time-correlators with all configurations (For fit) ///
      
      if (out_fit_flg) {
         if (calc_flg_Rcorr) snprintf(  outfile_name, sizeof(outfile_name)
                                      , "%s/%s_Rcorrelator_%s_fit", outfile_path
                                      , channel_type[ich].name.c_str()
                                      , spin.name.c_str() );
         else snprintf(  outfile_name, sizeof(outfile_name)
                       , "%s/%s_correlator_%s_fit", outfile_path
                       , channel_type[ich].name.c_str(), spin.name.c_str() );
         
         analysis::output_data_fit(*Corr, outfile_name, use_JK_data);
      }
      
      /// Calculate the fitting by using A exp(-Bt) ///
      
      if (calc_flg_fit) {
         if (!input_fit_flg) Corr->make_mean_err(data, err, use_JK_data);
         
         for (int conf=0; conf<analysis::Nconf; conf++) {
            if (!input_fit_flg)
               for (int it=0; it<analysis::tSIZE/2; it++)
                  data[it] = (*Corr)(conf)(it).real();
            else
               for (int it=0; it<analysis::tSIZE/2; it++)
                  data[it] = data_all[conf+analysis::Nconf*it];
            
            (*fit)(conf).set_func( "1EXP", param_ini );
            chisq[conf] =
            (*fit)(conf).fit_data_NR(  cood, data, err, analysis::tSIZE/2
                                     , fit_range_min, fit_range_max, 0.000001 );
            param[conf] = (*fit)(conf)(1);
         }
         analysis::make_mean_err(  param, mass_NBS[0+4*ich], mass_NBS[1+4*ich]
                                 , analysis::Nconf, use_JK_data  );
         analysis::make_mean_err(  chisq, mass_NBS[2+4*ich], mass_NBS[3+4*ich]
                                 , analysis::Nconf, true );
      }
      
   } // Nch
   
   delete [] data;
   delete [] cood;
   delete [] err;
   delete [] chisq;
   delete [] param;
   
   if (input_fit_flg) delete [] data_all;

   delete fit;
   delete Corr;
   delete eff_mass;
   
   /// Show the fitting results ///
   
   if (calc_flg_fit) {
      printf("\n"
             " @ =========================================================== @\n");
      printf(" @               EFFECTIVE MASS FITTING RESULTS                @\n");
      printf(" @ =========================================================== @\n"
             "\n");
      for (int ihad=0; ihad<Nhad; ihad++)
         printf(" %18s : [Lattice Unit] %lf +/- %lf\n"
                "                    : [   MeV/c2   ] %lf +/- %lf\n"
                "                    : [ chisq/conf ] %lf +/- %lf\n\n"
                , hadron_type[ihad].name_only.c_str()
                , mass_had[0+4*ihad], mass_had[1+4*ihad]
                , mass_had[0+4*ihad]*197.327/lat_space
                , mass_had[1+4*ihad]*197.327/lat_space
                , mass_had[2+4*ihad], mass_had[3+4*ihad] );
      for (int ich=0; ich<Nch; ich++)
         printf(" %18s : [Lattice Unit] %lf +/- %lf\n"
                "                    : [   MeV/c2   ] %lf +/- %lf\n"
                "                    : [ chisq/conf ] %lf +/- %lf\n\n"
                , channel_type[ich].name.c_str()
                , mass_NBS[0+4*ich], mass_NBS[1+4*ich]
                , mass_NBS[0+4*ich]*197.327/lat_space
                , mass_NBS[1+4*ich]*197.327/lat_space
                , mass_NBS[2+4*ich], mass_NBS[3+4*ich] );
      printf(" @ =========================================================== @\n");
   }
   if (hadron_type  != NULL) delete [] hadron_type;
   if (channel_type != NULL) delete [] channel_type;
   delete [] mass_had;
   delete [] mass_NBS;
   
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
         else if (strcmp(argv[loop],"-in_fit")==0)
            input_fit_flg = true;
         else if (strcmp(argv[loop],"-fit"   )==0)
            calc_flg_fit = true;
         else if (strcmp(argv[loop],"-calc_R")==0)
            calc_flg_Rcorr = true;
         else if (strcmp(argv[loop],"-dir_type")==0)
            dir_type = argv[loop+1];
         else if (strcmp(argv[loop],"-mom")==0)
            momentum = atoi(argv[loop+1]);
         else if (strcmp(argv[loop],"-spin")==0)
            spin.set(argv[loop+1]);
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
         else if (strcmp(argv[loop],"-channel")==0) {
            int count_tmp = 0;
            do {
               if (loop+count_tmp == argc-1) {
                  printf("\n @@@@@@ Need \"@\" at the end of -hadron option\n\n");
                  return 1;
               }
               count_tmp++;
            } while (strcmp(argv[loop+count_tmp],"@") != 0);
            Nch = count_tmp-1;
            if (channel_type != NULL) delete [] channel_type;
            channel_type = new CHANNEL_TYPE[Nch];
            for (int n=0; n<Nch; n++){
               loop++;
               channel_type[n].set(argv[loop]);
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
   if (  take_JK_flg) use_JK_data = true;
   if (input_fit_flg) out_fit_flg = false;
   
   analysis::Nconf = analysis::set_data_list(conf_list);
   
   printf("\n @ Arguments set :\n");
   printf(" @ conf list  = %s\n",conf_list);
   printf(" @ #. conf    = %d\n",analysis::Nconf);
   printf(" @ t_size     = %d\n",analysis::tSIZE);
   printf(" @ lat space  = %lf\n",lat_space);
   printf(" @ input dir  = %s\n",analysis::data_list[MAIN_PATH]);
   printf(" @ dir type   = %s\n",dir_type.c_str());
   printf(" @ t shift    = %s\n",analysis::data_list[N_T_SHIFT]);
   printf(" @ x shift    = %s\n",analysis::data_list[N_X_SHIFT]);
   printf(" @ y shift    = %s\n",analysis::data_list[N_Y_SHIFT]);
   printf(" @ z shift    = %s\n",analysis::data_list[N_Z_SHIFT]);
   printf(" @ snk rela   = %s\n",analysis::data_list[SNK_RELA]);
   printf(" @ src rela   = %s\n",analysis::data_list[SRC_RELA]);
   printf(" @ #. hadron  = %d\n @ hadron     = { ",Nhad);
   for(int ihad=0; ihad<Nhad; ihad++)
      printf("%s ",hadron_type[ihad].name_only.c_str());
   printf("}\n @ momentum   = %d\n",momentum);
   printf(" @ #. channel = %d\n @ channel    = { ",Nch);
   for(int ich=0; ich<Nch; ich++)
      printf("%s ",channel_type[ich].name.c_str());
   printf("}\n @ spin proj  = %s\n",spin.name.c_str());
   printf(" @ output dir = %s\n @\n",outfile_path);
   printf(" @ calc Rcorr = %s\n",analysis::bool_to_str(calc_flg_Rcorr).c_str());
   printf(" @ input  bin = %s\n",analysis::bool_to_str(input_fit_flg).c_str());
   printf(" @ output bin = %s\n",analysis::bool_to_str(out_fit_flg).c_str());
   printf(" @ use JK data= %s\n",analysis::bool_to_str(use_JK_data).c_str());
   printf(" @ take JK    = %s\n",analysis::bool_to_str(take_JK_flg).c_str());
   printf(" @ calc fit   = %s\n",analysis::bool_to_str(calc_flg_fit).c_str());
   printf(" @ range min  = %d\n",fit_range_min);
   printf(" @ range max  = %d\n\n",fit_range_max);
   
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
      if (     strcmp(tmp_c1,"MAS_Gauge_confs_list"  )==0)
         snprintf(conf_list,sizeof(conf_list),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Path_to_input_dir" )==0)
         analysis::set_data_list(MAIN_PATH, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Input_dir_type" )==0)
         dir_type = tmp_c2;
      else if (strcmp(tmp_c1,"MAS_Path_to_output_dir")==0)
         snprintf(outfile_path,sizeof(outfile_path),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Size_of_time"      )==0)
         analysis::tSIZE = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Momentum"          )==0)
         momentum = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Spin_projection"   )==0)
         spin.set(tmp_c2);
      else if (strcmp(tmp_c1,"MAS_T_shift"           )==0)
         analysis::set_data_list(N_T_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"MAS_X_shift"           )==0)
         analysis::set_data_list(N_X_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Y_shift"           )==0)
         analysis::set_data_list(N_Y_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Z_shift"           )==0)
         analysis::set_data_list(N_Z_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Calc_hadron_name"  )==0) {
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
      else if (strcmp(tmp_c1,"MAS_Calc_channel_name" )==0) {
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
         Nch          = count_tmp;
         channel_type = new CHANNEL_TYPE[Nch];
         for (int n=0; n<Nch; n++){
            channel_type[n].set(tmp_tmp_str[n]);
         }
      }
      else if (strcmp(tmp_c1,"MAS_Snk_relativistic"  )==0)
         analysis::set_data_list(SNK_RELA, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Src_relativistic"  )==0)
         analysis::set_data_list(SRC_RELA, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Calc_Rcorrelator"  )==0)
         calc_flg_Rcorr = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Input_fit_data"    )==0)
         input_fit_flg = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Output_fit_data"   )==0)
         out_fit_flg = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Calc_fit_data"     )==0)
         calc_flg_fit = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Take_jack_knife"    )==0)
         take_JK_flg = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Use_jack_knife_data")==0)
         use_JK_data = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Fit_range_min"     )==0)
         fit_range_min = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Fit_range_max"     )==0)
         fit_range_max = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"MAS_Lattice_spacing"   )==0)
         lat_space = atof(tmp_c2);
      //***************************************************//
      else{
         printf("\n @@@@@@ invalid option \"%s\" in arguments file \"%s\"\n\n"
                , tmp_c1, file_name);
         return 1;
      }
   }
   return 0;
}
