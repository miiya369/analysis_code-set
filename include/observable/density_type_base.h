//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup observable
 * @brief   Header File for BASE-definition of density function type for nuclei
 * @author  Takaya Miyamoto
 * @since   Sun Jun 19 03:26:49 JST 2016
 */
//--------------------------------------------------------------------------

#ifndef DENSITY_TYPE_BASE_H
#define DENSITY_TYPE_BASE_H

#include <common/analysis.h>

//--------------------------------------------------------------------------
/**
 * @brief The base-class for the density function type
 */
//--------------------------------------------------------------------------
class DENSITY_TYPE_BASE
{
protected:
   int    m_A;
   double m_r0;
   double m_RA;
   double m_rho0;

public:
   
//============================== For reading =============================//
   const double operator() (double a_r) const {
      return rho(a_r);
   }
//======================== Constructor & Destructor ======================//
   DENSITY_TYPE_BASE();
   DENSITY_TYPE_BASE(const DENSITY_TYPE_BASE&);
   DENSITY_TYPE_BASE(const int, const double);
   virtual ~DENSITY_TYPE_BASE() {}
   
//============================= For initialize ===========================//
   void set_base(const int, const double);
   
   virtual void set(const int, const double);
   virtual void set(const int, const double, const double);
//================ definition of the density function type ===============//
   virtual const double rho(double a_r) const { return 0.0; }
//=========================== Several functions ==========================//
   void set_RA();
   
   int    A()    { return m_A;    }
   double r0()   { return m_r0;   }
   double RA()   { return m_RA;   }
   double rho0() { return m_rho0; }
   const int    A()    const { return m_A;    }
   const double r0()   const { return m_r0;   }
   const double RA()   const { return m_RA;   }
   const double rho0() const { return m_rho0; }
   
   virtual       double aA()       { return 0.0; }
   virtual const double aA() const { return 0.0; }
};

#endif
