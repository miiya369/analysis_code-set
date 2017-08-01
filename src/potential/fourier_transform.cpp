//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Take fourier transformation for momentum representation
 * @author  Takaya Miyamoto
 * @since   Sun Oct 18 04:14:40 JST 2015
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>

cdouble POTENTIAL::momentum_rep(const double momentum) {
   cdouble ret;
   
   for (      int z=0; z<analysis::zSIZE; z++)
      for (   int y=0; y<analysis::ySIZE; y++)
         for (int x=0; x<analysis::xSIZE; x++)
            if (x>analysis::xSIZE/2 || y>x || z>y) continue;
            else
               ret += (  potential[idx(x,y,z)]
                       * exp(-COMP_IMAG * momentum * sqrt(x*x + y*y + z*z)));
   return ret;
}
