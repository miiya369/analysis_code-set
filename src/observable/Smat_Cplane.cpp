//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup observable
 * @brief   Function for calculate S matrix on the complex plane
 * @author  Takaya Miyamoto
 * @since   Tue Oct 20 00:22:17 JST 2015
 */
//--------------------------------------------------------------------------

#include <observable/phase_shift.h>
#include <stdlib.h>

cdouble observable::Smat_Cplane(  double Re_k, double Im_k, double *aa, double mu
                                , int Nparam, int func_type
                                , double lat_space, double del, double r_max ) {
   
   string class_name = "________________________________";
   string func_name  = "calc_Smat_Cplane______";
   analysis::route(class_name, func_name, 1);
   
   double hc_api = hbar_c / lat_space;
   
   cdouble k = cdouble(Re_k,Im_k);
   cdouble E = pow(hbar_c,2)/(2.0*mu) * k*k;
   
   cdouble phi[3], phi_diff, Coe, XY;
   phi[0] = COMP_ZERO;   // initialize of wave function
   phi[1] = cdouble(del, del);
   
   for (double r=del; r<=r_max; r+=del) {
      cdouble v;
      v = cdouble( V(r/lat_space, aa, Nparam, func_type) * hc_api );
      phi[2] = 2.0 * ( 1.0 + mu*pow(del/hbar_c,2)*(v-E) ) * phi[1] - phi[0];
      
      phi_diff = (phi[2]-phi[0])/(2.0*del);
      phi[0]   =  phi[1];
      phi[1]   =  phi[2];
      
//      printf("%lf %lf %lf\n", r, phi[0].real(), phi[0].imag());   // For Debug
   } // r
   
   Coe = (phi_diff - phi[0]*k*COMP_IMAG) * 0.5 * exp( COMP_IMAG*k*r_max);
   XY  = (phi_diff + phi[0]*k*COMP_IMAG) * 0.5 * exp(-COMP_IMAG*k*r_max);
   
//   printf("%lf %lf\n", Coe.real(), Coe.imag());   // For Debug
//   printf("%lf %lf\n", XY.real(),  XY.imag());    // For Debug
   
   analysis::route(class_name, func_name, 0);
   return XY/Coe;
}
