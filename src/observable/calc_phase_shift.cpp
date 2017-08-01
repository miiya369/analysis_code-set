//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup phase shift
 * @brief   Function for calculate phase shift
 * @author  Takaya Miyamoto
 * @since   Fri Nov 13 01:44:42 JST 2015
 */
//--------------------------------------------------------------------------

#include <observable/phase_shift.h>

void phase_shift::calc_phase_shift(T_MATRIX& a_Tmat, PHASE_SHIFT *phase)
{
   int Nch2 = a_Tmat.Nch() * a_Tmat.Nch();
   
   for (int i=0; i<Nch2; i++) phase[i].mem_alloc(a_Tmat);
   
   if      (a_Tmat.Nch() == 1) { // For Single-channel
      for (int iE=0; iE<a_Tmat.data_size(); iE++) {
         cmatrix Smat(a_Tmat.Smat(iE));
         cdouble del  = log(Smat(0)) / (2.0*COMP_IMAG) * 180.0/PI;
         phase[0](iE) = del.real();
//         phase[0](iE) = acos(Smat(0).real());
//         phase[0](iE) = asin(Smat(0).imag());
      }
   }
   else if (a_Tmat.Nch() == 2) { // For 2x2 Coupled-channel
      for (int iE=0; iE<a_Tmat.data_size(); iE++) {
         cmatrix Smat(a_Tmat.Smat(iE));
         cdouble tan2_SYM = - (Smat(0,1) * Smat(1,0)) / (Smat(0,0) * Smat(1,1));
         cdouble cos2_SYM = 1.0 / sqrt(1.0 + tan2_SYM);
         cdouble del1_SYM = log(Smat(0,0)/cos2_SYM) / (2.0*COMP_IMAG) * 180.0/PI;
         cdouble del2_SYM = log(Smat(1,1)/cos2_SYM) / (2.0*COMP_IMAG) * 180.0/PI;
         
         printf("%lf %lf %lf\n", phase[0].E(iE)
                , tan2_SYM.real(), tan2_SYM.imag());   // For Debug
         
         phase[0](iE) = del1_SYM.real();
         phase[1](iE) = del2_SYM.real();
         phase[2](iE) = abs(Smat(0,0)); // = Inelatisity
         phase[3](iE) = atan(tan2_SYM.real()) / 2.0; // = Mixing angle (Debug)
      }
   }
   else {
      ERROR_COMMENTS("The NxN Coupled-channel (N >= 3) calculations have not"
                     " been implemented yet.");
   }
}

void phase_shift::calc_phase_shift_eq(  PHASE_SHIFT &phase, double *aa, double mu
                                      , int Nparam, int func_type
                                      , double lat_space, double del
                                      , double r_max )
{
   DEBUG_LOG
   
   double hc_api = hbar_c / lat_space;
   double   mumu =     mu /    hc_api; //  mu = phys,   mumu = Dim-less
   double deldel =    del / lat_space; // del = phys, deldel = Dim-less
   double rr, vv;
   cdouble kk, tmp, delta;
   
   for (int iE=0; iE<phase.data_size(); iE++) { // E = Phys, kk = Dim-less
      kk    = sqrt( cdouble(2.0 * mu * phase.E(iE)) ) / hbar_c;
      delta = 0.0;
      
      for (double r=del; r<=r_max; r+=del) {
         rr  = r / lat_space;                // r  = phys, rr = Dim-less
         vv  = observable::V(rr, aa, Nparam, func_type); // vv = Dim-less
         
         delta += -(deldel/kk) * (2.0*mumu*vv) * pow(sin(kk*rr + delta), 2);
      } // r
      
      phase(iE) = delta.real() * 180.0/PI;
   } // iE
}
