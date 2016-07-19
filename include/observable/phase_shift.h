//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup phase shift
 * @brief   Header file for phase shift class
 * @author  Takaya Miyamoto
 * @since   Tue Jul  5 03:35:27 JST 2016
 */
//--------------------------------------------------------------------------

#ifndef PHASE_SHIFT_H
#define PHASE_SHIFT_H

#include <observable/observable_base.h>

//--------------------------------------------------------------------------
/**
 * @brief The class for calculate phase shift
 */
//--------------------------------------------------------------------------
class PHASE_SHIFT {
   
private:
   string class_name, func_name;
   
   int      Ndata;
   cdouble *phase_shift;
   double  *energy;
   
protected:
   
public:
//============================ For inner index ===========================//
   
//============================== For writing =============================//
   cdouble& operator()(size_t index) {
      return phase_shift[index];
   }
//============================== For reading =============================//
   const cdouble& operator()(size_t index) const {
      return phase_shift[index];
   }
//======================== Constructor & Destructor ======================//
   PHASE_SHIFT() {
      class_name = "PHASE_SHIFT_____________________";
      func_name = "______________________";
      analysis::route(class_name, func_name, 1);
      
      phase_shift = NULL;
      energy      = NULL;
   }
   PHASE_SHIFT(double E_min, double E_max, double E_dev) {
      class_name = "PHASE_SHIFT_____________________";
      func_name = "______________________";
      analysis::route(class_name, func_name, 1);
      
      phase_shift = NULL;
      energy      = NULL;
      
      mem_alloc(E_min, E_max, E_dev);
   }
   PHASE_SHIFT(double* ENERGY, size_t dataSIZE) {
      class_name = "PHASE_SHIFT_____________________";
      func_name = "______________________";
      analysis::route(class_name, func_name, 1);
      
      phase_shift = NULL;
      energy      = NULL;
      
      mem_alloc(ENERGY, dataSIZE);
   }
   ~PHASE_SHIFT() {
      if (phase_shift != NULL) delete [] phase_shift;
      if (energy      != NULL) delete [] energy;
      
      func_name = "______________________";
      analysis::route(class_name, func_name, 0);
   }
//============================= For initialize ===========================//
   void mem_alloc(double E_min, double E_max, double E_dev) {
      
      func_name = "mem_alloc_phase_shift_";
      analysis::route( class_name, func_name, 1 );
      
      int count = 0;
      if (E_dev < 0) for (double loop=E_max; loop>=E_min; loop/=(-E_dev)) count++;
      else for (double loop=E_min; loop<=E_max; loop+=E_dev) count++;
      
      Ndata = count;
      
      if (phase_shift == NULL) phase_shift = new cdouble[Ndata];
      if (energy      == NULL) energy      = new double [Ndata];
      
      if (E_dev < 0) {
         double tmp_E = E_max;
         for (int loop=Ndata-1; loop>=0; loop--) {
            energy[loop] = tmp_E;
            tmp_E /= (-E_dev);
         }
         
      } else {
         double tmp_E = E_min;
         for (int loop=0; loop<Ndata; loop++) {
            energy[loop] = tmp_E;
            tmp_E += E_dev;
         }
      }
      analysis::route(class_name, func_name, 0);
   }
   void mem_alloc(double* ENERGY, size_t dataSIZE) {
      
      func_name = "mem_alloc_phase_shift_";
      analysis::route( class_name, func_name, 1 );
      
      Ndata = dataSIZE;
      
      if (phase_shift == NULL) phase_shift = new cdouble[Ndata];
      if (energy      == NULL) energy      = new double [Ndata];
      
      for (int loop=0; loop<Ndata; loop++) energy[loop] = ENERGY[loop];
      
      analysis::route(class_name, func_name, 0);
   }
   void mem_del() {
      
      func_name = "mem_delete_phase_shift";
      analysis::route(class_name, func_name, 1);
      
      if (phase_shift != NULL) {
         delete [] phase_shift;   phase_shift = NULL;
      }
      if (energy      != NULL) {
         delete [] energy;        energy = NULL;
      }
      analysis::route(class_name, func_name, 0);
   }
//============================ Operator define ===========================//
   
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
   int          info_class()     { return CLASS_PHASE_SHIFT; }
   size_t       data_size()      { return Ndata; }
   
   double E(size_t index){ return energy[index]; }
};

namespace observable {
   
   void output_phase_shift    ( const char*, PHASE_SHIFT& );
   void output_phase_shift_all( const char*, CONFIG<PHASE_SHIFT>& );
   void output_phase_shift_err( const char*, CONFIG<PHASE_SHIFT>&, bool );

   void calc_phase_shift_dif(  PHASE_SHIFT*, double*, double*, double*
                             , int, int*, int*, double, double, double );
   void calc_phase_shift_int(  PHASE_SHIFT*, double*, double*, double*
                             , int, int, int, double, int );
}

#endif
