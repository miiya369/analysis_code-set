//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Calculate 2nd time-difference term of potential
 * @author  Takaya Miyamoto
 * @since   Wed Jul 29 01:43:38 JST 2015
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>

void POTENTIAL::calc_2nd_timediff() {
    
   func_name = "calc_2nd_timediff_____";
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
   
      /* time 2nd difference part */
   for (int n=0; n<NBSwave::xyznSIZE; n++)
      potential[n] =  (Rcorr_ptr1[n] + Rcorr_ptr3[n] - Rcorr_ptr2[n] * 2.0)
                    / (4.0 * reduced_mass);
   
   printf(" @ Finished calculate potential time 2nd differential part : "
          "%s, spin=%d, spin_z=%d, t=%d\n"
          , channel.name.c_str(), spin, spin_z, time_slice);
   potential_type = "potT2term";
   
   analysis::route(class_name, func_name, 0);
}
