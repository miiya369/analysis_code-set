//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Calculate 2nd time-difference term of potential
 * @author  Takaya Miyamoto
 * @since   Tue Feb  2 19:47:05 JST 2016
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>

string potential::second_time_diff(  R_CORRELATOR &K_Rcorr
                                   , const R_CORRELATOR &Rcorr1
                                   , const R_CORRELATOR &Rcorr2
                                   , const R_CORRELATOR &Rcorr3
                                   , const double        reduced_mass)
{
   K_Rcorr.mem_alloc();
   
   for (      int z=0; z<analysis::zSIZE; z++)
      for (   int y=0; y<analysis::ySIZE; y++)
         for (int x=0; x<analysis::xSIZE; x++)
         {
            K_Rcorr(x,y,z) =
            
            (  Rcorr1(x,y,z)
             + Rcorr3(x,y,z)
             - Rcorr2(x,y,z) * 2.0 ) / (4.0 * reduced_mass);
         }
   
   return "potT2term";
}
