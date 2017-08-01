//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup observable
 * @brief   Function for calculate Tmatrix by using differential eq.
 * @author  Takaya Miyamoto
 * @since   Fri Nov 13 01:44:42 JST 2015
 */
//--------------------------------------------------------------------------

#include <observable/Tmatrix.h>
#include <potential/folding_potential.h>

//--------------------------------------------------------------------------
/**
 * @brief T-matrix calculation for folding-potential (Single-channel)
 */
//--------------------------------------------------------------------------
void T_MATRIX::calc_Tmat_fpot(  const DENSITY_TYPE_BASE &a_density
                              , const double *a_params, const double a_mu
                              , const int a_Nparam, const int a_func_type
                              , const double a_lat_space, const double a_del
                              , const double a_r_max, const double a_Ndiv ) {
   DEBUG_LOG
   
   if ((*this).Nch() != 1) { // Now, This code can use only #.channel = 1.
      ERROR_COMMENTS("T-matrix calculation for folding-potential has not "
                     "been implemented for #.channel >= 2.");
   }
   if (a_func_type != 3 && // Now, This code can use only Gaussian-type potentials.
       a_func_type != 4 &&
       a_func_type != 5 &&
       a_func_type != 8) {
      ERROR_COMMENTS("ONLY Gaussian-type fitted potentials are compatible.");
   }
   
   double hc_api    = hbar_c / a_lat_space;
   int    max_r_idx = int(a_r_max / a_del);
   double *Vtmp     = new double[max_r_idx];
   
   cdouble Atmp, kk, phi_diff, phi[3];
   cdouble Coe, XY, Smat;
   
#ifdef _OPENMP
#pragma omp parallel for
#endif
   for (int ir=0; ir<max_r_idx; ir++) { // Set the potentials
      double r = (ir+1) * a_del;
      Vtmp[ir] = fpotential_gauss::fpot_r(  r/a_lat_space, a_params, a_Nparam
                                          , a_density, a_Ndiv ) * hc_api;
      // For Debug
      // observable::V(r/a_lat_space, a_params, a_Nparam, a_func_type) * hc_api;
   }
   double r_max = max_r_idx * a_del;
   
   for (int iE=0; iE<(*this).data_size(); iE++)
   {
      kk = sqrt( cdouble(2.0*a_mu*(*this).E(iE)) ) / hbar_c;
      
      phi[0] = COMP_ZERO; // initialize for the wave function
      phi[1] = cdouble(  a_del*rand()/(double)RAND_MAX
                       , a_del*rand()/(double)RAND_MAX );
      
      for (int ir=0; ir<max_r_idx; ir++) { // Solve the Schrodinger eq.
         Atmp     = (2.0*a_mu) * (Vtmp[ir] - (*this).E(iE));
         phi[2]   = (2.0 + pow(a_del/hbar_c, 2) * Atmp) * phi[1] - phi[0];
         phi_diff = (phi[2]-phi[0]) / (2.0*a_del);
         phi[0]   = phi[1];
         phi[1]   = phi[2];
//         print_debug(r, phi[0], phi_diff); // For Debug;
      }
      Coe  = (phi_diff - phi[0]*kk*COMP_IMAG) * exp( COMP_IMAG*kk*r_max);
      XY   = (phi_diff + phi[0]*kk*COMP_IMAG) * exp(-COMP_IMAG*kk*r_max);
      Smat = XY / Coe; // Construct S-matrix
      
      (*this)(iE)(0) = (Smat - 1.0) / (2.0 * COMP_IMAG); // S = 1 + 2iT
   
   } // iE
      
   delete [] Vtmp;
}

inline void print_debug(double a_r, cmatrix a_data1, cmatrix a_data2) // For Debug
{
   printf("%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n"
          , a_r
          , a_data1(0,0).real(), a_data1(0,0).imag()
          , a_data1(0,1).real(), a_data1(0,1).imag()
          , a_data1(1,0).real(), a_data1(1,0).imag()
          , a_data1(1,1).real(), a_data1(1,1).imag()
          , a_data2(0,0).real(), a_data2(0,0).imag()
          , a_data2(0,1).real(), a_data2(0,1).imag()
          , a_data2(1,0).real(), a_data2(1,0).imag()
          , a_data2(1,1).real(), a_data2(1,1).imag() );
}
