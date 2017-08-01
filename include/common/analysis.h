//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Define the namespace for common variables & functions
 * @author  Takaya Miyamoto
 * @since   Mon Nov  7 16:14:42 JST 2016
 */
//--------------------------------------------------------------------------

#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <common/common_inc.h>

namespace analysis
{
   //! Set the path of data file
   string set_path(int);
   extern bool use_doi_san_path;
   
   void usage(int);
   
   bool machine_is_little_endian();
   void endian_convert(cdouble*, int);
   void endian_convert( double*, int);
   void endian_convert(    int*, int);
   
   //! Convert the bool to string ( yes or no )
   string bool_to_str(bool);
   //! Convert the string ( yes or no ) to bool
   bool str_to_bool(string);
   //! Convert index xyz -> x,y,z
   void convert_xyz (int*, int*, int*,       int);
   //! Convert index xyzn -> x,y,z,conf
   void convert_xyzn(int*, int*, int*, int*, int);
   
   //! Convert index 0,1,...,L -> -L/2+1,...,L/2
   void convert_origin(int, int, int, int*, int*, int*);
}

namespace analysis
{
   extern int xSIZE;
   extern int ySIZE;
   extern int zSIZE;
   extern int tSIZE;
   extern int Nconf;
   
   extern char data_list[MAX_N_DATA][MAX_LEN_PATH];
   
   void set_data_list(  int,  const char*, ... );
   //! Setting confs. list, and return the number of confs.
   int  set_data_list(  const char* );
   //! Setting data list for path of NBS wave or correlator datas.
   void set_data_list(  const char*, const char*, const char*, const char*
                      , const char*, const char*, const char* );
   //! Wrapper of above 2 functions
   int  set_data_list(  const char*, const char*, const char*, const char*
                      , const char*, const char*, const char*, const char* );
   void set_data     (  int, int, int, int, int );
   
   int  reduced_Ndata();
}

#include <common/analysis_const.h>
#include <common/spin_type.h>
#include <common/hadron_type.h>
#include <common/channel_type.h>

namespace analysis
{
   void make_mean_err(const  double*,  double&,  double&, const int, const bool);
   void make_mean_err(const cdouble*, cdouble&, cdouble&, const int, const bool);
}

#include <common/config_tmp.h>

#endif
