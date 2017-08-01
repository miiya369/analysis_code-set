//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup R-correlator
 * @brief   Functions for calculate R-correlator
 * @author  Takaya Miyamoto
 * @since   Mon Nov  7 16:14:42 JST 2016
 */
//--------------------------------------------------------------------------

#include <R_correlator/R_correlator.h>

//--------------------------------------------------------------------------
/**
 * @brief Calculate R-correlator using prepared correlator pointer
 */
//--------------------------------------------------------------------------
void R_CORRELATOR_SRC_PRJ::input(  const NBS_WAVE_SRC_PRJ &wave
                                 , const CORRELATOR &corr1
                                 , const CORRELATOR &corr2
                                 , const int time_slice ) {
   DEBUG_LOG
   
   for (int axyz=0; axyz<data_size(); axyz++)
      Rcorr_src_prj[axyz] = wave(axyz) / (corr1(time_slice) * corr2(time_slice));
}

void R_CORRELATOR::input(  const NBS_WAVE &wave, const CORRELATOR &corr1
                         , const CORRELATOR &corr2, const int time_slice ) {
   DEBUG_LOG
   
   for (int xyz=0; xyz<data_size(); xyz++)
      Rcorr[xyz] = wave(xyz) / (corr1(time_slice) * corr2(time_slice));
}

//--------------------------------------------------------------------------
/**
 * @brief Calculate R-correlator using prepared each hadron mass
 */
//--------------------------------------------------------------------------
void R_CORRELATOR_SRC_PRJ::input(  const NBS_WAVE_SRC_PRJ &wave
                                 , const double HAD1_mass
                                 , const double HAD2_mass
                                 , const int time_slice ) {
   DEBUG_LOG
   
   double dev_mass = exp(-(HAD1_mass+HAD2_mass) * time_slice);
   
   for (int axyz=0; axyz<data_size(); axyz++)
      Rcorr_src_prj[axyz] = wave(axyz) / dev_mass;
}

void R_CORRELATOR::input(  const NBS_WAVE &wave, const double HAD1_mass
                         , const double HAD2_mass, const int time_slice ) {
   DEBUG_LOG
   
   double dev_mass = exp(-(HAD1_mass+HAD2_mass) * time_slice);
   
   for (int xyz=0; xyz<data_size(); xyz++)
      Rcorr[xyz] = wave(xyz) / dev_mass;
}
