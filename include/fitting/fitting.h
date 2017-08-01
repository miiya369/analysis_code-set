//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Header file for fitting class
 * @author  Takaya Miyamoto
 * @since   Wed Apr 27 02:03:26 JST 2016
 */
//--------------------------------------------------------------------------

#ifndef FITTING_H
#define FITTING_H

#include <common/analysis.h>
#include <fitting/function_type.h>

//--------------------------------------------------------------------------
/**
 * @brief The namespace for fitting
 */
//--------------------------------------------------------------------------
namespace fitting
{
   int input_data_binary( const char*, int&, int& );   // for header
   int input_data_binary( const char*, double*, double*, double* );
}

//--------------------------------------------------------------------------
/**
 * @brief The class for fitting
 */
//--------------------------------------------------------------------------
class FIT
{
private:
   double       *param;
   FIT_FUNCTION  func_type;
   
protected:
    
public:
//============================ For inner index ===========================//
   
//============================== For writing =============================//
   double& operator()(int index) {
      return param[index];
   }
//============================== For reading =============================//
   const double& operator()(int index) const {
      return param[index];
   }
//======================== Constructor & Destructor ======================//
   FIT();
   FIT(const FIT&);
   FIT(const char*, const double*);
   FIT(const int,   const double*);
   ~FIT();
//============================= For initialize ===========================//
   void set_func(const char*, const double*);
   void set_func(const int  , const double*);
   void mem_del();
//============================ Operator define ===========================//
   
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
   int info_class()    { return CLASS_FITTING; }
   int info_func_num() { return func_type.number; }
   int data_size()     { return func_type.Nparam; }
   const int info_class()    const { return CLASS_FITTING; }
   const int info_func_num() const { return func_type.number; }
   const int data_size()     const { return func_type.Nparam; }
   
   void   print_func_gnu ();
   void   print_param    ();
   double fit_data_NR    (  const double*, const double*, const double*
                          , const int, const int, const int, const double );
   double fit_data_NR    (  const double*, const double*, const double*
                          , const int, const double, const double, const double );
   double fit_data_NR    (  const double*, const double*, const double*
                          , const int, const double );
};

namespace fitting
{
   void output_param(CONFIG<FIT>&, const char*);
}

#endif
