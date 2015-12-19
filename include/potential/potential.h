//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Header file for potential class
 * @author  Takaya Miyamoto
 * @since   Sat Sep 12 01:11:13 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef POTENTIAL_H
#define POTENTIAL_H

#include <R_correlator/R_correlator.h>

//--------------------------------------------------------------------------
/**
 * @brief The namespace for potential calculation
 */
//--------------------------------------------------------------------------
namespace potential {
   
   string kernel(  R_CORRELATOR&
                 , R_CORRELATOR&
                 , R_CORRELATOR&
                 , R_CORRELATOR&, double );
   
   string laplacian(  R_CORRELATOR&
                    , R_CORRELATOR&, double );
   
   string first_time_diff(  R_CORRELATOR&
                          , R_CORRELATOR&
                          , R_CORRELATOR& );
   
   string second_time_diff(  R_CORRELATOR&
                           , R_CORRELATOR&
                           , R_CORRELATOR&
                           , R_CORRELATOR&, double );
}

//--------------------------------------------------------------------------
/**
 * @brief The class for potential
 */
//--------------------------------------------------------------------------
class POTENTIAL {
    
private:
   string class_name, func_name;
   
   cdouble *potential;
   
protected:
   
public:
//============================ For inner index ===========================//
   size_t idx(int x,int y,int z) {
      return x + analysis::xSIZE *( y + analysis::ySIZE * z );
   }
//============================== For writing =============================//
   cdouble& operator()(int x, int y, int z) {
      return potential[x + analysis::xSIZE *( y + analysis::ySIZE * z )];
   }
   cdouble& operator()(size_t index) {
      return potential[index];
   }
//============================== For reading =============================//
   const cdouble& operator()(int x, int y, int z) const {
      return potential[x + analysis::xSIZE *( y + analysis::ySIZE * z )];
   }
   const cdouble& operator()(size_t index) const {
      return potential[index];
   }
//======================== Constructor & Destructor ======================//
   POTENTIAL() {
      class_name = "POTENTIAL_______________________";
      func_name = "______________________";
      analysis::route(class_name, func_name, 1);

      potential    = NULL;
   }
   ~POTENTIAL() {
      if (potential != NULL) delete [] potential;
      
      func_name = "______________________";
      analysis::route(class_name, func_name, 0);
   }
//============================= For initialize ===========================//
   void mem_alloc() {
      
      func_name = "mem_alloc_potential___";
      analysis::route( class_name, func_name, 1 );
      
      if (potential == NULL) {
         size_t xyzSIZE = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
         potential = new cdouble[xyzSIZE];
      }
      analysis::route(class_name, func_name, 0);
   }
   void set(R_CORRELATOR &K_Rcorr, R_CORRELATOR &Rcorr) {
      
      func_name = "set_pot_K_R_/_R_______";
      analysis::route(class_name, func_name, 1);
      
      mem_alloc();
      input(K_Rcorr, Rcorr);
      
      func_name = "set_pot_K_R_/_R_______";
      analysis::route(class_name, func_name, 0);
   }
   void set(R_CORRELATOR &Rcorr) {
      
      func_name = "set_pot_R_____________";
      analysis::route(class_name, func_name, 1);
      
      mem_alloc();
      input(Rcorr);
      
      func_name = "set_pot_R_____________";
      analysis::route(class_name, func_name, 0);
   }
   void mem_del() {
      
      func_name = "mem_delete_pot________";
      analysis::route(class_name, func_name, 1);
      
      if (potential != NULL) {
         delete [] potential;
         potential = NULL;
      }
      analysis::route(class_name, func_name, 0);
   }
//============================ Operator define ===========================//
   
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
   int          info_class()    { return CLASS_POTENTIAL; }
   size_t       info_data_size(){
      return analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   }
   
   void input( R_CORRELATOR&, R_CORRELATOR& );
   void input( R_CORRELATOR& );
   void set_from_binary( const char* );
   
   cdouble momentum_rep( double );
};

#endif
