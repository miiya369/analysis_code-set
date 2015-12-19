//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Calculate potential kernel
 * @author  Takaya Miyamoto
 * @since   Wed Jul 22 05:35:46 JST 2015
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>

void POTENTIAL::calc_pot_kernel() {
    
   func_name = "calc_pot_kernel_______";
   analysis::route(class_name, func_name, 1);
   
   if (Rcorr == NULL) {
      analysis::error(1,"Potential has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   cdouble *Rcorr_ptr1 = NULL;
   cdouble *Rcorr_ptr2 = NULL;
   cdouble *Rcorr_ptr3 = NULL;
   
   for (int ttt=0; ttt<3; ttt++) {
      if      (Rcorr_t[ttt] == time_slice-1)
         Rcorr_ptr1 = Rcorr[ttt].Rcorr;
      else if (Rcorr_t[ttt] == time_slice  )
         Rcorr_ptr2 = Rcorr[ttt].Rcorr;
      else if (Rcorr_t[ttt] == time_slice+1)
         Rcorr_ptr3 = Rcorr[ttt].Rcorr;
   }
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
               
               (  Rcorr_ptr2[idx((x+1)%analysis::xSIZE, y, z, i)]
                + Rcorr_ptr2[idx( x-1 + xSHIFT        , y, z, i)]
                + Rcorr_ptr2[idx(x, (y+1)%analysis::ySIZE, z, i)]
                + Rcorr_ptr2[idx(x,  y-1 + ySHIFT        , z, i)]
                + Rcorr_ptr2[idx(x, y, (z+1)%analysis::zSIZE, i)]
                + Rcorr_ptr2[idx(x, y,  z-1 + zSHIFT        , i)]
                - Rcorr_ptr2[idx(x, y, z, i)] * 6.0 )
                 / (2.0 * reduced_mass)      /* Laplacian part */
               +
               (  Rcorr_ptr1[idx(x,y,z,i)]   /* time 1st difference part */
                - Rcorr_ptr3[idx(x,y,z,i)] ) * 0.5
               +
               (  Rcorr_ptr1[idx(x,y,z,i)]   /* time 2nd difference part */
                + Rcorr_ptr3[idx(x,y,z,i)]
                - Rcorr_ptr2[idx(x,y,z,i)] * 2.0 ) / (4.0 * reduced_mass);
            }
   printf(" @ Finished calculate potential    : %s, t=%d\n"
          , channel.name.c_str(), time_slice);
   potential_type = "potential";
   
   analysis::route(class_name, func_name, 0);
}
