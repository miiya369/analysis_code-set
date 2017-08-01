//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup ALL
 * @brief   Common header file for all headers & source files
 * @author  Takaya Miyamoto
 * @since   Mon Nov  7 16:14:42 JST 2016
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

#ifdef DEBUG_MODE
#define DEBUG_LOG printf("DEBUG-MODE: %s\n", __PRETTY_FUNCTION__);
#else
#define DEBUG_LOG
#endif

#define WORNING_COMMENTS(COMMENTS) \
printf("\n\nWARNING(%s, Line:%d): %s\n\n", __FILE__, __LINE__, COMMENTS);

#define ERROR_COMMENTS(COMMENTS) { \
printf("\n\nERROR(%s, Line:%d): %s\n\n", __FILE__, __LINE__, COMMENTS); \
exit(1); \
}

#define ERROR_FOPEN(FNAME) { \
printf("\n\nFILE OPEN ERROR(%s, Line:%d): Cannot open the file %s.\n\n" \
, __FILE__, __LINE__, FNAME); \
exit(1); \
}

using namespace std;

#define PI (4.0*atan(1))   // Definition for analysis code set
#define hbar_c (197.327053)
#define MAX_N_CH_NAME 32
#define MAX_N_ARG     64
#define MAX_N_DATA    1024
#define MAX_LEN_PATH  256

typedef complex<int>    cint;
typedef complex<double> cdouble;
#ifndef COMP_IMAG
#define COMP_IMAG cdouble(0.0, 1.0)
#endif
#ifndef COMP_ZERO
#define COMP_ZERO cdouble(0.0, 0.0)
#endif

inline cdouble cmp_sqr(cdouble cmp) {
   return cdouble(pow(cmp.real(), 2), pow(cmp.imag(), 2));
}
inline cdouble cmp_sqrt(cdouble cmp) {
   return cdouble(sqrt(cmp.real()), sqrt(cmp.imag()));
}

#endif
