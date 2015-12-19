//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup scattering length
 * @brief   Header file for scattering length class
 * @author  Takaya Miyamoto
 * @since   Fri Sep  4 00:04:33 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef SCATTERNG_LENGTH_H
#define SCATTERNG_LENGTH_H

#include <observable/observable_base.h>

//--------------------------------------------------------------------------
/**
 * @brief The class for calculate phase shift
 */
//--------------------------------------------------------------------------
class SCATTERING_LENGTH {
   
private:
   string class_name, func_name;
   
   int     data_size;
   double *scatt_length;
   double *energy;
   
protected:
   
public:
//============================ For inner index ===========================//
   
//============================== For writing =============================//
   double& operator()(size_t index) {
      return scatt_length[index];
   }
//============================== For reading =============================//
   const double& operator()(size_t index) const {
      return scatt_length[index];
   }
//======================== Constructor & Destructor ======================//
   SCATTERING_LENGTH() {
      class_name = "SCATTERNG_LENGTH________________";
      func_name = "______________________";
      analysis::route( class_name, func_name, 1 );
      
      scatt_length = NULL;
      energy       = NULL;
   }
   SCATTERING_LENGTH(double E_min, double E_max, double E_dev) {
      class_name = "SCATTERNG_LENGTH________________";
      func_name = "______________________";
      analysis::route( class_name, func_name, 1 );
      
      scatt_length = NULL;
      energy       = NULL;
      
      mem_alloc(E_min, E_max, E_dev);
   }
   SCATTERING_LENGTH(size_t dataSIZE) {
      class_name = "SCATTERNG_LENGTH________________";
      func_name = "______________________";
      analysis::route( class_name, func_name, 1 );
      
      scatt_length = NULL;
      energy       = NULL;
      
      mem_alloc(dataSIZE);
   }
   ~SCATTERING_LENGTH() {
      if (scatt_length != NULL) delete [] scatt_length;
      if (energy       != NULL) delete [] energy;
      
      func_name = "______________________";
      analysis::route( class_name, func_name, 0 );
   }
//============================= For initialize ===========================//
   void mem_alloc(double E_min, double E_max, double E_dev) {
      
      func_name = "mem_alloc_scatt_length";
      analysis::route( class_name, func_name, 1 );
      
      int count = 0;
      for (double loop=E_min; loop<=E_max; loop+=E_dev) count++;
      
      data_size = count;
      
      if (scatt_length == NULL) scatt_length = new double[data_size];
      if (energy       == NULL) energy       = new double[data_size];
      
      double tmp_E = E_min;
      for (int loop=0; loop<data_size; loop++) {
         energy[loop] = tmp_E;
         tmp_E += E_dev;
      }
      analysis::route(class_name, func_name, 0);
   }
   void mem_alloc(size_t dataSIZE) {
      
      func_name = "mem_alloc_scatt_length";
      analysis::route( class_name, func_name, 1 );
      
      data_size = dataSIZE;
      
      if (scatt_length == NULL) scatt_length = new double[data_size];
      if (energy       == NULL) energy       = new double[data_size];
      
      analysis::route(class_name, func_name, 0);
   }
   void set_E(double iE, size_t index) {
      
      func_name = "set_energy____________";
      analysis::route(class_name, func_name, 1);
      
      if (energy != NULL) energy[index] = iE;
      else analysis::error(1,"Scatt length has not been alloc. memory yet !");
      
      analysis::route(class_name, func_name, 0);
   }
   void mem_del() {
      
      func_name = "mem_delete_scattlength";
      analysis::route(class_name, func_name, 1);
      
      if (scatt_length != NULL) {
         delete [] scatt_length;   scatt_length  = NULL;
      }
      if (energy       != NULL) {
         delete [] energy;         energy = NULL;
      }
      analysis::route(class_name, func_name, 0);
   }
//============================ Operator define ===========================//
   
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
   int          info_class()     { return CLASS_SCATTERING_LENGTH; }
   size_t       info_data_size() { return data_size; }
   
   double E(size_t index){ return energy[index]; }
};

#endif
