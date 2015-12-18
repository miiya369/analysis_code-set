//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Common header file for all class of Analysis Code set
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <fstream>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <complex>
#include <time.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#include <analysis_const.h>

#define PI (4.0*atan(1))   // Definition for analysis code set
#define MAX_N_ARG       64
#define MAX_N_DATA      1024
#define MAX_LEN_PATH    256

using namespace std;

typedef complex<int> cint;
typedef complex<double> cdouble;

void    usage( int );

//! Setting arguments, and return the number of arguments
int     set_arg( int, int, char**, char argv_o[MAX_N_ARG][MAX_LEN_PATH]
                , char list[MAX_N_DATA][MAX_LEN_PATH] );

//! Setting path of data file from list-file, and return the number of confs.
int     set_data_list( const char*, char list[MAX_N_DATA][MAX_LEN_PATH] );

//! Indicate the current class & function name
void    route( string, string, int );

void    endian_convert( cdouble*, int );
void    endian_convert( double*, int );
void    endian_convert( int );

//! Set the path of data file
string  set_path( char data_list[MAX_N_DATA][MAX_LEN_PATH], int );

//! Convert the channel number to hadron names
string channel_to_name( int );
//! Convert the hadron names to channel number
int name_to_channel( char* );
//! Convert the fit function number to function name
string N_func_to_name( int );
//! Convert the fit function name to function number
int name_to_N_func( char* );
//! Convert the hadron type number to hadron name
string had_type_to_name( int );
//! Convert the hadron name to hadron type number
int name_to_hadron_type( char* );

//! Indicate some error, and kill the job
void    error( int, const char* );

#endif
