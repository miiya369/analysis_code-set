//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup phase shift
 * @brief   Header file for phase shift class
 * @author  Takaya Miyamoto
 * @since   Thu Jul 23 17:31:26 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef PHASE_SHIFT_H
#define PHASE_SHIFT_H

#include <common/analysis.h>
#include <fitting/function_type_const.h>

//--------------------------------------------------------------------------
/**
 * @brief The class for calculate phase shift
 */
//--------------------------------------------------------------------------
class PHASE_SHIFT {
   
private:
   string class_name, func_name;
   
   FIT_FUNCTION func_type;
   
   double *param;
   double lattice_space;
   double ll;
   double del;
   double r_max;
   double test_r_0;
   double test_V_0;
   
   int    E_min, E_max;
   double mass;
   double hbar_c;
   
protected:
   
public:
   double *phase_shift;
//============================ For inner index ===========================//
   int idx( int conf, int dat ) {
      return conf + analysis::Nconf * dat;
   }
//============================== For writing =============================//
   double& operator()(int energy, int conf) {
      return phase_shift[conf + analysis::Nconf * energy];
   }
   double& operator()(size_t index) {
      return phase_shift[index];
   }
//============================== For reading =============================//
   const double& operator()(int energy, int conf) const {
      return phase_shift[conf + analysis::Nconf * energy];
   }

   const double& operator()(size_t index) const {
      return phase_shift[index];
   }
//======================== Constructor & Destructor ======================//
   PHASE_SHIFT() {
      class_name = "PHASE_SHIFT_____________________";
      func_name = "______________________";
      analysis::route(class_name, func_name, 1);
      
      hbar_c = 197.327;   // MeV*fm
      
      phase_shift = NULL;
      param       = NULL;
   }
   ~PHASE_SHIFT() {
      if (param        != NULL) delete [] param;
      if (phase_shift  != NULL) delete [] phase_shift;
      
      func_name = "______________________";
      analysis::route(class_name, func_name, 0);
   }
//============================= For initialize ===========================//
   void delete_param() {
      
      func_name = "delete_param__________";
      analysis::route(class_name, func_name, 1);
      
      if (param != NULL) {
         delete [] param;   param = NULL;
      }
      analysis::route(class_name, func_name, 0);
   }
   void delete_phase_shift() {
      
      func_name = "delete_phase_shift____";
      analysis::route(class_name, func_name, 1);
      
      if (phase_shift != NULL) {
         delete [] phase_shift;   phase_shift = NULL;
      }
      analysis::route(class_name, func_name, 0);
   }
//============================ Operator define ===========================//
   
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
   void input_param( const char* );
   void calc_phase_shift( double, double, double, double, double, int, int );
   void calc_phase_shift_test( double, double, double, double, double, int, int );
   double V( double, int );
   double func_const ( double, int );
   double func_exp   ( double, int );
   double func_gauss ( double, int );
   double func_sgauss( double, int );
   void   output_phase_shift_all( const char* );
   void   output_phase_shift_err( const char* );
   double func_test( double );
};

#endif
