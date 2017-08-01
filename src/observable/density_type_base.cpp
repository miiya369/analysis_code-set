//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup observable
 * @brief   BASE-Definition of the density function type for nuclei
 * @author  Takaya Miyamoto
 * @since   Sat Nov 12 03:33:54 JST 2016
 */
//--------------------------------------------------------------------------

#include <common/analysis.h>
#include <observable/density_type_base.h>

//======================== Constructor & Destructor ======================//
DENSITY_TYPE_BASE::DENSITY_TYPE_BASE() {
   m_A    = 0;
   m_r0   = 0.0;
   m_RA   = 0.0;
   m_rho0 = 0.0;
}
DENSITY_TYPE_BASE::DENSITY_TYPE_BASE(const DENSITY_TYPE_BASE& other) {
   m_A    = other.A();
   m_r0   = other.r0();
   m_RA   = other.RA();
   m_rho0 = other.rho0();
}
DENSITY_TYPE_BASE::DENSITY_TYPE_BASE(const int a_A, const double a_r0) {
   set_base(a_A, a_r0);
}

//============================= For initialize ===========================//
void DENSITY_TYPE_BASE::set_base(const int a_A, const double a_r0) {
   m_A  = a_A;
   m_r0 = a_r0;
   set_RA();
}

void DENSITY_TYPE_BASE::set(const int a_A, const double a_r0) {
   set_base(a_A, a_r0);
}
void DENSITY_TYPE_BASE::set(  const int a_A, const double a_r0
                            , const double a_aA) {
   set_base(a_A, a_r0);
}
//==================== definition of fit function type ===================//

//=========================== Several functions ==========================//
void DENSITY_TYPE_BASE::set_RA() {
   m_RA = m_r0 * pow((double)m_A, 1.0/3.0);
}
