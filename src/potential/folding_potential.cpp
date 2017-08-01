//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Definition for the folding potential class
 * @author  Takaya Miyamoto
 * @since   Mon Nov  7 16:14:42 JST 2016
 */
//--------------------------------------------------------------------------

#include <potential/folding_potential.h>

//======================== Constructor & Destructor ======================//
F_POTENTIAL_GAUSS::F_POTENTIAL_GAUSS() {
   DEBUG_LOG
   
   m_data = NULL;
   
   m_Nplot = 0;
   m_max_r = 0.0;
}
F_POTENTIAL_GAUSS::F_POTENTIAL_GAUSS(const F_POTENTIAL_GAUSS& other) {
   DEBUG_LOG
   
   m_Nplot = other.data_size();
   m_max_r = other.max_range();
   
   m_data = NULL;
   mem_alloc();
   
   for (int n=0; n<(*this).data_size(); n++)
      (*this)(n) = other(n);
}
F_POTENTIAL_GAUSS::F_POTENTIAL_GAUSS(const int a_Nplot, const double a_max_r) {
   DEBUG_LOG
   
   set(a_Nplot, a_max_r);
}
F_POTENTIAL_GAUSS::~F_POTENTIAL_GAUSS() {
   DEBUG_LOG
   
   if (m_data != NULL) delete [] m_data;
}
//============================= For initialize ===========================//
void F_POTENTIAL_GAUSS::mem_alloc() {
   DEBUG_LOG
   
   if ((*this).data_size() == 0)
      ERROR_COMMENTS("The data size have not been set yet.");
   
   if (m_data == NULL) m_data = new double[(*this).data_size()];
}
void F_POTENTIAL_GAUSS::set(const int a_Nplot, const double a_max_r) {
   DEBUG_LOG
   
   m_Nplot = a_Nplot;
   m_max_r = a_max_r;
   
   mem_alloc();
}
void F_POTENTIAL_GAUSS::mem_del() {
   DEBUG_LOG
   
   if (m_data != NULL) {
      delete [] m_data;
      m_data = NULL;
   }
}
//============================ Operator define ===========================//
F_POTENTIAL_GAUSS& F_POTENTIAL_GAUSS::operator=(const F_POTENTIAL_GAUSS& rhs) {
   mem_alloc();
   
   for (int n=0; n<(*this).data_size(); n++)
      (*this)(n) = rhs(n);
   
   return *this;
}
F_POTENTIAL_GAUSS& F_POTENTIAL_GAUSS::operator+=(const F_POTENTIAL_GAUSS& rhs) {
   if (m_data == NULL) ERROR_COMMENTS("The data have not been allocated yet.");
   
   for (int n=0; n<(*this).data_size(); n++)
      (*this)(n) += rhs(n);
   
   return *this;
}
F_POTENTIAL_GAUSS& F_POTENTIAL_GAUSS::operator-=(const F_POTENTIAL_GAUSS& rhs) {
   if (m_data == NULL) ERROR_COMMENTS("The data have not been allocated yet.");
   
   for (int n=0; n<(*this).data_size(); n++)
      (*this)(n) -= rhs(n);
   
   return *this;
}
F_POTENTIAL_GAUSS& F_POTENTIAL_GAUSS::operator*=(const double rhs) {
   if (m_data == NULL) ERROR_COMMENTS("The data have not been allocated yet.");
   
   for (int n=0; n<(*this).data_size(); n++)
      (*this)(n) *= rhs;
   
   return *this;
}
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
void F_POTENTIAL_GAUSS::calc_fpot(  const double* a_params, const int a_Nparam
                                  , const DENSITY_TYPE_BASE &a_density
                                  , const double a_Ndiv ) {
   DEBUG_LOG
   
   double a_sqr = pow(a_density.RA(), 2.0);
   
   /// Simpson's rule is used for the integral culculation ///
   
   double Ndiv      = a_Ndiv;
   double div_rr    = 1.0 / (2.0*Ndiv); // = (b-a)/(2*N)
   double factor[2] = {2.0, 4.0};
   double Integrand, rr, tmpI, tmp_exp1, tmp_exp2, pn, qn2;
   
#define r_org sqrt(-2.0*a_sqr*log(rr))
   
   double div_r = (m_max_r) / double(m_Nplot), r = 0.0;
   for (int iplot=0; iplot<m_Nplot; iplot++)
   {
      (*this)(iplot) = 0.0;
      r += div_r;
      for (int iparam=0; iparam<a_Nparam; iparam+=2) {
         pn   =     a_params[iparam];
         qn2  = pow(a_params[iparam+1], 2.0);
         rr   = 0.0; // = a
         tmpI = 0.0; // Integrand = 0.0 at rr = 0
         for (int i=1; i<2*Ndiv; i++) {
            rr       += div_rr;
            tmp_exp1  = exp(-pow(r-r_org, 2.0) / qn2);
            tmp_exp2  = exp(-pow(r+r_org, 2.0) / qn2);
            Integrand = (a_sqr/rr) * a_density(r_org) * (tmp_exp1-tmp_exp2);
            tmpI += factor[i%2] * Integrand;
         }
         rr   += div_rr;
         tmpI  = (div_rr/3.0) * (tmpI + 0.0); // Integrand = 0.0 at rr = 1
         
         (*this)(iplot) += (pn*qn2) * tmpI;
      }
      (*this)(iplot) *= (PI/r);
   }
#undef r_trans
}

