//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup R-correlator
 * @brief   Header file for R-correlator class
 * @author  Takaya Miyamoto
 * @since   Mon Nov  7 19:51:54 JST 2016
 */
//--------------------------------------------------------------------------

#ifndef R_CORRELATOR_H
#define R_CORRELATOR_H

#include <NBSwave/NBSwave.h>
#include <correlator/correlator.h>

//--------------------------------------------------------------------------
/**
 * @brief The class for R-correlator spin projected at source part
 */
//--------------------------------------------------------------------------
class R_CORRELATOR_SRC_PRJ
{
private:
   cdouble *Rcorr_src_prj;
   
protected:
   
public:
//============================ For inner index ===========================//
   int idx(int x,int y,int z, int a) {
      return a + 4 *( x + analysis::xSIZE *( y + analysis::ySIZE * z ));
   }
//============================== For writing =============================//
   cdouble& operator()(int x, int y, int z, int a) {
      return Rcorr_src_prj[   a + 4
                           *( x + analysis::xSIZE *( y + analysis::ySIZE * z )) ];
   }
   cdouble& operator()(int space,int a) {
      return Rcorr_src_prj[ a + 4 * space ];
   }
   cdouble& operator()(int index) {
      return Rcorr_src_prj[index];
   }
//============================== For reading =============================//
   const cdouble& operator()(int x, int y, int z, int a) const {
      return Rcorr_src_prj[   a + 4
                           *( x + analysis::xSIZE *( y + analysis::ySIZE * z )) ];
   }
   const cdouble& operator()(int space, int a) const {
      return Rcorr_src_prj[ a + 4 * space ];
   }
   const cdouble& operator()(int index) const {
      return Rcorr_src_prj[index];
   }
//======================== Constructor & Destructor ======================//
   R_CORRELATOR_SRC_PRJ();
   R_CORRELATOR_SRC_PRJ(  const R_CORRELATOR_SRC_PRJ&);
   R_CORRELATOR_SRC_PRJ(  const NBS_WAVE_SRC_PRJ&
                        , const CORRELATOR&, const CORRELATOR&, const int);
   R_CORRELATOR_SRC_PRJ(  const NBS_WAVE_SRC_PRJ&
                        , const double, const double, const int);
   ~R_CORRELATOR_SRC_PRJ();
//============================= For initialize ===========================//
   void mem_alloc();
   void set(  const NBS_WAVE_SRC_PRJ&
            , const CORRELATOR&, const CORRELATOR&, const int);
   void set(  const NBS_WAVE_SRC_PRJ&
            , const double, const double, const int);
   void mem_del();
//============================ Operator define ===========================//
   R_CORRELATOR_SRC_PRJ& operator=(const R_CORRELATOR_SRC_PRJ&);
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
   int info_class() { return CLASS_R_CORRELATOR_SRC_PRJ; }
   int data_size () {
      return 4 * analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   }
   const int info_class() const { return CLASS_R_CORRELATOR_SRC_PRJ; }
   const int data_size () const {
      return 4 * analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   }
   
   void input(  const NBS_WAVE_SRC_PRJ&
              , const CORRELATOR&, const CORRELATOR&, const int );
   void input(  const NBS_WAVE_SRC_PRJ&, const double, const double, const int );
};

//--------------------------------------------------------------------------
/**
 * @brief The class for R-correlator
 */
//--------------------------------------------------------------------------
class R_CORRELATOR
{
private:
   cdouble *Rcorr;
   
protected:
   
public:
//============================ For inner index ===========================//
   int idx(int x,int y,int z) {
      return x + analysis::xSIZE *( y + analysis::ySIZE * z );
   }
//============================== For writing =============================//
   cdouble& operator()(int x, int y, int z) {
      return Rcorr[ x + analysis::xSIZE *( y + analysis::ySIZE * z ) ];
   }
   cdouble& operator()(int index) {
      return Rcorr[index];
   }
//============================== For reading =============================//
   const cdouble& operator()(int x, int y, int z) const {
      return Rcorr[ x + analysis::xSIZE *( y + analysis::ySIZE * z ) ];
   }
   const cdouble& operator()(int index) const {
      return Rcorr[index];
   }
//======================== Constructor & Destructor ======================//
   R_CORRELATOR();
   R_CORRELATOR(const R_CORRELATOR&);
   R_CORRELATOR(const NBS_WAVE&, const CORRELATOR&, const CORRELATOR&, const int);
   R_CORRELATOR(const NBS_WAVE&, const double, const double, const int);
   ~R_CORRELATOR();
//============================= For initialize ===========================//
   void mem_alloc();
   void set(const NBS_WAVE&, const CORRELATOR&, const CORRELATOR&, const int);
   void set(const NBS_WAVE&, const double, const double, const int);
   void mem_del();
//============================ Operator define ===========================//
   R_CORRELATOR& operator=(const R_CORRELATOR&);
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
   int info_class() { return CLASS_R_CORRELATOR; }
   int data_size () {
      return analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   }
   const int info_class() const { return CLASS_R_CORRELATOR; }
   const int data_size () const {
      return analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   }
   
   void input(const NBS_WAVE&, const CORRELATOR&, const CORRELATOR&, const int);
   void input(const NBS_WAVE&, const double, const double, const int);
};

namespace Rcorrelator
{
   void spin_projection  (  const R_CORRELATOR_SRC_PRJ&, R_CORRELATOR&
                          , const SPIN_TYPE );
   
   void Swave_projection (R_CORRELATOR&);
   void Swave_division   (R_CORRELATOR&,         R_CORRELATOR&);
   void Swave_division   (R_CORRELATOR_SRC_PRJ&, R_CORRELATOR_SRC_PRJ&);
   
   void parity_projection(R_CORRELATOR&);
   void parity_projection(R_CORRELATOR_SRC_PRJ&);
   
   void LP_projection    (R_CORRELATOR&);
   void remove_angular   (R_CORRELATOR&, const int, const double);
   
   void mult_YDstar(R_CORRELATOR_SRC_PRJ&, const SPIN_TYPE);
   
   inline int matrix_idx(int row, int column, int rot_type) {
      return column + 4 * ( row + 4 * rot_type );
   } // rot_type = 0 -> E, 1~6 -> 6C4, 7~9 -> 3C2, 10~17 -> 8C3, 18~23 -> 6C2
}

#endif
