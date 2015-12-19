//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup observable
 * @brief   Base header file for each observable class
 * @author  Takaya Miyamoto
 * @since   Mon Oct 19 23:54:58 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef OBSERVABLE_BASE_H
#define OBSERVABLE_BASE_H

#include <common/analysis.h>

//--------------------------------------------------------------------------
/**
 * @brief The namespace for observable
 */
//--------------------------------------------------------------------------
namespace observable {
   
   void input_param( const char*, int&, int&, int& );   // for header
   void input_param( const char*, double* );
   
   void input_phase_shift( const char*, int&, int& );   // for header
   void input_phase_shift( const char*, double*, cdouble* );
   
   double V( double, double*, int, int );
   double func_const ( double, double*, int );
   double func_exp   ( double, double*, int );
   double func_gauss ( double, double*, int );
   double func_sgauss( double, double*, int );
   double func_test  ( double, double*, int );
   
   double Vp( double, double, double*, int, int );
   double func_gauss_mom ( double, double, double*, int );
   
   cdouble calc_scatt_length(  double*, int, int, double, double
                             , double, double, double );
   
   cdouble Smat_Cplane(  double, double, double*, double, int, int
                       , double, double, double );
}

#endif
