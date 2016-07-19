//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Header File for definition of several enum
 * @author  Takaya Miyamoto
 * @since   Sun Jul 17 01:07:30 JST 2016
 */
//--------------------------------------------------------------------------

#ifndef ANALYSIS_CONST_H
#define ANALYSIS_CONST_H

//! enum for the class name
enum {
   
   CLASS_CORRELATOR = 0,
   CLASS_NBS_WAVE_ORG,
   CLASS_NBS_WAVE_SRC_PRJ,
   CLASS_NBS_WAVE,
   CLASS_R_CORRELATOR,
   CLASS_R_CORRELATOR_SRC_PRJ,
   CLASS_POTENTIAL,
   CLASS_FITTING,
   CLASS_PHASE_SHIFT,
   CLASS_TENSOR_FORCE,
};

//! enum for the main project name
enum {
    
   COUPLED_CHANNEL_POT_2x2 = 0,
   CALC_WAVE_FUNCTION,
   EFFECTIVEMASS,
   CALC_TENSOR_FORCE,
   CALC_TENSOR_FORCE_2x2,
   CALC_POTENTIAL,
   FITTING_DATA,
   CALC_PHASE_SHIFT,
   AVERAGE_COMPRESS,
   EIGEN_ENERGY,
   JACK_KNIFE_BIN_CHECK,
   MAKE_JACK_KNIFE_SAMPLES,
   DISPERSION_RELATION,
   ANALYSIS_FIT_PARAM,
   DIFFERENCE_CHACK,
   WAVE_DECOMPRESSION,
   CALC_PHASE_SHIFT_2x2,
};

//! enum for spin index of NBS wave function
enum {
   
   SPIN_0_0   = 0,
   SPIN_1_p1  = 1,
   SPIN_1_0   = 2,
   SPIN_1_0ud = 3,
   SPIN_1_0du = 4,
   SPIN_1_m1  = 5,
   SPIN_1_ave = 6,
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
   OPER_TYPE,
   MAX_PATH_ARG,
};

#endif
