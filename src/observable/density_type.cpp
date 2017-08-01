//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup observable
 * @brief   Definition of the density function type for nuclei
 * @author  Takaya Miyamoto
 * @since   Sat Nov 12 03:33:54 JST 2016
 */
//--------------------------------------------------------------------------

#include <common/analysis.h>
#include <observable/density_type.h>

//--------------------------------------------------------------------------
/**
 * @brief The class for the density function (Gaussian-type)
 */
//--------------------------------------------------------------------------
//======================== Constructor & Destructor ======================//
DENSITY_GAUSSIAN::DENSITY_GAUSSIAN() : DENSITY_TYPE_BASE() {}
DENSITY_GAUSSIAN::DENSITY_GAUSSIAN(const DENSITY_GAUSSIAN& other)
: DENSITY_TYPE_BASE(other) {}
DENSITY_GAUSSIAN::DENSITY_GAUSSIAN(const int a_A, const double a_r0)
: DENSITY_TYPE_BASE(a_A, a_r0) { set_rho0(); }

//============================= For initialize ===========================//
void DENSITY_GAUSSIAN::set(const int a, const double b, const double c) {
   ERROR_COMMENTS("Wrong initialized function has been used.");
}
void DENSITY_GAUSSIAN::set(const int a_A, const double a_r0) {
   set_base(a_A, a_r0);
   set_rho0();
}
//==================== definition of fit function type ===================//
const double DENSITY_GAUSSIAN::rho(double a_r) const {
   return m_rho0 * exp(-pow(a_r/m_RA, 2.0));
}
//=========================== Several functions ==========================//
void DENSITY_GAUSSIAN::set_rho0() {
   m_rho0 = m_A / (pow(PI, 1.5) * pow(m_RA, 3.0));
}

//--------------------------------------------------------------------------
/**
 * @brief The class for the density function (Woods-Saxon-type)
 */
//--------------------------------------------------------------------------
//======================== Constructor & Destructor ======================//
DENSITY_WOODS_SAXON::DENSITY_WOODS_SAXON() : DENSITY_TYPE_BASE() {
   m_aA = 0.0;
}
DENSITY_WOODS_SAXON::DENSITY_WOODS_SAXON(const DENSITY_WOODS_SAXON& other)
: DENSITY_TYPE_BASE(other) {
   m_aA = other.aA();
}
DENSITY_WOODS_SAXON::DENSITY_WOODS_SAXON(  const int a_A, const double a_r0
                                         , const double a_aA )
: DENSITY_TYPE_BASE(a_A, a_r0) {
   m_aA = a_aA;
   set_rho0();
}

//============================= For initialize ===========================//
void DENSITY_WOODS_SAXON::set(const int a, const double b) {
   ERROR_COMMENTS("Wrong initialized function has been used.");
}
void DENSITY_WOODS_SAXON::set(  const int a_A, const double a_r0
                              , const double a_aA) {
   set_base(a_A, a_r0);
   m_aA = a_aA;
   set_rho0();
}
//==================== definition of fit function type ===================//
const double DENSITY_WOODS_SAXON::rho(double a_r) const {
   return m_rho0 / (1.0 + exp((a_r-m_RA)/m_aA));
}
//=========================== Several functions ==========================//
void DENSITY_WOODS_SAXON::set_rho0() {
   double a_sqr = pow(m_RA, 2.0);
   
   /// Simpson's rule is used for the integral culculation ///
   
   double Ndiv      = 1e+6;
   double div_rr    = 1.0 / (2.0*Ndiv); // = (b-a)/(2*N)
   double rr        = 0.0;              // = a
   double factor[2] = {2.0, 4.0};

#define r_org     sqrt(-2.0*a_sqr*log(rr))
#define Integrand ((a_sqr * r_org) / (rr * (1.0 + exp((r_org-m_RA)/m_aA))))

   double tmpI = 0.0; // Integrand = 0.0 at rr = 0
   for (int i=1; i<2*Ndiv; i++) {
      rr   += div_rr;
      tmpI += factor[i%2] * Integrand;
   }
   rr   += div_rr;
   tmpI  = (div_rr/3.0) * (tmpI + 0.0); // Integrand = 0.0 at rr = 1

#undef Integrand
#undef r_trans
   
   m_rho0 = m_A / (4.0 * PI * tmpI);
}
