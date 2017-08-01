//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup observable
 * @brief   Header file for T-matrix class
 * @author  Takaya Miyamoto
 * @since   Tue Jul  5 03:35:27 JST 2016
 */
//--------------------------------------------------------------------------

#ifndef T_MATRIX_H
#define T_MATRIX_H

#include <common/complex_matrix.h>
#include <observable/observable_base.h>
#include <observable/density_type.h>

//--------------------------------------------------------------------------
/**
 * @brief The class for the T-matrix
 */
//--------------------------------------------------------------------------
class T_MATRIX
{
private:
   cmatrix *m_data;
   
   int      m_Ndata;
   int      m_Nch;
   double  *m_Energy;
   
protected:
   
public:
//============================ For inner index ===========================//
   
//============================== For writing =============================//
   cmatrix& operator()(int a_index) {
      return m_data[a_index];
   }
//============================== For reading =============================//
   const cmatrix& operator()(int a_index) const {
      return m_data[a_index];
   }
//======================== Constructor & Destructor ======================//
   T_MATRIX();
   T_MATRIX(const T_MATRIX&);
   T_MATRIX(const double, const double, const double, const int);
   T_MATRIX(const double*, const int, const int);
   ~T_MATRIX();
//============================= For initialize ===========================//
   void mem_alloc(const int, const int);
   void mem_alloc(const double, const double, const double, const int);
   void mem_alloc(const double*, const int, const int);
   void mem_del();
//============================ Operator define ===========================//
   T_MATRIX& operator=(const T_MATRIX&);
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
   int    info_class()          { return CLASS_T_MATRIX; }
   int    data_size ()          { return m_Ndata; }
   int    Nch       ()          { return m_Nch;   }
   double E         (int index) { return m_Energy[index]; }
   const int    info_class()          const { return CLASS_T_MATRIX; }
   const int    data_size ()          const { return m_Ndata; }
   const int    Nch       ()          const { return m_Nch;   }
   const double E         (int index) const { return m_Energy[index]; }
   
   cmatrix Smat(int a_index) {
      return 1.0 + 2.0 * COMP_IMAG * (*this)(a_index);
   }
   const cmatrix Smat(int a_index) const {
      return 1.0 + 2.0 * COMP_IMAG * (*this)(a_index);
   }
   
   void calc_Tmat_dif(  const double*, const double*, const double*
                      , const int*, const int*, const double
                      , const int, const double, const double );
   void calc_Tmat_int(); // Not implemented yet.
   
   // NB. OpenMP is used in the following function.
   void calc_Tmat_fpot(  const DENSITY_TYPE_BASE&, const double*, const double
                       , const int, const int, const double, const double
                       , const double, const double );
};

namespace analysis {
   //! The function for data output
   template <>
   void output_data_all<T_MATRIX>(CONFIG<T_MATRIX>&, const char*);
}

namespace observable
{
   double calc_scatt_length_fpot(  const DENSITY_TYPE_BASE&, const double*
                                 , const int, const int, const double, const double
                                 , const double, const double, const double
                                 , const double);
}

#endif
