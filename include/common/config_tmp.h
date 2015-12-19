//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Common header file for configuration template class
 * @author  Takaya Miyamoto
 * @since   Wed Sep  2 22:28:05 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef CONFIG_TMP_H
#define CONFIG_TMP_H

#include <common/analysis.h>

//--------------------------------------------------------------------------
/**
 * @brief The template class for configuration
 */
//--------------------------------------------------------------------------
template <class X> class CONFIG {
   
private:
   string class_name, func_name;
   
   int num_conf;
   
   X *data;
   
protected:
   
public:
//============================ For inner index ===========================//
   
//============================== For writing =============================//
   X& operator()(size_t index) { return data[index]; }
//============================== For reading =============================//
   const X& operator()(size_t index) const { return data[index]; }
//======================== Constructor & Destructor ======================//
   CONFIG() {
      class_name = "CONFIG_TMP______________________";
      func_name  = "______________________";
      analysis::route(class_name, func_name, 1);
      
      data       = NULL;
      
      mem_alloc();
   }
   CONFIG(int NUM_CONF) {
      class_name = "CONFIG_TMP______________________";
      func_name  = "______________________";
      analysis::route(class_name, func_name, 1);
      
      data       = NULL;
      
      mem_alloc(NUM_CONF);
   }
   ~CONFIG() {
      if (data != NULL) delete [] data;
      
      func_name = "______________________";
      analysis::route( class_name, func_name, 0 );
   }
//============================= For initialize ===========================//
   void mem_alloc() {
      
      func_name = "mem_alloc_CONFIG______";
      analysis::route( class_name, func_name, 1 );
      
      if (data == NULL) {
         num_conf = analysis::Nconf;
         data = new X[num_conf];
      }
      analysis::route(class_name, func_name, 0);
   }
   void mem_alloc(int NUM_CONF) {
      
      func_name = "mem_alloc_CONFIG______";
      analysis::route( class_name, func_name, 1 );
      
      if (data == NULL) {
         num_conf = NUM_CONF;
         data = new X[num_conf];
      }
      analysis::route(class_name, func_name, 0);
   }
   void mem_del() {
      
      func_name = "mem_delete_CONFIG_____";
      analysis::route(class_name, func_name, 1);
      
      if (data != NULL) {
         delete [] data;   data = NULL;
      }
      analysis::route( class_name, func_name, 0 );
   }
//============================ Operator define ===========================//
   
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
   int  info_conf(){ return num_conf; }
   
   void make_JK_sample();
   void make_mean_err( double*, double*, bool );
   void make_mean_err( cdouble*, cdouble*, bool );
};

#include <common/make_jack_knife_sample.h>

#endif