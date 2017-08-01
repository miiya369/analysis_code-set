//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup potential
 * @brief   Main part for calculate eigenvalue and eigenvector 
 * @brief   of schrodinger equation by using ARPACK
 * @author  Takaya Miyamoto
 * @since   Mon Jan 30 09:16:15 JST 2017
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>
#include <common/matrix_func.h>

#define PROJECT EIGEN_ARPACK   // <- Project name

static CHANNEL_TYPE channel;
static int          time_slice;
static SPIN_TYPE    spin;
static double       HAD1_mass;
static double       HAD2_mass;
static double       lat_a;
static double       reduced_mass;

static int  Lsize_sch;
static int  Vsize;
static int  Neigen;
static int *nghbrs;

static char conf_list[MAX_LEN_PATH];
static char outfile_path[MAX_LEN_PATH];

static bool take_JK_flg   = false;
static bool use_JK_data   = false;

static bool arguments_check = false;
static int  set_args(int, char**);
static int  set_args_from_file(char*);

extern "C" {
// The ARPACK subroutine to compute eigenvalues //
   extern void dsaupd_(int *IDO, char *BMAT, int *Ndim, char *WHICH,
                       int *NEV, double *TOL, double *RESID, int *NCV,
                       double *Vout, int *LDV, int *IPARAM, int *IPNTR,
                       double *WORKD, double *WORKL, int *LWORKL,
                       int *INFO);
   
   static char   Bmat [2] = "I";
   static char   Which[3] = "SA";
   static char   a_ALL[4] = "All";
   static double Tol      =  0.0; // -> 0.0 corresponds to machine precision
   static int Ncv;
   static int Ldv;
   static int Lworkl;
   
// The ARPACK subroutine to compute eigenvectors for the computed eigenvalues //
   extern void dseupd_(int *RVEC, char *All, int *SELECT, double *Evec,
                       double *V1, int *LDV1, double *SIGMA,
                       char *BMAT, int *Ndim, char *WHICH, int *NEV,
                       double *TOL, double *RESID, int *NCV, double *V2,
                       int *LDV2, int *IPARAM, int *IPNTR, double *WORKD,
                       double *WORKL, int *LWORKL, int *IERR);
}

static void Hpsi     (double*, double*, double* );
static int  solve_sch(double*, double*, double**);

