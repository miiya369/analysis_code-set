//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Calculate laplacian term of potential
 * @author  Takaya Miyamoto
 * @since   Fri Dec 18 00:45:23 JST 2015
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>

string potential::laplacian(  R_CORRELATOR &K_Rcorr, R_CORRELATOR &Rcorr
                            , double reduced_mass ) {
   
   K_Rcorr.mem_alloc();
   
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
            
            (  Rcorr((x+1)%analysis::xSIZE, y, z)
             + Rcorr( x-1 + xSHIFT        , y, z)
             + Rcorr(x, (y+1)%analysis::ySIZE, z)
             + Rcorr(x,  y-1 + ySHIFT        , z)
             + Rcorr(x, y, (z+1)%analysis::zSIZE)
             + Rcorr(x, y,  z-1 + zSHIFT        )
             - Rcorr(x, y, z) * 6.0 ) / (2.0 * reduced_mass);
         }
   
   return "potLapterm";
}

string potential::laplacian(  NBS_WAVE &K_NBS, NBS_WAVE &NBS
                            , double reduced_mass ) {
   
   K_NBS.mem_alloc();
   
   int xSHIFT, ySHIFT, zSHIFT;
   for (      int z=0; z<analysis::zSIZE; z++)
      for (   int y=0; y<analysis::ySIZE; y++)
         for (int x=0; x<analysis::xSIZE; x++) {
            
            xSHIFT = 0; ySHIFT = 0; zSHIFT = 0;
            if (x == 0) xSHIFT = analysis::xSIZE;
            if (y == 0) ySHIFT = analysis::ySIZE;
            if (z == 0) zSHIFT = analysis::zSIZE;
            // It's using due to periodic b.c.
            K_NBS(x,y,z) =
            
            (  NBS((x+1)%analysis::xSIZE, y, z)
             + NBS( x-1 + xSHIFT        , y, z)
             + NBS(x, (y+1)%analysis::ySIZE, z)
             + NBS(x,  y-1 + ySHIFT        , z)
             + NBS(x, y, (z+1)%analysis::zSIZE)
             + NBS(x, y,  z-1 + zSHIFT        )
             - NBS(x, y, z) * 6.0 ) / (2.0 * reduced_mass);         }
   
   return "potLapterm";
}
