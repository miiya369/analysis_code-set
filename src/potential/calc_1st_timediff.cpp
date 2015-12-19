//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Calculate 1st time-difference term of potential
 * @author  Takaya Miyamoto
 * @since   Wed Jul 22 05:33:57 JST 2015
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>

void POTENTIAL::calc_1st_timediff() {
   
   func_name = "calc_1st_timediff_____";
   analysis::route(class_name, func_name, 1);
   
   if (Rcorr == NULL) {
      analysis::error(1,"Potential has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   cdouble *Rcorr_ptr1 = NULL;
   cdouble *Rcorr_ptr2 = NULL;
   
   for (int ttt=0; ttt<3; ttt++) {
      if (     Rcorr_t[ttt] == time_slice-1)
         Rcorr_ptr1 = Rcorr[ttt].Rcorr;
      else if (Rcorr_t[ttt] == time_slice+1)
         Rcorr_ptr2 = Rcorr[ttt].Rcorr;
   }
   
      /* time 1st difference part */
   for (int n=0; n<NBSwave::xyznSIZE; n++)
      potential[n] = (Rcorr_ptr1[n] - Rcorr_ptr2[n]) * 0.5;
   
   printf(" @ Finished calculate potential time 1st difference part : %s, t=%d\n"
          , channel.name.c_str(), time_slice);
   potential_type = "potT1term";
   
   analysis::route(class_name, func_name, 0);
}
