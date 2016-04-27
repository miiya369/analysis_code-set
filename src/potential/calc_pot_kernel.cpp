//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Calculate potential kernel
 * @author  Takaya Miyamoto
 * @since   Wed Feb  3 00:39:01 JST 2016
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>

string potential::kernel(  R_CORRELATOR &K_Rcorr
                         , const R_CORRELATOR &Rcorr1
                         , const R_CORRELATOR &Rcorr2
                         , const R_CORRELATOR &Rcorr3
                         , const double        reduced_mass )
{
   K_Rcorr.mem_alloc();
   
   int xSHIFT, ySHIFT, zSHIFT;
   for (      int z=0; z<analysis::zSIZE; z++)
      for (   int y=0; y<analysis::ySIZE; y++)
         for (int x=0; x<analysis::xSIZE; x++)
         {
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
                ) / (2.0 * reduced_mass)      /* Laplacian part */
               +
               (  Rcorr1(x,y,z)               /* time 1st difference part */
                - Rcorr3(x,y,z) ) * 0.5
               +
               (  Rcorr1(x,y,z)               /* time 2nd difference part */
                + Rcorr3(x,y,z)
                - Rcorr2(x,y,z) * 2.0 ) / (4.0 * reduced_mass);
            }
   
   return "potential";
}

string potential::kernel(  R_CORRELATOR_SRC_PRJ &K_Rcorr
                         , const R_CORRELATOR_SRC_PRJ &Rcorr1
                         , const R_CORRELATOR_SRC_PRJ &Rcorr2
                         , const R_CORRELATOR_SRC_PRJ &Rcorr3
                         , const double        reduced_mass )
{
   K_Rcorr.mem_alloc();
   
   int xSHIFT, ySHIFT, zSHIFT;
   for (int ab=0; ab<4; ab++)
   {
      for (      int z=0; z<analysis::zSIZE; z++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int x=0; x<analysis::xSIZE; x++)
            {
               xSHIFT = 0; ySHIFT = 0; zSHIFT = 0;
               if (x == 0) xSHIFT = analysis::xSIZE;
               if (y == 0) ySHIFT = analysis::ySIZE;
               if (z == 0) zSHIFT = analysis::zSIZE;
               // It's using due to periodic b.c.
               K_Rcorr(x,y,z,ab) =
               
               (  Rcorr2((x+1)%analysis::xSIZE, y, z, ab)
                + Rcorr2( x-1 + xSHIFT        , y, z, ab)
                + Rcorr2(x, (y+1)%analysis::ySIZE, z, ab)
                + Rcorr2(x,  y-1 + ySHIFT        , z, ab)
                + Rcorr2(x, y, (z+1)%analysis::zSIZE, ab)
                + Rcorr2(x, y,  z-1 + zSHIFT        , ab)
                - Rcorr2(x, y, z, ab) * 6.0
                ) / (2.0 * reduced_mass)      /* Laplacian part */
               +
               (  Rcorr1(x,y,z,ab)               /* time 1st difference part */
                - Rcorr3(x,y,z,ab) ) * 0.5
               +
               (  Rcorr1(x,y,z,ab)               /* time 2nd difference part */
                + Rcorr3(x,y,z,ab)
                - Rcorr2(x,y,z,ab) * 2.0 ) / (4.0 * reduced_mass);
            }
   }
   return "potential";
}
