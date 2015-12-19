//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Calculate laplacian term of potential
 * @author  Takaya Miyamoto
 * @since   Wed Jul 29 01:43:38 JST 2015
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>

void POTENTIAL::calc_laplacian() {
    
   func_name = "calc_laplacian________";
   analysis::route(class_name, func_name, 1);
   
   if (Rcorr == NULL) {
      analysis::error(1,"Potential has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   cdouble *Rcorr_ptr = NULL;
   
   for (int ttt=0; ttt<3; ttt++)
      if (Rcorr_t[ttt] == time_slice)
         Rcorr_ptr = Rcorr[ttt].Rcorr;
   
      /* Laplacian part */
   int xSHIFT, ySHIFT, zSHIFT;
   for (         int i=0; i<analysis::Nconf; i++)
      for (      int z=0; z<analysis::zSIZE; z++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int x=0; x<analysis::xSIZE; x++) {
               
               xSHIFT = 0; ySHIFT = 0; zSHIFT = 0;
               if (x == 0) xSHIFT = analysis::xSIZE;
               if (y == 0) ySHIFT = analysis::ySIZE;
               if (z == 0) zSHIFT = analysis::zSIZE;
               // It's using due to periodic b.c.
               potential[idx(x,y,z,i)] =
               
               (  Rcorr_ptr[idx((x+1)%analysis::xSIZE, y, z, i)]
                + Rcorr_ptr[idx( x-1 + xSHIFT        , y, z, i)]
                + Rcorr_ptr[idx(x, (y+1)%analysis::ySIZE, z, i)]
                + Rcorr_ptr[idx(x,  y-1 + ySHIFT        , z, i)]
                + Rcorr_ptr[idx(x, y, (z+1)%analysis::zSIZE, i)]
                + Rcorr_ptr[idx(x, y,  z-1 + zSHIFT        , i)]
                - Rcorr_ptr[idx(x, y, z, i)] * 6.0 )
               / (2.0 * reduced_mass);
            }
   printf(" @ Finished calculate potential laplacian part             : "
          "%s, spin=%d, spin_z=%d, t=%d\n"
          , channel.name.c_str(), spin, spin_z, time_slice);
   potential_type = "potLapterm";
   
   analysis::route(class_name, func_name, 0);
}
