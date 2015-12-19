//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Calculate the tensor potential
 * @author  Takaya Miyamoto
 * @since   Wed Dec 16 06:20:44 JST 2015
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>

void potential::tensor_potential(  POTENTIAL &V_C, POTENTIAL &V_T
                                 , R_CORRELATOR     &Rcorr
                                 , R_CORRELATOR &S12_Rcorr
                                 , R_CORRELATOR   &K_Rcorr ) {
   
   V_C.mem_alloc();
   V_T.mem_alloc();
   
   R_CORRELATOR Rcorr_bar, S12_Rcorr_bar, K_Rcorr_bar;
   
   Rcorrelator::Swave_division(     Rcorr,     Rcorr_bar );
   Rcorrelator::Swave_division( S12_Rcorr, S12_Rcorr_bar );
   Rcorrelator::Swave_division(   K_Rcorr,   K_Rcorr_bar );
   
   cdouble det;
   size_t  xyzSIZE = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   
   for (size_t xyz=0; xyz<xyzSIZE; xyz++) {
      
      det = Rcorr(xyz)*S12_Rcorr_bar(xyz) - S12_Rcorr(xyz)*Rcorr_bar(xyz);
      
      V_C(xyz) = (  S12_Rcorr_bar(xyz) * K_Rcorr(xyz)
                  - S12_Rcorr(xyz)     * K_Rcorr_bar(xyz) ) / det;
      
      V_T(xyz) = (  K_Rcorr_bar(xyz) * Rcorr(xyz)
                  - K_Rcorr(xyz)     * Rcorr_bar(xyz) ) / det;
   }
}
