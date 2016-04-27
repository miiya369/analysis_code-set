//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup R-correlator
 * @brief   Header file for R-correlator class
 * @author  Takaya Miyamoto
 * @since   Wed Feb  3 00:22:57 JST 2016
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
   string class_name, func_name;
   
   cdouble *Rcorr_src_prj;
   
protected:
   
public:
//============================ For inner index ===========================//
   size_t idx(int x,int y,int z, int a)
   {
      return a + 4 *( x + analysis::xSIZE *( y + analysis::ySIZE * z ));
   }
//============================== For writing =============================//
   cdouble& operator()(int x, int y, int z, int a)
   {
      return Rcorr_src_prj[   a + 4
                           *( x + analysis::xSIZE *( y + analysis::ySIZE * z )) ];
   }
   cdouble& operator()(int space,int a)
   {
      return Rcorr_src_prj[ a + 4 * space ];
   }
   cdouble& operator()(size_t index)
   {
      return Rcorr_src_prj[index];
   }
//============================== For reading =============================//
   const cdouble& operator()(int x, int y, int z, int a) const
   {
      return Rcorr_src_prj[   a + 4
                           *( x + analysis::xSIZE *( y + analysis::ySIZE * z )) ];
   }
   const cdouble& operator()(int space,int a) const
   {
      return Rcorr_src_prj[ a + 4 * space ];
   }
   const cdouble& operator()(size_t index) const
   {
      return Rcorr_src_prj[index];
   }
//======================== Constructor & Destructor ======================//
   R_CORRELATOR_SRC_PRJ()
   {
      class_name = "R_CORRELATOR_SRC_PRJ____________";
      func_name = "______________________";
      analysis::route(class_name, func_name, 1);
      
      Rcorr_src_prj = NULL;
   }
   R_CORRELATOR_SRC_PRJ(  const NBS_WAVE_SRC_PRJ &nbs, const CORRELATOR &corr1
                        , const CORRELATOR &corr2, const int time_slice )
   {
      class_name = "R_CORRELATOR_SRC_PRJ____________";
      func_name = "______________________";
      analysis::route(class_name, func_name, 1);
      
      Rcorr_src_prj = NULL;
      
      set( nbs, corr1, corr2, time_slice );
   }
   R_CORRELATOR_SRC_PRJ(  const NBS_WAVE_SRC_PRJ &nbs, const double HAD1_mass
                        , const double HAD2_mass, const int time_slice )
   {
      class_name = "R_CORRELATOR_SRC_PRJ____________";
      func_name = "______________________";
      analysis::route(class_name, func_name, 1);
      
      Rcorr_src_prj = NULL;
      
      set( nbs, HAD1_mass, HAD2_mass, time_slice );
   }
   ~R_CORRELATOR_SRC_PRJ()
   {
      if (Rcorr_src_prj != NULL) delete [] Rcorr_src_prj;
      
      func_name = "______________________";
      analysis::route(class_name, func_name, 0);
   }
//============================= For initialize ===========================//
   void mem_alloc()
   {
      func_name = "mem_alloc_Rcor_SRC_PRJ";
      analysis::route( class_name, func_name, 1 );
      
      if (Rcorr_src_prj == NULL)
      {
         size_t axyzSIZE = 4 * analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
         Rcorr_src_prj = new cdouble[axyzSIZE];
      }
      analysis::route(class_name, func_name, 0);
   }
   void set(  const NBS_WAVE_SRC_PRJ &nbs, const CORRELATOR &corr1
            , const CORRELATOR &corr2, const int time_slice )
   {
      func_name = "set_Rcorr_NBS/corr_PRJ";
      analysis::route(class_name, func_name, 1);
      
      mem_alloc();
      input(nbs, corr1, corr2, time_slice);
      
      func_name = "set_Rcorr_NBS/corr_PRJ";
      analysis::route(class_name, func_name, 0);
   }
   void set(  const NBS_WAVE_SRC_PRJ &nbs
            , const double HAD1_mass, const double HAD2_mass, int time_slice )
   {
      func_name = "set_Rcorr_NBS/mass_PRJ";
      analysis::route(class_name, func_name, 1);
      
      mem_alloc();
      input(nbs, HAD1_mass, HAD2_mass, time_slice);
      
      func_name = "set_Rcorr_NBS/mass_PRJ";
      analysis::route(class_name, func_name, 0);
   }
   void mem_del()
   {
      func_name = "mem_delete_Rcorr___PRJ";
      analysis::route(class_name, func_name, 1);
      
      if (Rcorr_src_prj != NULL) {
         delete [] Rcorr_src_prj;   Rcorr_src_prj = NULL;
      }
      analysis::route(class_name, func_name, 0);
   }
//============================ Operator define ===========================//
   
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
   int info_class() { return CLASS_R_CORRELATOR_SRC_PRJ; }
   int data_size ()
   {
      return 4 * analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   }
   void input(  const NBS_WAVE_SRC_PRJ&
              , const CORRELATOR&, const CORRELATOR&, const int );
   
   void input( const NBS_WAVE_SRC_PRJ&, const double, const double, const int );
};

//--------------------------------------------------------------------------
/**
 * @brief The class for R-correlator
 */