void F_POTENTIAL_GAUSS::calc_fpot_test(  const double* a_params, const int a_Nparam
                                       , const int a_A, const double a_RA ) {
   DEBUG_LOG
   
   double pn, qn, Pn, Qn;
   double div_r = (m_max_r) / double(m_Nplot), r = 0.0;
   for (int iplot=0; iplot<m_Nplot; iplot++)
   {
      (*this)(iplot) = 0.0;
      r += div_r;
      for (int iparam=0; iparam<a_Nparam; iparam+=2) {
         pn = a_params[iparam  ];
         qn = a_params[iparam+1];
         Pn = a_A * pn * pow(qn, 3.0) / pow(a_RA*a_RA + qn*qn, 1.5);
         Qn = sqrt(a_RA*a_RA + qn*qn);
         
         (*this)(iplot) += Pn * exp(-pow(r/Qn, 2.0));
      }
   }
}

double fpotential_gauss::fpot_r(  const double a_r
                                , const double* a_params, const int a_Nparam
                                , const DENSITY_TYPE_BASE &a_density
                                , const double a_Ndiv ) {
   double a_sqr = pow(a_density.RA(), 2.0);
   
   /// Simpson's rule is used for the integral culculation ///
   
   double Ndiv      = a_Ndiv;
   double div_rr    = 1.0 / (2.0*Ndiv); // = (b-a)/(2*N)
   double factor[2] = {2.0, 4.0};
   double Integrand, rr, tmpI, tmp_exp1, tmp_exp2, pn, qn2;
   
#define r_org sqrt(-2.0*a_sqr*log(rr))
   
   double ret = 0.0;
   for (int iparam=0; iparam<a_Nparam; iparam+=2) {
      pn   =     a_params[iparam];
      qn2  = pow(a_params[iparam+1], 2.0);
      rr   = 0.0; // = a
      tmpI = 0.0; // Integrand = 0.0 at rr = 0
      for (int i=1; i<2*Ndiv; i++) {
         rr       += div_rr;
         tmp_exp1  = exp(-pow(a_r-r_org, 2.0) / qn2);
         tmp_exp2  = exp(-pow(a_r+r_org, 2.0) / qn2);
         Integrand = (a_sqr/rr) * a_density(r_org) * (tmp_exp1-tmp_exp2);
         tmpI += factor[i%2] * Integrand;
      }
      rr   += div_rr;
      tmpI  = (div_rr/3.0) * (tmpI + 0.0); // Integrand = 0.0 at rr = 1
      
      ret += (pn*qn2) * tmpI;
   }
#undef r_trans
   
   return (ret * (PI/a_r));
}
