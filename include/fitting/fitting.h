//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Header file for fitting class
 * @author  Takaya Miyamoto
 * @since   Wed Jul 22 21:40:39 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef FITTING_H
#define FITTING_H

#include <common/analysis.h>
#include <fitting/function_type_const.h>

//--------------------------------------------------------------------------
/**
 * @brief The class for fitting
 */
//--------------------------------------------------------------------------
class FIT {
    
private:
   string class_name, func_name;
   
   int fit_type;
   int Ndata;
   double *cood;
   double *data;
   double *err;
   
   FIT_FUNCTION func_type;
   double *param_ini;
   double *param_out;
   
   bool new_flg_data, new_flg_param, fitting_flg;
   
protected:
    
public:
   double chisq_mean;
   double chisq_err;
   double *param_mean;
   double *param_err;
//============================ For inner index ===========================//
   int idx( int conf, int dat ) {
      return conf + analysis::Nconf * dat;
   }
//============================== For writing =============================//
   
//============================== For reading =============================//
   
//======================== Constructor & Destructor ======================//
   FIT() {
      class_name = "FITTING_________________________";
      func_name = "______________________";
      analysis::route(class_name, func_name, 1);
      
      new_flg_data  = false;
      new_flg_param = false;
      fitting_flg   = false;
   }
   ~FIT() {
      if (new_flg_data) {
         delete [] cood;
         delete [] data;
         delete [] err;
      }
      if (new_flg_param) {
         delete [] param_ini;
         delete [] param_out;
         delete [] param_err;
         delete [] param_mean;
      }
      func_name = "______________________";
      analysis::route(class_name, func_name, 0);
   }
//============================= For initialize ===========================//
   void delete_data() {
      
      func_name = "delete_data___________";
      analysis::route(class_name, func_name, 1);
      
      if (new_flg_data) {
         delete [] cood;
         delete [] data;
         delete [] err;
         new_flg_data = false;
         fitting_flg  = false;
      }
      analysis::route(class_name, func_name, 0);
   }
   
   void reset_func() {
      
      func_name = "reset_func____________";
      analysis::route(class_name, func_name, 1);
      
      if (new_flg_param) {
         delete [] param_ini;
         delete [] param_out;
         delete [] param_err;
         delete [] param_mean;
         new_flg_param = false;
         fitting_flg   = false;
      }
      analysis::route(class_name, func_name, 0);
   }
//============================ Operator define ===========================//
   
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
   void input_data     ( const char* );
   void set_func       ( const char*, double*, int );
   void fit_data_NR    ( int, int, double );
   void print_func_gnu ( bool );
   void print_param    ();
   void output_param   ( const char* );
};

#endif