//========================================================================//
int main(int argc, char **argv)
{
   if (set_args(argc, argv) == 1) return 0;
   
   time_t start_time, end_time;
   time(&start_time);
   
   /// Input 2pt-correlators ///
   
   CONFIG<CORRELATOR> *Corr1 = new CONFIG<CORRELATOR>;
   CONFIG<CORRELATOR> *Corr2 = new CONFIG<CORRELATOR>;
   
   for (int iconf=0; iconf<analysis::Nconf; iconf++) {
      (*Corr1)(iconf).set(channel.hadron1, iconf, 0, "PS");
      (*Corr2)(iconf).set(channel.hadron2, iconf, 0, "PS");
   }
   if (take_JK_flg) {
      Corr1 -> make_JK_sample();
      Corr2 -> make_JK_sample();
   }
   
   /// Input NBS wave functions & Construct R-correlators ///
   
   NBSwave::rot_matrix_init();
   
   NBS_WAVE_ORG         *Wave_org = NULL;
   CONFIG<NBS_WAVE>     *Wave     = new CONFIG<NBS_WAVE>;
   CONFIG<R_CORRELATOR> *Rcorr    = new CONFIG<R_CORRELATOR>[3];
   
   if (channel.flg_spin_proj) spin.set(channel.spin_name);
   else
      Wave_org = new NBS_WAVE_ORG;
   
   int count = 0;
   for (int it=time_slice-1; it<=time_slice+1; it++) {
      for (int iconf=0; iconf<analysis::Nconf; iconf++) {
         if (channel.flg_spin_proj) (*Wave)(iconf).set(channel, it, iconf);
         else {
            Wave_org -> set(channel, it, iconf);
            NBSwave::spin_projection(*Wave_org, (*Wave)(iconf), spin);
         }
         NBSwave::LP_projection((*Wave)(iconf));
      }
      if (take_JK_flg) Wave -> make_JK_sample();
      
      for (int iconf=0; iconf<analysis::Nconf; iconf++)
         Rcorr[count](iconf).set( (*Wave )(iconf)
                                 ,(*Corr1)(iconf), (*Corr2)(iconf), it);
      count++;
   }
   
   /// Construct the potentials ///
   
   CONFIG<R_CORRELATOR> *K_Rcorr   = new CONFIG<R_CORRELATOR>;
   CONFIG<POTENTIAL>    *potential = new CONFIG<POTENTIAL>;
   
   reduced_mass = HAD1_mass*HAD2_mass / (HAD1_mass+HAD2_mass);
   
   for (int conf=0; conf<analysis::Nconf; conf++) {
      potential::kernel((*K_Rcorr)(conf)
                        , Rcorr[0](conf), Rcorr[1](conf), Rcorr[2](conf)
                        , HAD1_mass, HAD2_mass );
      (*potential)(conf).set((*K_Rcorr)(conf), Rcorr[1](conf));
   }
   
   if (Wave_org != NULL) delete Wave_org;
   delete Wave;
   delete Corr1;
   delete Corr2;
   delete [] Rcorr;
   delete K_Rcorr;
   
   /// Calculate the eigenvalues & eigenvectors ///
   
   Vsize = pow(Lsize_sch, 3);
   
   Ncv    = 4 * Neigen;   if (Ncv > Vsize) Ncv = Vsize;
   Ldv    = Vsize;
   Lworkl = Ncv * (Ncv+8);   // Refer to USER GUIDE of ARPACK.
   
   double *Eenargy = new double[Neigen * analysis::Nconf];
   Wave = new CONFIG<NBS_WAVE>[Neigen];
   
   // allocate and compute neighbours array //
   nghbrs = new int[6*Vsize];
   int idx, ipx, ipy, ipz, imx, imy, imz, Nx = Lsize_sch;
   for (      int iz = 0; iz < Nx; iz++)
      for (   int iy = 0; iy < Nx; iy++)
         for (int ix = 0; ix < Nx; ix++) {
            idx =  ix          + Nx*( iy          + Nx*( iz         ));
            ipx = (ix+1   )%Nx + Nx*( iy          + Nx*( iz         ));
            ipy =  ix          + Nx*((iy+1   )%Nx + Nx*( iz         ));
            ipz =  ix          + Nx*( iy          + Nx*((iz+1   )%Nx));
            imx = (ix-1+Nx)%Nx + Nx*( iy          + Nx*( iz         ));
            imy =  ix          + Nx*((iy-1+Nx)%Nx + Nx*( iz         ));
            imz =  ix          + Nx*( iy          + Nx*((iz-1+Nx)%Nx));
            nghbrs[6*idx+0] = ipx;
            nghbrs[6*idx+1] = imx;
            nghbrs[6*idx+2] = ipy;
            nghbrs[6*idx+3] = imy;
            nghbrs[6*idx+4] = ipz;
            nghbrs[6*idx+5] = imz;
         }
   
   int counter = 0, run_OK = 0;
   printf(" @ eigen energy calculating |   0%%");
   fflush(stdout);
//#ifdef _OPENMP
//#pragma omp parallel for   // OpenMP cannot be used for ARPACK ??
//#endif
   for (int iconf=0; iconf<analysis::Nconf; iconf++) {
      double  *pot  = new double [Vsize];
      double **evec = new double*[Vsize];
      double  *eval = new double [Neigen];
      for (      int iz = -Nx/2; iz < Nx/2; iz++)
         for (   int iy = -Nx/2; iy < Nx/2; iy++)
            for (int ix = -Nx/2; ix < Nx/2; ix++) {
               int iX = ix < 0 ? ix + Nx : ix;
               int iY = iy < 0 ? iy + Nx : iy;
               int iZ = iz < 0 ? iz + Nx : iz;
               int in = iX + Nx*( iY + Nx* iZ );
               if (abs(ix) < analysis::xSIZE/2 &&
                   abs(iy) < analysis::ySIZE/2 &&
                   abs(iz) < analysis::zSIZE/2  ) {
                  int iXX = ix < 0 ? ix + analysis::xSIZE : ix;
                  int iYY = iy < 0 ? iy + analysis::ySIZE : iy;
                  int iZZ = iz < 0 ? iz + analysis::zSIZE : iz;
                  int inn = iXX + analysis::xSIZE*( iYY + analysis::ySIZE* iZZ );
                  pot [in] = (*potential)(iconf)(inn).real();
               }
               else
                  pot [in] = 0.0;
               
               evec[in] = new double[Neigen];
            }
      if (solve_sch(pot, eval, evec) != 0) run_OK = 1;
      
      for (int i=0; i<Neigen; i++) {
         Eenargy[i+Neigen*iconf] = eval[i];
         Wave[i](iconf).mem_alloc();
         
         for (      int iz = -analysis::zSIZE/2; iz < analysis::zSIZE/2; iz++)
            for (   int iy = -analysis::ySIZE/2; iy < analysis::ySIZE/2; iy++)
               for (int ix = -analysis::xSIZE/2; ix < analysis::xSIZE/2; ix++) {
                  int iX  = ix < 0 ? ix +              Nx : ix;
                  int iY  = iy < 0 ? iy +              Nx : iy;
                  int iZ  = iz < 0 ? iz +              Nx : iz;
                  int iXX = ix < 0 ? ix + analysis::xSIZE : ix;
                  int iYY = iy < 0 ? iy + analysis::ySIZE : iy;
                  int iZZ = iz < 0 ? iz + analysis::zSIZE : iz;
                  int in  = iX  +              Nx*( iY  +              Nx* iZ  );
                  int inn = iXX + analysis::xSIZE*( iYY + analysis::ySIZE* iZZ );
                  Wave[i](iconf)(inn) = evec[in][i] / evec[0][i];
               }
         NBSwave::LP_projection(Wave[i](iconf));
      }
      for (int i=0; i<Vsize; i++) delete [] evec[i];
      delete [] pot;
      delete [] eval;
      
      counter++;
      printf("\b\b\b\b%3.0lf%%",double(counter)/double(analysis::Nconf)*100);
      fflush(stdout);
   } // iconf
   
   printf("\n");
   delete potential;
   delete nghbrs;
   
   if (run_OK != 0) return 1;
   
   /// Output the eigenvalues & eigenvectors ///
   
   for (int i=0; i<Neigen; i++) {
      char outfile_name[MAX_LEN_PATH];
      snprintf(  outfile_name, sizeof(outfile_name)
               , "%s/%s_%s_Evec_level%02d_err_t%03d"
               , outfile_path, channel.name.c_str(), spin.name.c_str()
               , i, time_slice );
      
      analysis::output_data_err(Wave[i], outfile_name, use_JK_data);
   }
   delete [] Wave;
   
   double *err  = new double[Neigen];
   double *mean = new double[Neigen];
   double factor;
   if (use_JK_data)
      factor = double(analysis::Nconf-1);
   else
      factor = 1.0 / double(analysis::Nconf-1);
   
   printf("\n"
          " @ ====================================================== @\n");
   printf(" @    RESULTS OF EIGEN ENERGY FOR SCHRODINGER EQUATION    @\n");
   printf(" @ ====================================================== @\n"
          "\n");
   printf(" [dimension-less]\n");
   for (int i=0; i<Neigen; i++) {
             mean[i]  = 0.0;
      double sqr_mean = 0.0;
      for (int conf=0; conf<analysis::Nconf; conf++) {
         mean[i]  +=      Eenargy[i+conf*Neigen];
         sqr_mean += pow( Eenargy[i+conf*Neigen], 2 );
      }
      mean[i]  /= double(analysis::Nconf);
      sqr_mean /= double(analysis::Nconf);
      err[i]    = sqrt(factor * (sqr_mean-pow(mean[i],2)) );
      printf("%9d %18.9lf +/- %12.9lf\n", i, mean[i], err[i]);
   }
   printf(" [MeV]\n");
   for (int i=0; i<Neigen; i++) {
      mean[i]  = 0.0;
      double sqr_mean = 0.0;
      for (int conf=0; conf<analysis::Nconf; conf++) {
         mean[i]  +=      Eenargy[i+conf*Neigen] * hbar_c / lat_a;
         sqr_mean += pow( Eenargy[i+conf*Neigen] * hbar_c / lat_a, 2 );
      }
      mean[i]  /= double(analysis::Nconf);
      sqr_mean /= double(analysis::Nconf);
      err[i]    = sqrt(factor * (sqr_mean-pow(mean[i],2)) );
      printf("%9d %18.6lf +/- %12.6lf\n", i, mean[i], err[i]);
   }
   printf("\n @ ====================================================== @\n");
   
   delete [] err;
   delete [] mean;
   delete [] Eenargy;
   
   time(&end_time);
   printf("\n @ JOB END : ELAPSED TIME [s] = %d\n\n"
          ,(int)difftime(end_time,start_time) );
   
   return 0;
}
//========================================================================//

