//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Correlator
 * @brief   Header file for correlator class
 * @author  Takaya Miyamoto
 * @since   Tue Jul 21 01:36:40 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef CORRELATOR_H
#define CORRELATOR_H

#include <common/analysis.h>

//--------------------------------------------------------------------------
/**
 * @brief The class for correlator
 */
//--------------------------------------------------------------------------
class CORRELATOR {
   
private:
   string class_name, func_name;
   
protected:
   HADRON_TYPE hadron;
   
public:
   cdouble *corr;
   cdouble *Z_factor;
//============================ For inner index ===========================//
   size_t corr_idx( int t, int ptc_type ){
      return t + analysis::tSIZE * ptc_type;
   }
   size_t nt( int conf, int t ){
      return conf + analysis::Nconf * t;
   }
//============================== For writing =============================//
   cdouble& operator()(int t, int conf) {
      return corr[conf + analysis::Nconf * t];
   }
   cdouble& operator()(size_t index) {
      return corr[index];
   }
//============================== For reading =============================//
   const cdouble& operator()(int t, int conf) const {
      return corr[conf + analysis::Nconf * t];
   }
   const cdouble& operator()(size_t index) const {
      return corr[index];
   }
//======================== Constructor & Destructor ======================//
   CORRELATOR() {
      class_name = "CORRELATOR______________________";
      func_name  = "______________________";
      analysis::route(class_name, func_name, 1);
      
      corr        = NULL;
      Z_factor    = NULL;
   }
   ~CORRELATOR() {
      if (corr     != NULL) delete [] corr;
      if (Z_factor != NULL) delete [] Z_factor;
      
      func_name = "______________________";
      analysis::route(class_name, func_name, 0);
   }
//============================= For initialize ===========================//
   void set_corr( HADRON_TYPE HADRON ) {
      
      func_name = "set_corr______________";
      analysis::route(class_name, func_name, 1);
      
      hadron = HADRON;
      input_corr();
      
      func_name = "set_corr______________";
      analysis::route(class_name, func_name, 0);
   }
   
   void delete_corr() {
      
      func_name = "delete_corr___________";
      analysis::route(class_name, func_name, 1);
      
      if (corr != NULL) {
         delete [] corr;       corr     = NULL;
      }
      if (Z_factor != NULL) {
         delete [] Z_factor;   Z_factor = NULL;
      }
      analysis::route(class_name, func_name, 0);
   }
//============================ Operator define ===========================//
   
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
   void input_corr();
   void make_JK_sample_corr( int );
   void calc_z_factor();
   void output_corr_all( const char* );
   void output_corr_err( const char* );
   void output_corr_fit( const char* );
   void output_effmass_all( const char* );
   void output_effmass_err( const char* );
   void output_z_factor_all( const char* );
   void output_z_factor_err( const char* );
};

#endif
