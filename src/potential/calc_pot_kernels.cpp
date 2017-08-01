//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Calculate potential kernels
 * @author  Takaya Miyamoto
 * @since   Mon Jan 30 08:58:23 JST 2017
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>

//--------------------------------------------------------------------------
/**
 * @brief The potential karnel (Include Laplacian, 1st time-derivative term)
 */
//--------------------------------------------------------------------------
string potential::kernel(  R_CORRELATOR &K_Rcorr
                         , const R_CORRELATOR &Rcorr1
                         , const R_CORRELATOR &Rcorr2
                         , const R_CORRELATOR &Rcorr3
                         , const double mass1, const double mass2 ) {
   K_Rcorr.mem_alloc();
   
   double delta = (mass1 - mass2) / (mass1 + mass2);
   double mu    = (mass1 * mass2) / (mass1 + mass2);
   
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
             ) / (2.0 * mu)   /* Laplacian part */
            +
            (  Rcorr1(x,y,z)            /* time 1st difference part */
             - Rcorr3(x,y,z) ) * 0.5
            +
            (  Rcorr1(x,y,z)            /* time 2nd difference part */
             + Rcorr3(x,y,z)
             - Rcorr2(x,y,z) * 2.0 ) * (1.0 + 3.0*delta*delta) / (8.0 * mu);
         }
   return "potential";
}

string potential::kernel(  R_CORRELATOR_SRC_PRJ &K_Rcorr
                         , const R_CORRELATOR_SRC_PRJ &Rcorr1
                         , const R_CORRELATOR_SRC_PRJ &Rcorr2
                         , const R_CORRELATOR_SRC_PRJ &Rcorr3
                         , const double mass1, const double mass2 ) {
   K_Rcorr.mem_alloc();
   
   double delta = (mass1 - mass2) / (mass1 + mass2);
   double mu    = (mass1 * mass2) / (mass1 + mass2);
   
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
                ) / (2.0 * mu)   /* Laplacian part */
               +
               (  Rcorr1(x,y,z,ab)         /* time 1st difference part */
                - Rcorr3(x,y,z,ab) ) * 0.5
               +
               (  Rcorr1(x,y,z,ab)            /* time 2nd difference part */
                + Rcorr3(x,y,z,ab)
                - Rcorr2(x,y,z,ab) * 2.0 ) * (1.0 + 3.0*delta*delta) / (8.0 * mu);
            }
   }
   return "potential";
}

//--------------------------------------------------------------------------
/**
 * @brief The laplacian term
 */
//--------------------------------------------------------------------------
string potential::laplacian(  R_CORRELATOR &K_Rcorr, const R_CORRELATOR &Rcorr
                            , const double mass1, const double mass2 ) {
   K_Rcorr.mem_alloc();
   
   double mu    = (mass1 * mass2) / (mass1 + mass2);
   
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
            
            (  Rcorr((x+1)%analysis::xSIZE, y, z)
             + Rcorr( x-1 + xSHIFT        , y, z)
             + Rcorr(x, (y+1)%analysis::ySIZE, z)
             + Rcorr(x,  y-1 + ySHIFT        , z)
             + Rcorr(x, y, (z+1)%analysis::zSIZE)
             + Rcorr(x, y,  z-1 + zSHIFT        )
             - Rcorr(x, y, z) * 6.0 ) / (2.0 * mu);
         }
   return "potLapterm";
}
string potential::laplacian(  NBS_WAVE &K_NBS, const NBS_WAVE &NBS
                            , const double mass1, const double mass2 ) {
   K_NBS.mem_alloc();
   
   double mu    = (mass1 * mass2) / (mass1 + mass2);
   
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
            K_NBS(x,y,z) =
            
            (  NBS((x+1)%analysis::xSIZE, y, z)
             + NBS( x-1 + xSHIFT        , y, z)
             + NBS(x, (y+1)%analysis::ySIZE, z)
             + NBS(x,  y-1 + ySHIFT        , z)
             + NBS(x, y, (z+1)%analysis::zSIZE)
             + NBS(x, y,  z-1 + zSHIFT        )
             - NBS(x, y, z) * 6.0 ) / (2.0 * mu);
         }
   return "potLapterm";
}
string potential::laplacian_bare(      R_CORRELATOR &K_Rcorr,
                                 const R_CORRELATOR &Rcorr) {
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
            
            (  Rcorr((x+1)%analysis::xSIZE, y, z)
             + Rcorr( x-1 + xSHIFT        , y, z)
             + Rcorr(x, (y+1)%analysis::ySIZE, z)
             + Rcorr(x,  y-1 + ySHIFT        , z)
             + Rcorr(x, y, (z+1)%analysis::zSIZE)
             + Rcorr(x, y,  z-1 + zSHIFT        )
             - Rcorr(x, y, z) * 6.0 );
         }
   return "potLapterm";
}

//--------------------------------------------------------------------------
/**
 * @brief The 1st time-difference term
 */
//--------------------------------------------------------------------------
string potential::first_time_diff(  R_CORRELATOR &K_Rcorr
                                  , const R_CORRELATOR &Rcorr1
                                  , const R_CORRELATOR &Rcorr2 ) {
   K_Rcorr.mem_alloc();
   
   for (      int z=0; z<analysis::zSIZE; z++)
      for (   int y=0; y<analysis::ySIZE; y++)
         for (int x=0; x<analysis::xSIZE; x++)
         {
            K_Rcorr(x,y,z) =
            
            (Rcorr1(x,y,z) - Rcorr2(x,y,z)) * 0.5;
         }
   return "potT1term";
}

//--------------------------------------------------------------------------
/**
 * @brief The 2nd time-difference term
 */
//--------------------------------------------------------------------------
string potential::second_time_diff(  R_CORRELATOR &K_Rcorr
                                   , const R_CORRELATOR &Rcorr1
                                   , const R_CORRELATOR &Rcorr2
                                   , const R_CORRELATOR &Rcorr3
                                   , const double mass1, const double mass2 ) {
   K_Rcorr.mem_alloc();
   
   double delta = (mass1 - mass2) / (mass1 + mass2);
   double mu    = (mass1 * mass2) / (mass1 + mass2);
   
   for (      int z=0; z<analysis::zSIZE; z++)
      for (   int y=0; y<analysis::ySIZE; y++)
         for (int x=0; x<analysis::xSIZE; x++)
         {
            K_Rcorr(x,y,z) =
            
            (  Rcorr1(x,y,z)
             + Rcorr3(x,y,z)
             - Rcorr2(x,y,z) * 2.0 ) * (1.0 + 3.0*delta*delta) / (8.0 * mu);
         }
   return "potT2term";
}
