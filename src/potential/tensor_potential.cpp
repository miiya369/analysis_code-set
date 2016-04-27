//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Calculate the tensor potential
 * @author  Takaya Miyamoto
 * @since   Tue Feb  2 18:57:38 JST 2016
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>

void potential::tensor_potential(  POTENTIAL &V_C, POTENTIAL &V_T
                                 , R_CORRELATOR     &Rcorr
                                 , R_CORRELATOR &S12_Rcorr
                                 , R_CORRELATOR   &K_Rcorr )
{
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

void potential::tensor_potential(  POTENTIAL &V_C, POTENTIAL &V_T
                                 , const R_CORRELATOR     &S_Rcorr
                                 , const R_CORRELATOR &S_S12_Rcorr
                                 , const R_CORRELATOR   &S_K_Rcorr
                                 , const R_CORRELATOR     &D_Rcorr
                                 , const R_CORRELATOR &D_S12_Rcorr
                                 , const R_CORRELATOR   &D_K_Rcorr )
{
   V_C.mem_alloc();
   V_T.mem_alloc();
   
   cdouble det;
   size_t  xyzSIZE = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   
   for (size_t xyz=0; xyz<xyzSIZE; xyz++) {
      
      det = S_Rcorr(xyz)*D_S12_Rcorr(xyz) - S_S12_Rcorr(xyz)*D_Rcorr(xyz);
      
      V_C(xyz) = (  D_S12_Rcorr(xyz) * S_K_Rcorr(xyz)
                  - S_S12_Rcorr(xyz) * D_K_Rcorr(xyz) ) / det;
      
      V_T(xyz) = (  D_K_Rcorr(xyz) * S_Rcorr(xyz)
                  - S_K_Rcorr(xyz) * D_Rcorr(xyz) ) / det;
   }
}