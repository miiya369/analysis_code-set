//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Header file for the folding potential class
 * @brief   Only Gaussian-type potentials can be used.
 * @author  Takaya Miyamoto
 * @since   Mon Nov  7 16:14:42 JST 2016
 */
//--------------------------------------------------------------------------

#ifndef FOLDING_POTENTIAL_H
#define FOLDING_POTENTIAL_H

#include <observable/density_type.h>

//--------------------------------------------------------------------------
/**
 * @brief The class for the folding potential (For Gaussian-type potentials)
 */
//--------------------------------------------------------------------------
class F_POTENTIAL_GAUSS
{
private:
   double *m_data;
   
   int    m_Nplot;
   double m_max_r;
   
protected:
   
public:
//============================ For inner index ===========================//
   
//============================== For writing =============================//
   double& operator()(int a_r) {
      return m_data[a_r];
   }
//============================== For reading =============================//
   const double& operator()(int a_r) const {
      return m_data[a_r];
   }
//======================== Constructor & Destructor ======================//
   F_POTENTIAL_GAUSS();
   F_POTENTIAL_GAUSS(const F_POTENTIAL_GAUSS&);
   F_POTENTIAL_GAUSS(const int, const double);
   ~F_POTENTIAL_GAUSS();
//============================= For initialize ===========================//
   void mem_alloc();
   void set(const int, const double);
   void mem_del();
//============================ Operator define ===========================//
   F_POTENTIAL_GAUSS& operator=(const F_POTENTIAL_GAUSS&);
   F_POTENTIAL_GAUSS& operator+=(const F_POTENTIAL_GAUSS&);
   F_POTENTIAL_GAUSS& operator-=(const F_POTENTIAL_GAUSS&);
   F_POTENTIAL_GAUSS& operator*=(const double);
//============================ Operator helper ===========================//
   friend F_POTENTIAL_GAUSS& operator+(        F_POTENTIAL_GAUSS& lhs
                                       , const F_POTENTIAL_GAUSS& rhs ) {
      return lhs += rhs;
   }
   friend F_POTENTIAL_GAUSS& operator-(        F_POTENTIAL_GAUSS& lhs
                                       , const F_POTENTIAL_GAUSS& rhs ) {
      return lhs -= rhs;
   }
   friend F_POTENTIAL_GAUSS& operator*(        F_POTENTIAL_GAUSS& lhs
                                       , const double             rhs ) {
      return lhs *= rhs;
   }
   friend F_POTENTIAL_GAUSS& operator*(  const double             lhs
                                       ,       F_POTENTIAL_GAUSS& rhs ) {
      return rhs *= lhs;
   }
//=========================== Several functions ==========================//
   int    info_class() { return CLASS_F_POTENTIAL_GAUSS; }
   int    data_size () { return m_Nplot; }
   double max_range () { return m_max_r; }
   const int    info_class() const { return CLASS_F_POTENTIAL_GAUSS; }
   const int    data_size () const { return m_Nplot; }
   const double max_range () const { return m_max_r; }
   
   void calc_fpot(  const double*, const int, const DENSITY_TYPE_BASE&
                  , const double );
   void calc_fpot_test(const double*, const int, const int, const double);
};

namespace analysis {
   //! The function for data output
   template <> void output_data_all<F_POTENTIAL_GAUSS>(  CONFIG<F_POTENTIAL_GAUSS>&
                                                       , const char*);
   template <> void output_data_err<F_POTENTIAL_GAUSS>(  CONFIG<F_POTENTIAL_GAUSS>&
                                                       , const char*, bool);
   template <> void output_data_fit<F_POTENTIAL_GAUSS>(  CONFIG<F_POTENTIAL_GAUSS>&
                                                       , const char*, bool);
}

namespace fpotential_gauss {
   double fpot_r(  const double, const double*, const int
               , const DENSITY_TYPE_BASE&, const double );
}

#endif
