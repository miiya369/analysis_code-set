//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup observable
 * @brief   Function for calculate Tmatrix by using differential eq.
 * @author  Takaya Miyamoto
 * @since   Thu Feb  2 18:48:15 JST 2017
 */
//--------------------------------------------------------------------------

#include <observable/Tmatrix.h>
#include <stdlib.h> // for random initialize
#include <time.h>   // for random initialize

inline void print_debug(double, cmatrix, cmatrix); // For Debug

void T_MATRIX::calc_Tmat_dif(  const double *a_params, const double *a_mu
                             , const double *a_mass_threshold
                             , const int    *a_Nparam, const int *a_func_type
                             , const double a_lat_space, const int a_ang_mom
                             , const double a_del, const double a_r_max ) {
   DEBUG_LOG
   
   srand((unsigned int)time(NULL));   // for random initialize
   
   double hc_api = hbar_c / a_lat_space;
   
   double  *Energy   = new  double[(*this).Nch()];
   double  *delta_E  = new  double[(*this).Nch()];
   cdouble *kk       = new cdouble[(*this).Nch()];
   
   cmatrix Atmp    ((*this).Nch());
   cmatrix phi_diff((*this).Nch());
   cmatrix phi[3];
   for (int i=0; i<3; i++) phi[i].init((*this).Nch());
   
   cmatrix JpK((*this).Nch());
   cmatrix JmK((*this).Nch());
   cmatrix rKp((*this).Nch()); rKp.set_unit();
   cmatrix rKm((*this).Nch()); rKm.set_unit();
   cmatrix Smat((*this).Nch());
   
   for (int ich=0; ich<(*this).Nch(); ich++)   // Set the energy threshold
      delta_E[ich] = a_mass_threshold[ich] - a_mass_threshold[0];
   
   for (int iE=0; iE<(*this).data_size(); iE++)
   {
      for (int ich=0; ich<(*this).Nch(); ich++) {
         Energy[ich]  = (*this).E(iE) - delta_E[ich];
         kk[ich]      = sqrt(cdouble(2.0*a_mu[ich]*Energy[ich])) / hbar_c;
         rKp(ich,ich) = sqrt(kk[ich]);
         rKm(ich,ich) = 1.0 / sqrt(kk[ich]);
      }
      phi[0].set_zero(); // initialize for the wave function
      phi[1].set_unit(); phi[1] = (phi[1] * a_del);
      
      for (double r=a_del; r<=a_r_max; r+=a_del) { // Solve the Schrodinger eq.
         
         /// Set the potentials ///
         int tmpN = 0;
         for (   int ich =0; ich <(*this).Nch(); ich++ )
            for (int isrc=0; isrc<(*this).Nch(); isrc++) {
               int i = isrc + (*this).Nch() * ich;
               Atmp(ich,isrc) = ( observable::V(r/a_lat_space, &a_params[tmpN]
                                                , a_Nparam[i], a_func_type[i])
                                 ) * hc_api * (2.0*a_mu[ich]);
               tmpN += a_Nparam[i];
               
               if (ich == isrc)
                  Atmp(ich,isrc) += (a_ang_mom * (a_ang_mom-1) / (r*r) -
                                     (2.0*a_mu[ich]) * (Energy[ich]));
            }
         
         phi[2]   = (2.0 + pow(a_del/hbar_c, 2) * Atmp) * phi[1] - phi[0];
         phi_diff = (phi[2]-phi[0]) / (2.0*a_del);
         phi[0]   =  phi[1];
         phi[1]   =  phi[2];
//         print_debug(r, phi[0], phi_diff); // For Debug
      }
      for (   int ich =0; ich <(*this).Nch(); ich++ )
         for (int isrc=0; isrc<(*this).Nch(); isrc++) {
            JpK(ich,isrc) = ((  phi_diff(ich,isrc)
                              - phi  [0](ich,isrc)*kk[ich]*COMP_IMAG)
                             * exp( COMP_IMAG*kk[ich]*a_r_max-a_ang_mom*PI/2.0));
            JmK(ich,isrc) = ((  phi_diff(ich,isrc)
                              + phi  [0](ich,isrc)*kk[ich]*COMP_IMAG)
                             * exp(-COMP_IMAG*kk[ich]*a_r_max-a_ang_mom*PI/2.0));
         } // Construct Jost functions and S-matrix
      
      Smat = rKm * JmK * JpK.inverce() * rKp;
      
      // Unitarity check (within 1% is OK)
      int check_count = 0;
      for (int ich=0; ich<(*this).Nch(); ich++)
         if (Energy[ich] > 0) check_count++;
      
      cmatrix check_unitary_Smat(check_count);
      for (   int ich =0; ich <check_count; ich++ )
         for (int isrc=0; isrc<check_count; isrc++)
            check_unitary_Smat(ich, isrc) = Smat(ich, isrc);
      
      cmatrix check_unitary(check_count);
      check_unitary = check_unitary_Smat * check_unitary_Smat.dagger();
      double check_unitary_d;
      for (   int ich =0; ich <check_count; ich++ )
         for (int isrc=0; isrc<check_count; isrc++) {
            ich == isrc ? check_unitary_d = 1.0 : check_unitary_d = 0.0;
               if (fabs(abs(check_unitary(ich, isrc)) - check_unitary_d) > 0.01)
                  printf("WARNING: S-matrix may not be unitary, "
                         "|S_{%d,%d}(%lf)| = %lf\n"
                         , ich, isrc, (*this).E(iE)
                         , abs(check_unitary(ich, isrc)));
         }
      (*this)(iE) = (Smat - 1.0) / (2.0 * COMP_IMAG); // S = 1 + 2iT
      
//      print_debug((*this).E(iE), Smat, Smat * Smat.dagger()); // For Debug
      
//      exit(1); /////////////////////////////
   } // iE  
//   exit(1); /////////////////////////////
   
   delete [] Energy;
   delete [] delta_E;
   delete [] kk;
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
