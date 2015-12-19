//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Calculate 2nd time-difference term of potential
 * @author  Takaya Miyamoto
 * @since   Mon Aug 31 18:45:05 JST 2015
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>

string potential::second_time_diff(  R_CORRELATOR &K_Rcorr
                                   , R_CORRELATOR &Rcorr1
                                   , R_CORRELATOR &Rcorr2
                                   , R_CORRELATOR &Rcorr3
                                   , double        reduced_mass) {
   
   for (      int z=0; z<analysis::zSIZE; z++)
      for (   int y=0; y<analysis::ySIZE; y++)
         for (int x=0; x<analysis::xSIZE; x++) {
            K_Rcorr(x,y,z) =
            
            (  Rcorr1(x,y,z)               /* time 2nd difference part */
             + Rcorr3(x,y,z)
             - Rcorr2(x,y,z) * 2.0 ) / (4.0 * reduced_mass);
         }
   
   return "potT2term";
}
