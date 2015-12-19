//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Functions for input for potential
 * @author  Takaya Miyamoto
 * @since   Tue Jul 21 01:39:25 JST 2015
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>

void POTENTIAL::input_pot() {
   
   func_name = "input_pot_NBS/corr____";
   analysis::route(class_name, func_name, 1);
   
   if (potential == NULL) potential = new cdouble[NBSwave::xyznSIZE];
   
   if (corr1 == NULL) {
      corr1 = new cdouble[analysis::tSIZE * analysis::Nconf];
      corr2 = new cdouble[analysis::tSIZE * analysis::Nconf];
      
      CORRELATOR *corr_org = new CORRELATOR;
      if (hadron1.name == hadron2.name) {   // when used same baryon
         corr_org->set_corr(hadron1);
         corr_org->make_JK_sample_corr(1);
         for (size_t i=0; i<analysis::tSIZE*analysis::Nconf; i++) {
            corr1[i] = corr_org->corr[i];
            corr2[i] = corr_org->corr[i];
         }
      } else {   // when used different baryon
         corr_org->set_corr(hadron1);
         corr_org->make_JK_sample_corr(1);
         for (size_t i=0; i<analysis::tSIZE*analysis::Nconf; i++)
            corr1[i] = corr_org->corr[i];
         corr_org->delete_corr();
         corr_org->set_corr(hadron2);
         corr_org->make_JK_sample_corr(1);
         for (size_t i=0; i<analysis::tSIZE*analysis::Nconf; i++)
            corr2[i] = corr_org->corr[i];
      }
      delete corr_org;
   }
   if(Rcorr == NULL) {
      Rcorr = new R_CORRELATOR[3];
      for (int ttt=0; ttt<3; ttt++) {
         Rcorr[ttt].set_Rcorr( channel,time_slice+(ttt-1),endian_flg
                              ,spin,ang_mom,corr1,corr2,compress_flg);
         Rcorr_t[ttt] = Rcorr[ttt].info_time();
      }
   }
   for (int ttt=0; ttt<3; ttt++) read_time_slice_flg[ttt] = true;
   for (int ttt=0; ttt<3; ttt++) {
      Rcorr_reread_flg[ttt] = false;
      
      if      (Rcorr_t[ttt] == time_slice-1)
         read_time_slice_flg[0] = false;
      else if (Rcorr_t[ttt] == time_slice  )
         read_time_slice_flg[1] = false;
      else if (Rcorr_t[ttt] == time_slice+1)
         read_time_slice_flg[2] = false;
      else
         Rcorr_reread_flg[ttt] = true;
   }
   for (int ttt=0; ttt<3; ttt++)
      if (read_time_slice_flg[ttt])
         for (int tt=0; tt<3; tt++)
            if (Rcorr_reread_flg[tt]) {
               Rcorr[tt].delete_Rcorr();
               Rcorr[tt].set_Rcorr( channel,time_slice+(ttt-1),endian_flg
                                   ,spin,ang_mom,corr1,corr2,compress_flg);
               Rcorr_t[tt] = Rcorr[tt].info_time();
               Rcorr_reread_flg[tt] = false;
            }
   analysis::route(class_name, func_name, 0);
}

void POTENTIAL::input_pot( double HAD1_mass, double HAD2_mass ) {
   
   func_name = "input_pot_NBS/mass____";
   analysis::route(class_name, func_name, 1);
   
   if (potential == NULL) potential = new cdouble[NBSwave::xyznSIZE];
   
   if (Rcorr == NULL) {
      Rcorr = new R_CORRELATOR[3];
      for(int ttt=0; ttt<3; ttt++) {
         Rcorr[ttt].set_Rcorr( channel,time_slice+(ttt-1),endian_flg
                              ,spin,ang_mom,HAD1_mass,HAD2_mass,compress_flg);
         Rcorr_t[ttt] = Rcorr[ttt].info_time();
      }
   }
   for (int ttt=0; ttt<3; ttt++) read_time_slice_flg[ttt] = true;
   for (int ttt=0; ttt<3; ttt++) {
      Rcorr_reread_flg[ttt] = false;
      
      if      (Rcorr_t[ttt] == time_slice-1)
         read_time_slice_flg[0] = false;
      else if (Rcorr_t[ttt] == time_slice  )
         read_time_slice_flg[1] = false;
      else if (Rcorr_t[ttt] == time_slice+1)
         read_time_slice_flg[2] = false;
      else
         Rcorr_reread_flg[ttt] = true;
   }
   for (int ttt=0; ttt<3; ttt++)
      if (read_time_slice_flg[ttt])
         for (int tt=0; tt<3; tt++)
            if (Rcorr_reread_flg[tt]) {
               Rcorr[tt].delete_Rcorr();
               Rcorr[tt].set_Rcorr( channel,time_slice+(ttt-1),endian_flg
                                   ,spin,ang_mom,HAD1_mass,HAD2_mass,compress_flg);
               Rcorr_t[tt] = Rcorr[tt].info_time();
               Rcorr_reread_flg[tt] = false;
            }
   analysis::route(class_name, func_name, 0);
}
