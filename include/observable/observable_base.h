//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup observable
 * @brief   Base header file for each observable class
 * @author  Takaya Miyamoto
 * @since   Sat Nov 12 03:33:54 JST 2016
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
namespace observable
{   
   void input_param(const char*, int&, int&, int&);   // for header
   void input_param(const char*, double*);
   
   double V(const double, const double*, const int, const int);
   double func_const (const double, const double*, const int);
   double func_exp   (const double, const double*, const int);
   double func_gauss (const double, const double*, const int);
   double func_sgauss(const double, const double*, const int);
   double func_1g1y  (const double, const double*, const int);
   double func_test  (const double, const double*, const int);
   
   double Vp(const double, const double, const double*, const int, const int);
   double func_gauss_mom (const double, const double, const double*, const int);
   
   double calc_scatt_length(  const double*, const int, const int
                            , const double, const double, const double, const int
                            , const double, const double );
   
   cdouble Smat_Cplane(  double, double, double*, double, int, int
                       , double, double, double );
}

#endif
