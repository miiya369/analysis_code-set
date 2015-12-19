//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup R-correlator
 * @brief   Header file for R-correlator class
 * @author  Takaya Miyamoto
 * @since   Fri Dec 18 00:19:32 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef R_CORRELATOR_H
#define R_CORRELATOR_H

#include <NBSwave/NBSwave.h>
#include <correlator/correlator.h>

//--------------------------------------------------------------------------
/**
 * @brief The class for R-correlator
 */
//--------------------------------------------------------------------------
class R_CORRELATOR {
    
private:
   string class_name, func_name;
   
   cdouble *Rcorr;
   
protected:
   
public:
//============================ For inner index ===========================//
   size_t idx(int x,int y,int z) {
      return x + analysis::xSIZE *( y + analysis::ySIZE * z );
   }
//============================== For writing =============================//
   cdouble& operator()(int x, int y, int z) {
      return Rcorr[ x + analysis::xSIZE *( y + analysis::ySIZE * z ) ];
   }
   cdouble& operator()(size_t index) {
      return Rcorr[index];
   }
//============================== For reading =============================//
   const cdouble& operator()(int x, int y, int z) const {
      return Rcorr[ x + analysis::xSIZE *( y + analysis::ySIZE * z ) ];
   }
   const cdouble& operator()(size_t index) const {
      return Rcorr[index];
   }
//======================== Constructor & Destructor ======================//
   R_CORRELATOR() {
      class_name = "R_CORRELATOR____________________";
      func_name = "______________________";
      analysis::route(class_name, func_name, 1);
      
      Rcorr      = NULL;
   }
   R_CORRELATOR(  NBS_WAVE &nbs
                , CORRELATOR &corr1, CORRELATOR &corr2, int time_slice ) {
      class_name = "R_CORRELATOR____________________";
      func_name = "______________________";
      analysis::route(class_name, func_name, 1);
      
      Rcorr = NULL;
      
      set( nbs, corr1, corr2, time_slice );
   }
   R_CORRELATOR(  NBS_WAVE &nbs
                , double HAD1_mass, double HAD2_mass, int time_slice ) {
      class_name = "R_CORRELATOR____________________";
      func_name = "______________________";
      analysis::route(class_name, func_name, 1);
      
      Rcorr = NULL;
      
      set( nbs, HAD1_mass, HAD2_mass, time_slice );
   }
   ~R_CORRELATOR() {
      if (Rcorr != NULL) delete [] Rcorr;
      
      func_name = "______________________";
      analysis::route(class_name, func_name, 0);
   }
//============================= For initialize ===========================//
   void mem_alloc() {
      
      func_name = "mem_alloc_Rcorr_______";
      analysis::route( class_name, func_name, 1 );
      
      if (Rcorr == NULL) {
         size_t xyzSIZE = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
         Rcorr = new cdouble[xyzSIZE];
      }
      analysis::route(class_name, func_name, 0);
   }
   void set(  NBS_WAVE &nbs
            , CORRELATOR &corr1, CORRELATOR &corr2, int time_slice ) {
      
      func_name = "set_Rcorr_NBS/corr____";
      analysis::route(class_name, func_name, 1);
      
      mem_alloc();
      input(nbs, corr1, corr2, time_slice);
      
      func_name = "set_Rcorr_NBS/corr____";
      analysis::route(class_name, func_name, 0);
   }
   void set( NBS_WAVE &nbs, double HAD1_mass, double HAD2_mass, int time_slice ) {
      
      func_name = "set_Rcorr_NBS/mass____";
      analysis::route(class_name, func_name, 1);
      
      mem_alloc();
      input(nbs, HAD1_mass, HAD2_mass, time_slice);
      
      func_name = "set_Rcorr_NBS/mass____";
      analysis::route(class_name, func_name, 0);
   }
   void mem_del() {
      
      func_name = "mem_delete_Rcorr______";
      analysis::route(class_name, func_name, 1);
      
      if (Rcorr != NULL) {
         delete [] Rcorr;   Rcorr = NULL;
      }
      analysis::route(class_name, func_name, 0);
   }
//============================ Operator define ===========================//
   
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
   int info_class() { return CLASS_R_CORRELATOR; }
   int data_size () { return analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; }
   
   void input( NBS_WAVE&, CORRELATOR&, CORRELATOR&, int );
   void input( NBS_WAVE&, double, double, int );
};

namespace Rcorrelator {
   
   void Swave_projection ( R_CORRELATOR& );
   void Swave_division   ( R_CORRELATOR&, R_CORRELATOR& );
   void parity_projection( R_CORRELATOR&);
   void LP_projection    ( R_CORRELATOR& );
   void remove_angular   ( R_CORRELATOR&, int, double );
   
   inline int matrix_idx(int row, int column, int rot_type) {
      return column + 4 * ( row + 4 * rot_type );
   }   // rot_type = 0 -> E, 1~6 -> 6C4, 7~9 -> 3C2, 10~17 -> 8C3, 18~23 -> 6C2
}

#endif
