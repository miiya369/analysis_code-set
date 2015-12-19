//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Header File for definition of enum
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#ifndef ANALYSIS_CONST_H
#define ANALYSIS_CONST_H

#include <stdio.h>

//! enum for the hadron name
enum {
    
    PION = 0,
    PHI,
    ETASS,
    MESON_BARYON_BOUNDARY,
    PROTON,
    LAMBDA,
    SIGMA,
};

//! enum for the channel of NBS wave functions
enum {
//============ particle base ============//
// S=0
    PROTON_NEUTRON__PROTON_NEUTRON = 0,
// S=1
    PROTON_LAMBDA__PROTON_LAMBDA,
    PROTON_LAMBDA__PROTON_SIGMAZ,
    PROTON_LAMBDA__NEUTRON_SIGMAP,
    PROTON_SIGMAZ__PROTON_LAMBDA,
    PROTON_SIGMAZ__PROTON_SIGMAZ,
    PROTON_SIGMAZ__NEUTRON_SIGMAP,
    NEUTRON_SIGMAP__PROTON_LAMBDA,
    NEUTRON_SIGMAP__PROTON_SIGMAZ,
    NEUTRON_SIGMAP__NEUTRON_SIGMAP,   // = 9
    
    BOUNDARY_PARTICLE_ISOSPIN,
//============ isospin base ============//
    N_LAMBDA_12__N_LAMBDA_12,   // = 11
    N_LAMBDA_12__N_SIGMA_12,
    N_SIGMA_12__N_LAMBDA_12,
    N_SIGMA_12__N_SIGMA_12,
    N_LAMBDA_12__N_SIGMA_32,
    N_SIGMA_32__N_LAMBDA_12,
    N_SIGMA_32__N_SIGMA_32,     // = 17
};

//! enum for the fit function type
enum {
    
    TEST = -1,
    CONSTANT = 0,
    ONE_EXPONENTIAL,
    ONE_GAUSSIAN,
    TWO_GAUSSIAN,
    THREE_GAUSSIAN,
    ONE_SHIFTED_GAUSSIAN,
    TWO_SHIFTED_GAUSSIAN,
};

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
