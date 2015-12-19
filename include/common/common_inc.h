//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup ALL
 * @brief   Common header file for all header & source files
 * @author  Takaya Miyamoto
 * @since   Mon Oct  5 16:27:54 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef COMMON_INC_H
#define COMMON_INC_H

#include <stdio.h>
#include <stdarg.h>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <complex>
#include <time.h>

#ifdef _OPENMP
#include <omp.h>
#endif

using namespace std;

#define PI (4.0*atan(1))   // Definition for analysis code set
#define hbar_c (197.327053)
#define MAX_N_CH_NAME 32
#define MAX_N_ARG     64
#define MAX_N_DATA    1024
#define MAX_LEN_PATH  256

typedef complex<int> cint;
typedef complex<double> cdouble;
#define COMP_IMAG cdouble(0.0,1.0)
#define COMP_ZERO cdouble(0.0,0.0)

inline cdouble cmp_sqr( cdouble cmp ){
   return cdouble( pow( cmp.real(), 2 ), pow( cmp.imag(), 2 ) );
}
inline cdouble cmp_sqrt( cdouble cmp ){
   return cdouble( sqrt( cmp.real() ), sqrt( cmp.imag() ) );
}

#endif
