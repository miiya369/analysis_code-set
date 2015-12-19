//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup R-correlator
 * @brief   Functions for calculate R-correlator
 * @author  Takaya Miyamoto
 * @since   Mon Aug 31 18:42:39 JST 2015
 */
//--------------------------------------------------------------------------

#include <R_correlator/R_correlator.h>

//--------------------------------------------------------------------------
/**
 * @brief Calculate R-correlator using prepared correlator pointer
 */
//--------------------------------------------------------------------------
void R_CORRELATOR::input(  NBS_WAVE &wave
                         , CORRELATOR &corr1, CORRELATOR &corr2, int time_slice ) {
    
   func_name = "input_Rcorr_NBS/corr__";
   analysis::route(class_name, func_name, 1);
   
   size_t xyzSIZE = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   
   for (size_t xyz=0; xyz<xyzSIZE; xyz++)
      Rcorr[xyz] = wave(xyz) / (corr1(time_slice) * corr2(time_slice));
   
   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Calculate R-correlator using prepared each hadron mass
 */
//--------------------------------------------------------------------------
void R_CORRELATOR::input(  NBS_WAVE &wave
                         , double HAD1_mass, double HAD2_mass, int time_slice ) {
    
   func_name = "input_Rcorr_NBS/mass__";
   analysis::route(class_name, func_name, 1);
   
   double dev_mass = exp( -(HAD1_mass+HAD2_mass) * time_slice );
   
   size_t xyzSIZE = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   
   for (size_t xyz=0; xyz<xyzSIZE; xyz++)
      Rcorr[xyz] = wave(xyz) / dev_mass;
   
   analysis::route(class_name, func_name, 0);
}
