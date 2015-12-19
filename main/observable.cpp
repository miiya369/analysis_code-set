//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup observable
 * @brief   Main part for observable
 * @author  Takaya Miyamoto
 * @since   Thu Jul 23 18:04:09 JST 2015
 */
//--------------------------------------------------------------------------

#include <observable/phase_shift.h>
#include <observable/scattering_length.h>

#define PROJECT CALC_OBSERVABLE   // <- Project name

int  analysis::Nconf;
int  analysis::xSIZE;
int  analysis::ySIZE;
int  analysis::zSIZE;
int  analysis::tSIZE;
char analysis::data_list[MAX_N_DATA][MAX_LEN_PATH];

double lattice_space;
double mass;
double ang_mom;
int    E_min;
int    E_max;
double delta;
double max_r;
double V_0;
double r_0;

char infile_path[MAX_LEN_PATH];
char outfile_path[MAX_LEN_PATH];

bool test_flg           = false;
bool calc_scatt_len_flg = false;

bool arguments_check = false;
int  set_args(int, char**);
int  set_args_from_file(char*);

//========================================================================//
int main(int argc, char **argv) {
   if (set_args(argc, argv) == 1) return 0;
   
   time_t start_time, end_time;
   time( &start_time );
   
   PHASE_SHIFT       *phase = new PHASE_SHIFT;
   SCATTERING_LENGTH *scatt = NULL;
   char scatt_length_filename[1024];
   
   if (calc_scatt_len_flg) scatt = new SCATTERING_LENGTH;
   
   if (test_flg)
      phase->calc_phase_shift_test( r_0, V_0, mass, delta, max_r, E_min, E_max );
   else {
      phase->input_param( infile_path );
      phase->calc_phase_shift(  lattice_space, mass, ang_mom, delta
                              , max_r, E_min, E_max );
   }
   phase->output_phase_shift_err( outfile_path );
   
   if (calc_scatt_len_flg) {
      snprintf(scatt_length_filename, sizeof(scatt_length_filename)
               ,"%s_scattering_length",outfile_path);
      scatt->input_phase_shift( phase->phase_shift, mass, E_min, E_max );
      scatt->calc_scatt_len();
      scatt->output_scatt_len_err( scatt_length_filename );
   }
   delete phase;
   if (calc_scatt_len_flg) delete scatt;
   
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
         else if (strcmp(argv[loop],"-ifile")==0)
            snprintf(infile_path,sizeof(infile_path),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-ofile")==0)
            snprintf(outfile_path,sizeof(outfile_path),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-mass" )==0)
            mass = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-test" )==0)
            test_flg = true;
         else if (strcmp(argv[loop],"-V0"   )==0)
            V_0 = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-r0"   )==0)
            r_0 = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-check")==0)
            arguments_check = true;
         //***************************************************//
         else {
            printf("\n @@@@@@ invalid option \"%s\"\n", argv[loop]);
            analysis::usage(PROJECT);
            return 1;
         }
      }
   }
   
   printf("\n @ Arguments set :\n");
   printf(" @ lat space  = %lf\n",lattice_space);
   printf(" @ mass       = %lf\n",mass);
   printf(" @ ang mom    = %lf\n",ang_mom);
   printf(" @ E min      = %d\n",E_min);
   printf(" @ E max      = %d\n",E_max);
   printf(" @ delta      = %lf\n",delta);
   printf(" @ max_r      = %lf\n",max_r);
   printf(" @ infile     = %s\n",infile_path);
   printf(" @ outfile    = %s\n @\n",outfile_path);
   printf(" @ calc test      = %s\n",analysis::bool_to_str(test_flg).c_str());
   printf(" @ V_0 (For test) = %lf\n",V_0);
   printf(" @ r_0 (For test) = %lf\n @\n",r_0);
   printf(" @ calc sctt len  = %s\n\n"
          ,analysis::bool_to_str(calc_scatt_len_flg).c_str());
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
      if (     strcmp(tmp_c1,"OBS_Parameter_list")==0)
         snprintf(infile_path,sizeof(infile_path),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Output_file_name"     )==0)
         snprintf(outfile_path,sizeof(outfile_path),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Lattice_spacing"     )==0)
         lattice_space = atof(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Reduced_mass"     )==0)
         mass = atof(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Angular_momentum")==0)
         ang_mom = atof(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Calc_energy_min"  )==0)
         E_min = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Calc_energy_max")==0     )
         E_max = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Runge_kutta_delta"     )==0)
         delta = atof(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Runge_kutta_max_r"     )==0)
         max_r = atof(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Calc_squ_wall_pot"     )==0)
         test_flg = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Squ_wall_pot_V0")==0)
         V_0 = atof(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Squ_wall_pot_r0"  )==0)
         r_0 = atof(tmp_c2);
      else if (strcmp(tmp_c1,"OBS_Calc_scatt_length"  )==0)
         calc_scatt_len_flg = analysis::str_to_bool(tmp_c2);
      //***************************************************//
      else{
         printf("\n @@@@@@ invalid option \"%s\" in arguments file \"%s\"\n\n"
                , tmp_c1, file_name);
         return 1;
      }
   }
   return 0;
}
