//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Common header file for all class of Analysis Code set
 * @author  Takaya Miyamoto
 * @since   Mon Sep  7 02:31:08 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef ANALYSIS_H
#define ANALYSIS_H

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

namespace analysis {
    
   extern int xSIZE;
   extern int ySIZE;
   extern int zSIZE;
   extern int tSIZE;
   
   extern int Nconf;
   
   extern char data_list[MAX_N_DATA][MAX_LEN_PATH];

   void usage( int );
   //! Setting confs. list, and return the number of confs.
   int  set_data_list( const char* );
   //! Setting data list for path of NBS wave or correlator datas.
   void set_data_list(  const char*, const char*, const char*, const char*
                      , const char*, const char*, const char* );
   //! Wrapper of above 2 functions
   int  set_data_list(  const char*, const char*, const char*, const char*
                      , const char*, const char*, const char*, const char* );
   void set_data_list(  int, const char*, ... );
   //! Set the path of data file
   string set_path( int );
   
   bool machine_is_little_endian();
   void endian_convert( cdouble*, size_t );
   void endian_convert( double*, size_t );
   void endian_convert( int*, size_t );
   
   //! Convert the bool to string ( yes or no )
   string bool_to_str( bool );
   //! Convert the string ( yes or no ) to bool
   bool str_to_bool( string );
   //! Convert index xyz -> x,y,z
   void convert_xyz(int *ix, int *iy, int *iz, size_t xyz);
   //! Convert index xyzn -> x,y,z,conf
   void convert_xyzn(int *ix, int *iy, int *iz, int *conf, size_t xyzn);
   
   int  reduced_Ndata();
   
   void make_mean_err( double*, double&, double&, bool );
   void make_mean_err( cdouble*, cdouble&, cdouble&, bool );
   
   //! Print the current class & function name
   void route( string, string, int );
   //! Indicate some error, and kill the job
   void error( int, const char* );
}

#include <common/config_tmp.h>

namespace analysis {
   //! The function for data output
   template <class X> void output_data_all( X&, const char* );
   template <class X> void output_data_err( CONFIG<X>&, const char* );
   template <class X> void output_data_fit( CONFIG<X>&, const char* );
}

#include <common/analysis_const.h>
#include <common/hadron_type_const.h>
#include <common/channel_type_const.h>

#include <common/output_data.h>

#endif
