//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup R-correlator
 * @brief   Functions for calculate R-correlator
 * @author  Takaya Miyamoto
 * @since   Wed Feb  3 00:39:01 JST 2016
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
                                 , const CORRELATOR &corr2, const int time_slice )
{
   func_name = "input_Rcorr_NBS/corr_1";
   analysis::route(class_name, func_name, 1);
   
   size_t axyzSIZE = 4 * analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   
   for (size_t axyz=0; axyz<axyzSIZE; axyz++)
      Rcorr_src_prj[axyz] = wave(axyz) / (corr1(time_slice) * corr2(time_slice));
   
   analysis::route(class_name, func_name, 0);
}

void R_CORRELATOR::input(  const NBS_WAVE &wave, const CORRELATOR &corr1
                         , const CORRELATOR &corr2, const int time_slice )
{
   func_name = "input_Rcorr_NBS/corr_2";
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
void R_CORRELATOR_SRC_PRJ::input(  const NBS_WAVE_SRC_PRJ &wave
                                 , const double HAD1_mass
                                 , const double HAD2_mass, const int time_slice )
{
   func_name = "input_Rcorr_NBS/mass_1";
   analysis::route(class_name, func_name, 1);
   
   double dev_mass = exp( -(HAD1_mass+HAD2_mass) * time_slice );
   
   size_t axyzSIZE = 4 * analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   
   for (size_t axyz=0; axyz<axyzSIZE; axyz++)
      Rcorr_src_prj[axyz] = wave(axyz) / dev_mass;
   
   analysis::route(class_name, func_name, 0);
}

void R_CORRELATOR::input(  const NBS_WAVE &wave, const double HAD1_mass
                         , const double HAD2_mass, const int time_slice )
{
   func_name = "input_Rcorr_NBS/mass_2";
   analysis::route(class_name, func_name, 1);
   
   double dev_mass = exp( -(HAD1_mass+HAD2_mass) * time_slice );
   
   size_t xyzSIZE = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   
   for (size_t xyz=0; xyz<xyzSIZE; xyz++)
      Rcorr[xyz] = wave(xyz) / dev_mass;
   
   analysis::route(class_name, func_name, 0);
}