//--------------------------------------------------------------------------
class R_CORRELATOR
{
private:
   string class_name, func_name;
   
   cdouble *Rcorr;
   
protected:
   
public:
//============================ For inner index ===========================//
   size_t idx(int x,int y,int z)
   {
      return x + analysis::xSIZE *( y + analysis::ySIZE * z );
   }
//============================== For writing =============================//
   cdouble& operator()(int x, int y, int z)
   {
      return Rcorr[ x + analysis::xSIZE *( y + analysis::ySIZE * z ) ];
   }
   cdouble& operator()(size_t index)
   {
      return Rcorr[index];
   }
//============================== For reading =============================//
   const cdouble& operator()(int x, int y, int z) const
   {
      return Rcorr[ x + analysis::xSIZE *( y + analysis::ySIZE * z ) ];
   }
   const cdouble& operator()(size_t index) const
   {
      return Rcorr[index];
   }
//======================== Constructor & Destructor ======================//
   R_CORRELATOR()
   {
      class_name = "R_CORRELATOR____________________";
      func_name  = "______________________";
      analysis::route(class_name, func_name, 1);
      
      Rcorr      = NULL;
   }
   R_CORRELATOR(  const NBS_WAVE &nbs, const CORRELATOR &corr1
                , const CORRELATOR &corr2, const int time_slice )
   {
      class_name = "R_CORRELATOR____________________";
      func_name  = "______________________";
      analysis::route(class_name, func_name, 1);
      
      Rcorr = NULL;
      
      set( nbs, corr1, corr2, time_slice );
   }
   R_CORRELATOR(  const NBS_WAVE &nbs, const double HAD1_mass
                , const double HAD2_mass, const int time_slice )
   {
      class_name = "R_CORRELATOR____________________";
      func_name  = "______________________";
      analysis::route(class_name, func_name, 1);
      
      Rcorr = NULL;
      
      set( nbs, HAD1_mass, HAD2_mass, time_slice );
   }
   ~R_CORRELATOR()
   {
      if (Rcorr != NULL) delete [] Rcorr;
      
      func_name = "______________________";
      analysis::route(class_name, func_name, 0);
   }
//============================= For initialize ===========================//
   void mem_alloc()
   {
      func_name = "mem_alloc_Rcorr_______";
      analysis::route( class_name, func_name, 1 );
      
      if (Rcorr == NULL)
      {
         size_t xyzSIZE = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
         Rcorr = new cdouble[xyzSIZE];
      }
      analysis::route(class_name, func_name, 0);
   }
   void set(  const NBS_WAVE &nbs, const CORRELATOR &corr1
            , const CORRELATOR &corr2, const int time_slice )
   {
      func_name = "set_Rcorr_NBS/corr____";
      analysis::route(class_name, func_name, 1);
      
      mem_alloc();
      input(nbs, corr1, corr2, time_slice);
      
      func_name = "set_Rcorr_NBS/corr____";
      analysis::route(class_name, func_name, 0);
   }
   void set(  const NBS_WAVE &nbs, const double HAD1_mass
            , const double HAD2_mass, const int time_slice )
   {
      func_name = "set_Rcorr_NBS/mass____";
      analysis::route(class_name, func_name, 1);
      
      mem_alloc();
      input(nbs, HAD1_mass, HAD2_mass, time_slice);
      
      func_name = "set_Rcorr_NBS/mass____";
      analysis::route(class_name, func_name, 0);
   }
   void mem_del()
   {
      func_name = "mem_delete_Rcorr______";
      analysis::route(class_name, func_name, 1);
      
      if (Rcorr != NULL)
      {
         delete [] Rcorr;   Rcorr = NULL;
      }
      analysis::route(class_name, func_name, 0);
   }
//============================ Operator define ===========================//
   
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
   int info_class() { return CLASS_R_CORRELATOR; }
   int data_size () { return analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; }
   
   void input( const NBS_WAVE&, const CORRELATOR&, const CORRELATOR&, const int );
   void input( const NBS_WAVE&, const double, const double, const int );
};

namespace Rcorrelator
{
   void spin_projection  (  const R_CORRELATOR_SRC_PRJ&, R_CORRELATOR&
                          , const SPIN_TYPE );
   
   void Swave_projection ( R_CORRELATOR& );
   void Swave_division   ( R_CORRELATOR&,         R_CORRELATOR& );
   void Swave_division   ( R_CORRELATOR_SRC_PRJ&, R_CORRELATOR_SRC_PRJ& );
   
   void parity_projection( R_CORRELATOR&);
   void parity_projection( R_CORRELATOR_SRC_PRJ&);
   
   void LP_projection    ( R_CORRELATOR& );
   void remove_angular   ( R_CORRELATOR&, const int, const double );
   
   void mult_YDstar( R_CORRELATOR_SRC_PRJ&, const SPIN_TYPE );
   
   inline int matrix_idx(int row, int column, int rot_type) {
      return column + 4 * ( row + 4 * rot_type );
   }   // rot_type = 0 -> E, 1~6 -> 6C4, 7~9 -> 3C2, 10~17 -> 8C3, 18~23 -> 6C2
}

#endif
