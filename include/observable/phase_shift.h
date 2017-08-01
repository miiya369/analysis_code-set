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

#include <observable/Tmatrix.h>

//--------------------------------------------------------------------------
/**
 * @brief The class for calculate phase shift
 */
//--------------------------------------------------------------------------
class PHASE_SHIFT
{
private:
   double *m_data;
   
   int      m_Ndata;
   double  *m_Energy;
   
protected:
   
public:
//============================ For inner index ===========================//
   
//============================== For writing =============================//
   double& operator()(int a_index) {
      return m_data[a_index];
   }
//============================== For reading =============================//
   const double& operator()(int a_index) const {
      return m_data[a_index];
   }
//======================== Constructor & Destructor ======================//
   PHASE_SHIFT();
   PHASE_SHIFT(const PHASE_SHIFT&);
   PHASE_SHIFT(const T_MATRIX&);
   PHASE_SHIFT(const double, const double, const double);
   PHASE_SHIFT(const double*, const int);
   ~PHASE_SHIFT();
//============================= For initialize ===========================//
   void mem_alloc(const int);
   void mem_alloc(const T_MATRIX&);
   void mem_alloc(const double, const double, const double);
   void mem_alloc(const double*, const int);
   void mem_del();
//============================ Operator define ===========================//
   PHASE_SHIFT& operator=(const PHASE_SHIFT&);
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
   int    info_class()          { return CLASS_PHASE_SHIFT; }
   int    data_size ()          { return m_Ndata; }
   double E         (int index) { return m_Energy[index]; }
   const int    info_class()          const { return CLASS_PHASE_SHIFT; }
   const int    data_size ()          const { return m_Ndata; }
   const double E         (int index) const { return m_Energy[index]; }
};

namespace analysis {
   //! The function for data output
   template <>
   void output_data_all<PHASE_SHIFT>(CONFIG<PHASE_SHIFT>&, const char*);
   template <>
   void output_data_err<PHASE_SHIFT>(CONFIG<PHASE_SHIFT>&, const char*, bool);
}

namespace phase_shift
{
   void input_phase_shift(const char*, int&, int&);   // for header
   void input_phase_shift(const char*, double*, double*);
   
   void calc_phase_shift(T_MATRIX&, PHASE_SHIFT*);
   void calc_phase_shift_eq(  PHASE_SHIFT&, double*, double
                            , int, int, double, double, double );
}

#endif
