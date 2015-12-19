//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Define the namespace for common variables & functions
 * @author  Takaya Miyamoto
 * @since   Fri Oct 23 05:41:35 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <common/common_inc.h>

namespace analysis {
   
   //! Set the path of data file
   string set_path( int );
   string set_path_doi_san( int );
   
   void usage( int );
   
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
   
   //! Print the current class & function name
   void route( string, string, int );
   //! Indicate some error, and kill the job
   void error( int, const char* );
}

namespace analysis {
   
   extern int xSIZE;
   extern int ySIZE;
   extern int zSIZE;
   extern int tSIZE;
   extern int Nconf;
   
   extern char data_list[MAX_N_DATA][MAX_LEN_PATH];
   
   //! Setting confs. list, and return the number of confs.
   int  set_data_list(  const char* );
   //! Setting data list for path of NBS wave or correlator datas.
   void set_data_list(  const char*, const char*, const char*, const char*
                      , const char*, const char*, const char* );
   //! Wrapper of above 2 functions
   int  set_data_list(  const char*, const char*, const char*, const char*
                      , const char*, const char*, const char*, const char* );
   void set_data_list(  int,  const char*, ... );
   void set_data(       int, int, int, int, int );
   
   int  reduced_Ndata();
}

#include <common/analysis_const.h>
#include <common/hadron_type_const.h>
#include <common/channel_type_const.h>

namespace analysis {
   
   void make_mean_err( double*, double&, double&, int, bool );
   void make_mean_err( cdouble*, cdouble&, cdouble&, int, bool );
}

#include <common/config_tmp.h>

#endif
