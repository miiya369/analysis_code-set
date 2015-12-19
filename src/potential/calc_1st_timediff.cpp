//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Calculate 1st time-difference term of potential
 * @author  Takaya Miyamoto
 * @since   Mon Aug 31 18:44:39 JST 2015
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>

string potential::first_time_diff(  R_CORRELATOR &K_Rcorr
                                  , R_CORRELATOR &Rcorr1
                                  , R_CORRELATOR &Rcorr3 ) {
   
   for (      int z=0; z<analysis::zSIZE; z++)
      for (   int y=0; y<analysis::ySIZE; y++)
         for (int x=0; x<analysis::xSIZE; x++) {
            K_Rcorr(x,y,z) =
            
            (  Rcorr1(x,y,z)               /* time 1st difference part */
             - Rcorr3(x,y,z) ) * 0.5;
         }
   
   return "potT1term";
}
