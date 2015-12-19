//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Calculate laplacian term of potential
 * @author  Takaya Miyamoto
 * @since   Mon Aug 31 18:45:39 JST 2015
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>

string potential::laplacian(  R_CORRELATOR &K_Rcorr
                            , R_CORRELATOR &Rcorr2
                            , double        reduced_mass ) {
   
   int xSHIFT, ySHIFT, zSHIFT;
   for (      int z=0; z<analysis::zSIZE; z++)
      for (   int y=0; y<analysis::ySIZE; y++)
         for (int x=0; x<analysis::xSIZE; x++) {
            
            xSHIFT = 0; ySHIFT = 0; zSHIFT = 0;
            if (x == 0) xSHIFT = analysis::xSIZE;
            if (y == 0) ySHIFT = analysis::ySIZE;
            if (z == 0) zSHIFT = analysis::zSIZE;
            // It's using due to periodic b.c.
            K_Rcorr(x,y,z) =
            
            (  Rcorr2((x+1)%analysis::xSIZE, y, z)
               + Rcorr2( x-1 + xSHIFT        , y, z)
               + Rcorr2(x, (y+1)%analysis::ySIZE, z)
               + Rcorr2(x,  y-1 + ySHIFT        , z)
               + Rcorr2(x, y, (z+1)%analysis::zSIZE)
               + Rcorr2(x, y,  z-1 + zSHIFT        )
               - Rcorr2(x, y, z) * 6.0
               ) / (2.0 * reduced_mass);      /* Laplacian part */
         }
   
   return "potLapterm";
}
