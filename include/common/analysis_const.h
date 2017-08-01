//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Header File for definition of several enum
 * @author  Takaya Miyamoto
 * @since   Wed Nov  9 07:53:25 JST 2016
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
   CLASS_F_POTENTIAL_GAUSS,
   CLASS_TENSOR_FORCE,
   CLASS_FITTING,
   CLASS_T_MATRIX,
   CLASS_PHASE_SHIFT,
};

//! enum for the main project name
enum {
   EFFECTIVEMASS = 0,
   CALC_WAVE_FUNCTION,
   CALC_POTENTIAL,
   CALC_TENSOR_FORCE,
   FITTING_DATA,
   CALC_PHASE_SHIFT,
   JACK_KNIFE_BIN_CHECK,
   DISPERSION_RELATION,
   WAVE_DECOMPRESSION,
   COUPLED_CHANNEL_POT_2x2,
   COUPLED_CHANNEL_POT_3x3,
   CALC_TENSOR_FORCE_2x2,
   CALC_T_MATRIX_2x2,
   EIGEN_ENERGY,
   EIGEN_ARPACK,
   FOLDING_POTENTIAL_GAUSS,
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
   SPIN_2_0   = 7,
};

//! enum for the path setting
enum {
   MAIN_PATH = 0,
   NBS_DIRECTORY,
   CORR_DIRECTORY,
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
