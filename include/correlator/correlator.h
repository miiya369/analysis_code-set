//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Correlator
 * @brief   Header file for correlator class
 * @author  Takaya Miyamoto
 * @since   Fri Sep  4 17:21:25 JST 2015
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
   
   cdouble *corr;
   
protected:
   
public:
//============================ For inner index ===========================//

//============================== For writing =============================//
   cdouble& operator()(size_t index) {
      return corr[index];
   }
//============================== For reading =============================//
   const cdouble& operator()(size_t index) const {
      return corr[index];
   }
//======================== Constructor & Destructor ======================//
   CORRELATOR() {
      class_name = "CORRELATOR______________________";
      func_name  = "______________________";
      analysis::route(class_name, func_name, 1);
      
      corr             = NULL;
   }
   CORRELATOR( HADRON_TYPE HADRON, int iconf, const char* FILE_TYPE ) {
      class_name = "CORRELATOR______________________";
      func_name  = "______________________";
      analysis::route(class_name, func_name, 1);
      
      corr             = NULL;
      
      set( HADRON, iconf, FILE_TYPE );
   }
   ~CORRELATOR() {
      if (corr     != NULL) delete [] corr;
      
      func_name = "______________________";
      analysis::route(class_name, func_name, 0);
   }
//============================= For initialize ===========================//
   void mem_alloc() {
      
      func_name = "mem_alloc_corr________";
      analysis::route( class_name, func_name, 1 );
      
      if (corr == NULL) corr = new cdouble[analysis::tSIZE];
      
      analysis::route(class_name, func_name, 0);
   }
   void set( HADRON_TYPE HADRON, int iconf, const char* FILE_TYPE ) {
      
      func_name = "set_corr______________";
      analysis::route(class_name, func_name, 1);
      
      mem_alloc();
      input(HADRON, iconf, FILE_TYPE);
      
      func_name = "set_corr______________";
      analysis::route(class_name, func_name, 0);
   }
   void mem_del() {
      
      func_name = "mem_delete_corr_______";
      analysis::route(class_name, func_name, 1);
      
      if (corr != NULL) {
         delete [] corr;   corr = NULL;
      }
      analysis::route(class_name, func_name, 0);
   }
//============================ Operator define ===========================//
   
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
   int         info_class()     { return CLASS_CORRELATOR; }
   size_t      info_data_size() { return analysis::tSIZE; }
   
   void input(HADRON_TYPE, int, const char*);
};

#endif
