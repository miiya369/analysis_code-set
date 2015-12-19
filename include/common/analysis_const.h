//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Header File for definition of several enum
 * @author  Takaya Miyamoto
 * @since   Mon Jul 20 08:46:10 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef ANALYSIS_CONST_H
#define ANALYSIS_CONST_H

//! enum for the main project name
enum {
    
   COUPLED_CHANNEL_POT = 0,
   EFFECTIVEMASS,
   CALC_POTENTIAL,
   FITTING_DATA,
   CALC_OBSERVABLE,
   ISOSPIN_PROJECTION,
   EXTRACT_Z_FACTOR,
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
