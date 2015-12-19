//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup scattering length
 * @brief   Header file for scattering length class
 * @author  Takaya Miyamoto
 * @since   Thu Jul 23 18:02:30 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef SCATTERNG_LENGTH_H
#define SCATTERNG_LENGTH_H

#include <common/analysis.h>

//--------------------------------------------------------------------------
/**
 * @brief The class for calculate phase shift
 */
//--------------------------------------------------------------------------
class SCATTERING_LENGTH {
   
private:
   string class_name, func_name;
   
   int    E_min, E_max;
   double mass;
   double hbar_c;
   
   double *phase_shift;
   
protected:
   
public:
   double *scatt_length;
//============================ For inner index ===========================//
   int idx( int conf, int dat ) {
      return conf + analysis::Nconf * dat;
   }
//============================== For writing =============================//
   double& operator()(int energy, int conf) {
      return scatt_length[conf + analysis::Nconf * energy];
   }
   double& operator()(size_t index) {
      return scatt_length[index];
   }
//============================== For reading =============================//
   const double& operator()(int energy, int conf) const {
      return scatt_length[conf + analysis::Nconf * energy];
   }
   
   const double& operator()(size_t index) const {
      return scatt_length[index];
   }
//======================== Constructor & Destructor ======================//
   SCATTERING_LENGTH() {
      class_name = "SCATTERNG_LENGTH________________";
      func_name = "______________________";
      analysis::route( class_name, func_name, 1 );
      
      hbar_c = 197.327;   // MeV*fm
      
      phase_shift  = NULL;
      scatt_length = NULL;
   }
   
   ~SCATTERING_LENGTH() {
      if (phase_shift  != NULL) delete [] phase_shift;
      if (scatt_length != NULL) delete [] scatt_length;
      
      func_name = "______________________";
      analysis::route( class_name, func_name, 0 );
   }
//============================= For initialize ===========================//
   void delete_scatt_length() {
      
      func_name = "delete_scatt_length___";
      analysis::route(class_name, func_name, 1);
      
      if (phase_shift  != NULL) {
         delete [] phase_shift;    phase_shift  = NULL;
      }
      if (scatt_length != NULL) {
         delete [] scatt_length;   scatt_length = NULL;
      }
      analysis::route(class_name, func_name, 0);
   }
//============================ Operator define ===========================//
   
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
   void input_phase_shift( const char* );
   void input_phase_shift( double*, double, int, int );
   void calc_scatt_len( );
   void output_scatt_len_all( const char* );
   void output_scatt_len_err( const char* );
};

#endif
