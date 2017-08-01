//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup observable
 * @brief   Header File for definition of density function type for nuclei
 * @author  Takaya Miyamoto
 * @since   Sun Jun 19 03:26:49 JST 2016
 */
//--------------------------------------------------------------------------

#ifndef DENSITY_TYPE_H
#define DENSITY_TYPE_H

#include <observable/density_type_base.h>

//--------------------------------------------------------------------------
/**
 * @brief The class for the density function (Gaussian-type)
 */
//--------------------------------------------------------------------------
class DENSITY_GAUSSIAN : public DENSITY_TYPE_BASE
{
protected:
   
public:
   
//======================== Constructor & Destructor ======================//
   DENSITY_GAUSSIAN();
   DENSITY_GAUSSIAN(const DENSITY_GAUSSIAN&);
   DENSITY_GAUSSIAN(const int, const double);
   ~DENSITY_GAUSSIAN() {}
   
//============================= For initialize ===========================//
   void set(const int, const double);
   void set(const int, const double, const double);
//================ definition of the density function type ===============//
   const double rho(double) const;
//=========================== Several functions ==========================//
   void set_rho0();
};

//--------------------------------------------------------------------------
/**
 * @brief The class for the density function (Woods-Saxon-type)
 */
//--------------------------------------------------------------------------
class DENSITY_WOODS_SAXON : public DENSITY_TYPE_BASE
{
protected:
   double m_aA;
   
public:
   
//======================== Constructor & Destructor ======================//
   DENSITY_WOODS_SAXON();
   DENSITY_WOODS_SAXON(const DENSITY_WOODS_SAXON&);
   DENSITY_WOODS_SAXON(const int, const double, const double);
   ~DENSITY_WOODS_SAXON() {}
   
//============================= For initialize ===========================//
   void set(const int, const double);
   void set(const int, const double, const double);
//================ definition of the density function type ===============//
   const double rho(double) const;
//=========================== Several functions ==========================//
   void set_rho0();
   
   double aA() { return m_aA; }
   const double aA() const { return m_aA; }
};

#endif