// Following two functions are refered to bruno-san's code //
static void Hpsi(double *a_pot, double *in, double *out)
{
   /* out = H.in */
   for (int i=0; i<Vsize; ++i) {
      out[i] = 6 * in[i];
      for (int mu=0; mu<6; ++mu) {
         out[i] -= in[nghbrs[6*i+mu]];
      }
      out[i] /= 2 * reduced_mass;
   }
   
   /* out += pot.in */
   for (int i=0; i<Vsize; ++i) {
      out[i] += a_pot[i] * in[i];
   }
}
static int solve_sch(double* a_pot, double* a_eval, double** a_evec)
{
   int Ido = 0; // Initialization of the reverse communication parameter.
   
   double *Resid = new double[Vsize];
   double *VOut  = new double[Ncv * Ldv];
   
   int Iparam[11];      /* An array used to pass information to the routines
                           about their functional modes. */
   Iparam[0] = 1;       // <- Specifies the shift strategy (1->exact)
   Iparam[2] = 3*Vsize; // <- Maximum number of iterations
   Iparam[6] = 1;       /* Sets the mode of dsaupd.
                           1 is exact shifting (regular mode),
                           2 is user-supplied shifts,
                           3 is shift-invert mode,
                           4 is buckling mode,
                           5 is Cayley mode. */
   
   int Ipntr[11]; /* Indicates the locations in the work array workd
                     where the input and output vectors in the
                     callback routine are located. */
   
   double *Workd = new double[3*Vsize];
   double *Workl = new double[Lworkl];
   
   int Info = 0;  // Passes convergence information out of the iteration routine.
   int Rvec = 1;  /* Specifies that eigenvectors should be calculated.
                     It acts strangely if I ask not to calculate them. */
   
   int    *Select = new int   [Ncv];
   double *Eval   = new double[2*Ncv]; /* This vector will return the eigenvalues
                                          from the second routine, dseupd. */
   
   double Sigma; // Be not referenced if Iparam[6] = 1.
   int    Ierr;
   
   /* Here we enter the main loop where the calculations are
    performed.  The communication parameter ido tells us when
    the desired tolerance is reached, and at that point we exit
    and extract the solutions. */
   
   do {
      dsaupd_(&Ido, Bmat, &Vsize, Which, &Neigen, &Tol, Resid,
              &Ncv, VOut, &Ldv, Iparam, Ipntr, Workd, Workl,
              &Lworkl, &Info);
      
      if ((Ido==1)||(Ido==-1))
         Hpsi(a_pot, &Workd[Ipntr[0]-1], &Workd[Ipntr[1]-1]);
   
   } while ((Ido==1)||(Ido==-1));
   
   /* From those results, the eigenvalues and vectors are extracted. */
   
   if (Info < 0) {
      printf("Error with dsaupd, info = %d\n", Info);
      printf("Check documentation in dsaupd\n\n");
      return 1;
   } else {
      dseupd_(&Rvec, a_ALL, Select, Eval, VOut, &Ldv, &Sigma, Bmat,
              &Vsize, Which, &Neigen, &Tol, Resid, &Ncv, VOut, &Ldv,
              Iparam, Ipntr, Workd, Workl, &Lworkl, &Ierr);
      
      if (Ierr!=0) {
         printf("Error with dseupd, info = %d\n", Ierr);
         printf("Check documentation in dseupd.\n\n");
         return 1;
      } else if (Info==1) {
         printf("Maximum number of iterations reached.\n\n");
         return 1;
      } else if (Info==3) {
         printf("No shifts could be applied during implicit\n");
         printf("Arnoldi update, try increasing Ncv.\n\n");
         return 1;
      }
      
      /* Before exiting, we copy the solution information over to
       the arrays of the calling program, then clean up the
       memory used by this routine. */
      
      for (int i=0; i<Neigen; i++) {
         a_eval[i] = Eval[i];
         for (int j=0; j<Vsize; j++) a_evec[j][i] = VOut[i*Vsize+j];
      }
      
      delete [] Resid;
      delete [] VOut;
      delete [] Workd;
      delete [] Workl;
      delete [] Select;
      delete [] Eval;
   }
   return 0;
}
//========================================================================//

