//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Header file for fitting class
 * @author  Takaya Miyamoto
 * @since   Thu Sep  3 18:29:53 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef FITTING_H
#define FITTING_H

#include <common/analysis.h>
#include <fitting/function_type_const.h>

//--------------------------------------------------------------------------
/**
 * @brief The namespace for fitting
 */
//--------------------------------------------------------------------------
namespace fitting {
   
   void input_data_binary( const char*, int&, int& );   // for header
   void input_data_binary( const char*, double*, double*, double* );
}

//--------------------------------------------------------------------------
/**
 * @brief The class for fitting
 */
//--------------------------------------------------------------------------
class FIT {
    
private:
   string class_name, func_name;
   
   double       *param;
   FIT_FUNCTION  func_type;
   
protected:
    
public:
//============================ For inner index ===========================//
   
//============================== For writing =============================//
   double& operator()(size_t index) {
      return param[index];
   }
//============================== For reading =============================//
   const double& operator()(size_t index) const {
      return param[index];
   }
//======================== Constructor & Destructor ======================//
   FIT() {
      class_name = "FITTING_________________________";
      func_name = "______________________";
      analysis::route(class_name, func_name, 1);
      
      param = NULL;
   }
   FIT( const char* FUNC_NAME, const double *PARAM ) {
      class_name = "FITTING_________________________";
      func_name = "______________________";
      analysis::route(class_name, func_name, 1);
      
      param = NULL;
      set_func( FUNC_NAME, PARAM );
   }
   ~FIT() {
      if (param != NULL) delete [] param;
      
      func_name = "______________________";
      analysis::route(class_name, func_name, 0);
   }
//============================= For initialize ===========================//
   void set_func( const char* FUNC_NAME, const double *PARAM ) {
      
      func_name = "set_func______________";
      analysis::route(class_name, func_name, 1);
      
      func_type.set(FUNC_NAME);
      
      if (param != NULL) delete [] param;
      param = new double[func_type.Nparam];
      
//      printf(" @ fit function = %s\n", func_type.name.c_str());
      
      for (int loop=0; loop<func_type.Nparam; loop++)
         param[loop] = PARAM[loop];
      
      analysis::route(class_name, func_name, 0);
   }
   void mem_del() {
      
      func_name = "mem_delete_FIT________";
      analysis::route(class_name, func_name, 1);
      
      if (param != NULL) {
         delete [] param;   param = NULL;
      }
      analysis::route( class_name, func_name, 0 );
   }
//============================ Operator define ===========================//
   
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
   int          info_class()     { return CLASS_FITTING; }
   int          info_func_num()  { return func_type.number; }
   size_t       info_data_size() { return func_type.Nparam; }
   
   void   print_func_gnu ();
   void   print_param    ();
   double fit_data_NR    ( double*, double*, double*, int, int, double );
   double fit_data_NR    ( double*, double*, double*, int, double );
};

namespace fitting {
   
   void output_param( CONFIG<FIT>&, const char* );
}

#endif
