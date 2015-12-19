//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Correlator
 * @brief   Header file for correlator class
 * @author  Takaya Miyamoto
 * @since   Fri Dec 18 00:21:10 JST 2015
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
   int      Ndata;
   
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
      
      corr       = NULL;
      Ndata      = analysis::tSIZE;
   }
   CORRELATOR( size_t N_DATA ) {
      class_name = "CORRELATOR______________________";
      func_name  = "______________________";
      analysis::route(class_name, func_name, 1);
      
      corr       = NULL;
      Ndata      = N_DATA;
   }
   CORRELATOR( HADRON_TYPE HADRON, int iconf, int MOM, const char* FILE_TYPE ) {
      class_name = "CORRELATOR______________________";
      func_name  = "______________________";
      analysis::route(class_name, func_name, 1);
      
      corr       = NULL;
      Ndata      = analysis::tSIZE;
      set( HADRON, iconf, MOM, FILE_TYPE );
   }
   CORRELATOR( CHANNEL_TYPE CHANNEL, int iconf, SPIN_TYPE SPIN ) {
      class_name = "CORRELATOR______________________";
      func_name  = "______________________";
      analysis::route(class_name, func_name, 1);
      
      corr       = NULL;
      Ndata      = analysis::tSIZE / 2;
      set( CHANNEL, iconf, SPIN );
   }
   ~CORRELATOR() {
      if (corr != NULL) delete [] corr;
      
      func_name = "______________________";
      analysis::route(class_name, func_name, 0);
   }
//============================= For initialize ===========================//
   void mem_alloc() {
      
      func_name = "mem_alloc_corr________";
      analysis::route( class_name, func_name, 1 );
      
      if (corr == NULL) corr = new cdouble[Ndata];
      
      analysis::route(class_name, func_name, 0);
   }
   void mem_alloc( int N_DATA ) {
      
      func_name = "mem_alloc_corr________";
      analysis::route( class_name, func_name, 1 );
      
      if (Ndata != N_DATA) {
         mem_del();   Ndata = N_DATA;
      }
      if (corr == NULL) corr = new cdouble[Ndata];
      
      analysis::route(class_name, func_name, 0);
   }
   void set( HADRON_TYPE HADRON, int iconf, int MOM, const char* FILE_TYPE ) {
      
      func_name = "set_corr______________";
      analysis::route(class_name, func_name, 1);
      
      if (Ndata != analysis::tSIZE) {
         mem_del();   Ndata = analysis::tSIZE;
      }
      mem_alloc();
      input(HADRON, iconf, MOM, FILE_TYPE);
      
      func_name = "set_corr______________";
      analysis::route(class_name, func_name, 0);
   }
   void set( CHANNEL_TYPE CHANNEL, int iconf, SPIN_TYPE SPIN ) {
      
      func_name = "set_corr______________";
      analysis::route(class_name, func_name, 1);
      
      if (Ndata != analysis::tSIZE / 2) {
         mem_del();   Ndata = analysis::tSIZE / 2;
      }
      mem_alloc();
      input(CHANNEL, iconf, SPIN);
      
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
   int  info_class() { return CLASS_CORRELATOR; }
   int  data_size () { return Ndata; }
   
   void input(HADRON_TYPE,  int, int, const char*);
   void input(CHANNEL_TYPE, int, SPIN_TYPE);
};

#endif