int set_args(int argc, char** argv)
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
         if (     strcmp(argv[loop],"-f"        )==0){}
         //****** You may set additional potion in here ******//
         else if (strcmp(argv[loop],"-idir"     )==0)
            analysis::set_data_list(MAIN_PATH, "%s", argv[loop+1]);
         else if (strcmp(argv[loop],"-time"    )==0)
            time_slice = atoi(argv[loop+1]);
         else if (strcmp(argv[loop],"-ofile"  )==0)
            snprintf(outfile_path,sizeof(outfile_path),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-conf_list" )==0)
            snprintf(conf_list,sizeof(conf_list),"%s",argv[loop+1]);
         else if (strcmp(argv[loop],"-spin")==0)
            spin.set(argv[loop+1]);
         else if (strcmp(argv[loop],"-mass_had1")==0)
            HAD1_mass = atof(argv[loop+1]);
         else if (strcmp(argv[loop],"-mass_had2")==0)
            HAD2_mass = atof(argv[loop+1]);
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
   printf(" @ Lsize pot  = %d\n",analysis::xSIZE);
   printf(" @ Lsize sch  = %d\n",Lsize_sch);
   printf(" @ lat space  = %lf\n",lat_a);
   printf(" @ input dir  = %s\n",analysis::data_list[MAIN_PATH]);
   printf(" @ t shift    = %s\n",analysis::data_list[N_T_SHIFT]);
   printf(" @ x shift    = %s\n",analysis::data_list[N_X_SHIFT]);
   printf(" @ y shift    = %s\n",analysis::data_list[N_Y_SHIFT]);
   printf(" @ z shift    = %s\n",analysis::data_list[N_Z_SHIFT]);
   printf(" @ snk rela   = %s\n",analysis::data_list[SNK_RELA]);
   printf(" @ src rela   = %s\n",analysis::data_list[SRC_RELA]);
   printf(" @ channel    = %s\n",channel.name.c_str());
   printf(" @ output dir = %s\n @\n",outfile_path);
   printf(" @ time slice = %d\n",time_slice);
   printf(" @ HAD1 mass  = %lf\n",HAD1_mass);
   printf(" @ HAD2 mass  = %lf\n",HAD2_mass);
   printf(" @ spin       = %s\n",spin.name.c_str());
   printf(" @ #.eigen E  = %d\n",Neigen);
   printf(" @ use JK data= %s\n",analysis::bool_to_str(use_JK_data).c_str());
   printf(" @ take JK    = %s\n\n",analysis::bool_to_str(take_JK_flg).c_str());
   fflush(stdout);
   
   if (arguments_check) return 1;
   return 0;
}

