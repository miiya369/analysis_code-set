//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup R-correlator
 * @brief   Functions for calculate R-correlator
 * @author  Takaya Miyamoto
 * @since   Mon Jul 20 13:07:56 JST 2015
 */
//--------------------------------------------------------------------------

#include <R_correlator/R_correlator.h>

//--------------------------------------------------------------------------
/**
 * @brief Calculate R-correlator by input correlator files
 */
//--------------------------------------------------------------------------
void R_CORRELATOR::input_Rcorr( int spin, int angmom ) {
    
   func_name = "input_Rcorr_NBS/corr_1";
   analysis::route(class_name, func_name, 1);
   
   if (compress_flg) input_compressed_NBS();
   else              input_NBS();
   projection( spin, angmom );   // construct NBS wave
   make_JK_sample_projNBS(1);
   
   set_corr(hadron1);   // construct correlator 1
   make_JK_sample_corr(1);
   
   cdouble *tmp_corr = new cdouble[analysis::Nconf];
   
   if (hadron1.name == hadron2.name) {   // when used same baryon
      for (int conf = 0; conf<analysis::Nconf; conf++)
         tmp_corr[conf] = 2.0 * corr[nt(conf,time_slice)];
      delete_corr();
   }
   else {   // when used different baryon
      for (int conf = 0; conf<analysis::Nconf; conf++)
         tmp_corr[conf] = corr[nt(conf,time_slice)];
      delete_corr();
      
      set_corr(hadron2);   // construct correlator 2
      make_JK_sample_corr(1);
      
      for (int conf = 0; conf<analysis::Nconf; conf++)
         tmp_corr[conf] *= corr[nt(conf,time_slice)];
      delete_corr();
   }
   if (Rcorr == NULL) Rcorr = new cdouble[NBSwave::xyznSIZE];
   
   for (   size_t conf=0; conf<analysis::Nconf ; conf++)
      for (size_t xyz =0;  xyz<NBSwave::xyzSIZE; xyz++)
         Rcorr[xyz+NBSwave::xyzSIZE*conf] = projNBS[xyz+NBSwave::xyzSIZE*conf]
                                            / tmp_corr[conf];
   delete_NBS();
   delete [] tmp_corr;
   
   printf(" @ Finished construct R correlator : %s, t=%d\n"
          , channel.name.c_str(), time_slice);
   
   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Calculate R-correlator using prepared correlator pointer
 */
//--------------------------------------------------------------------------
void R_CORRELATOR::input_Rcorr(  int spin, int angmom
                               , cdouble *corr1, cdouble *corr2 ) {
    
   func_name = "input_Rcorr_NBS/corr_2";
   analysis::route(class_name, func_name, 1);
   
   if (compress_flg) input_compressed_NBS();
   else              input_NBS();
   projection( spin, angmom );   // construct NBS wave
   make_JK_sample_projNBS(1);
   
   if (Rcorr == NULL) Rcorr = new cdouble[NBSwave::xyznSIZE];
   
   for (   size_t conf=0; conf<analysis::Nconf ; conf++)
      for (size_t xyz =0;  xyz<NBSwave::xyzSIZE; xyz++)
         Rcorr[xyz+NBSwave::xyzSIZE*conf] = projNBS[xyz+NBSwave::xyzSIZE*conf]
               / (corr1[nt(conf,time_slice)] * corr2[nt(conf,time_slice)]);
   delete_NBS();
   
   printf(" @ Finished construct R correlator : %s, t=%d\n"
          , channel.name.c_str(), time_slice);
   
   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Calculate R-correlator using prepared each hadron mass
 */
//--------------------------------------------------------------------------
void R_CORRELATOR::input_Rcorr(  int spin, int angmom
                               , double HAD1_mass, double HAD2_mass ) {
    
   func_name = "input_Rcorr_NBS/mass__";
   analysis::route(class_name, func_name, 1);
   
   if (compress_flg) input_compressed_NBS();
   else              input_NBS();
   projection( spin, angmom );   // construct NBS wave
   make_JK_sample_projNBS(1);
   
   double dev_mass = exp( -(HAD1_mass+HAD2_mass) * time_slice );
   
   if (Rcorr == NULL) Rcorr = new cdouble[NBSwave::xyznSIZE];
   
   for (   size_t conf=0; conf<analysis::Nconf ; conf++)
      for (size_t xyz =0;  xyz<NBSwave::xyzSIZE; xyz++)
         Rcorr[xyz+NBSwave::xyzSIZE*conf] = projNBS[xyz+NBSwave::xyzSIZE*conf]
                                           / dev_mass;
   delete_NBS();
   
   printf(" @ Finished construct R correlator : %s, t=%d\n"
          , channel.name.c_str(), time_slice);
   
   analysis::route(class_name, func_name, 0);
}