//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Header File for definition of several enum
 * @author  Takaya Miyamoto
 * @since   Mon Sep  7 02:20:13 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef ANALYSIS_CONST_H
#define ANALYSIS_CONST_H

//! enum for the class name
enum {
   
   CLASS_CORRELATOR = 0,
   CLASS_NBS_WAVE_ORG,
   CLASS_NBS_WAVE,
   CLASS_R_CORRELATOR,
   CLASS_POTENTIAL,
   CLASS_FITTING,
   CLASS_PHASE_SHIFT,
};

//! enum for the main project name
enum {
    
   COUPLED_CHANNEL_POT = 0,
   CALC_WAVE_FUNCTION,
   EFFECTIVEMASS,
   CALC_POTENTIAL,
   FITTING_DATA,
   CALC_OBSERVABLE,
   EXTRACT_Z_FACTOR,
   AVERAGE_COMPRESS,
};

//! enum for the path setting
enum {
    
   MAIN_PATH = 0,
   NBS_DIRECTORY,
   CORR_DIRECTORY,
   N_CHANNEL,
   N_TIME,
   HADRON_NAME,
   N_T_SHIFT,
   N_X_SHIFT,
   N_Y_SHIFT,
   N_Z_SHIFT,
   SNK_RELA,
   SRC_RELA,
   MAX_PATH_ARG,
};

#endif