int set_args_from_file(char* file_name)
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
      if (     strcmp(tmp_c1,"ARP_Size_of_space_for_pot")==0){
         analysis::xSIZE = atoi(tmp_c2);
         analysis::ySIZE = atoi(tmp_c2);
         analysis::zSIZE = atoi(tmp_c2);
      }
      else if (strcmp(tmp_c1,"ARP_Size_of_space_for_sch")==0)
         Lsize_sch = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"ARP_Size_of_time"      )==0)
         analysis::tSIZE = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"ARP_calc_time_slice"   )==0)
         time_slice = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"ARP_Spin_projection"   )==0)
         spin.set(tmp_c2);
      else if (strcmp(tmp_c1,"ARP_Gauge_confs_list"  )==0)
         snprintf(conf_list,sizeof(conf_list),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"ARP_Path_to_input_dir" )==0)
         analysis::set_data_list(MAIN_PATH, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"ARP_Path_to_output_dir")==0)
         snprintf(outfile_path,sizeof(outfile_path),"%s",tmp_c2);
      else if (strcmp(tmp_c1,"ARP_T_shift"           )==0)
         analysis::set_data_list(N_T_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"ARP_X_shift"           )==0)
         analysis::set_data_list(N_X_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"ARP_Y_shift"           )==0)
         analysis::set_data_list(N_Y_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"ARP_Z_shift"           )==0)
         analysis::set_data_list(N_Z_SHIFT, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"ARP_Snk_relativistic"  )==0)
         analysis::set_data_list(SNK_RELA, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"ARP_Src_relativistic"  )==0)
         analysis::set_data_list(SRC_RELA, "%s", tmp_c2);
      else if (strcmp(tmp_c1,"ARP_Lattice_spacing"   )==0)
         lat_a = atof(tmp_c2);
      else if (strcmp(tmp_c1,"ARP_Number_of_eigen_energy" )==0)
         Neigen = atoi(tmp_c2);
      else if (strcmp(tmp_c1,"ARP_Had1_mass"         )==0)
         HAD1_mass = atof(tmp_c2);
      else if (strcmp(tmp_c1,"ARP_Had2_mass"         )==0)
         HAD2_mass = atof(tmp_c2);
      else if (strcmp(tmp_c1,"ARP_Take_jack_knife"    )==0)
         take_JK_flg = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"ARP_Use_jack_knife_data")==0)
         use_JK_data = analysis::str_to_bool(tmp_c2);
      else if (strcmp(tmp_c1,"ARP_Channel_name"      )==0)
         channel.set(tmp_c2);
      //***************************************************//
      else{
         printf("\n @@@@@@ invalid option \"%s\" in arguments file \"%s\"\n\n"
                , tmp_c1, file_name);
         return 1;
      }
   }
   return 0;
}
