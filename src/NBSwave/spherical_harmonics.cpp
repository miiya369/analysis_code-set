//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBS wave function
 * @brief   The function for spherical harmonics
 * @author  Takaya Miyamoto
 * @since   Wed Dec 16 06:20:44 JST 2015
 */
//--------------------------------------------------------------------------

#include <NBSwave/NBSwave.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for spherical harmonics Y(0,0)
 */
//--------------------------------------------------------------------------
cdouble NBSwave::Y_0_0( double x, double y, double z ) {
   
   return 1.0/sqrt(4.0*PI);
}
//--------------------------------------------------------------------------
/**
 * @brief Function for spherical harmonics Y(1,+1)
 */
//--------------------------------------------------------------------------
cdouble NBSwave::Y_1_p1( double x, double y, double z ) {
   
   return -sqrt(3.0/(8.0*PI)) * cdouble(x, y)/sqrt(x*x + y*y + z*z);
}
//--------------------------------------------------------------------------
/**
 * @brief Function for spherical harmonics Y(1,0)
 */
//--------------------------------------------------------------------------
cdouble NBSwave::Y_1_0( double x, double y, double z ) {
   
   return sqrt(3.0/(4.0*PI)) * z/sqrt(x*x + y*y + z*z);
}
//--------------------------------------------------------------------------
/**
 * @brief Function for spherical harmonics Y(1,-1)
 */
//--------------------------------------------------------------------------
cdouble NBSwave::Y_1_m1( double x, double y, double z ) {
   
   return sqrt(3.0/(8.0*PI)) * cdouble(x,-y)/sqrt(x*x + y*y + z*z);
}
//--------------------------------------------------------------------------
/**
 * @brief Function for spherical harmonics Y(2,+2)
 */
//--------------------------------------------------------------------------
cdouble NBSwave::Y_2_p2( double x, double y, double z ) {
   
   return sqrt(15.0/(32.0*PI)) * cdouble(x, y)*cdouble(x, y)/(x*x + y*y + z*z);
}
//--------------------------------------------------------------------------
/**
 * @brief Function for spherical harmonics Y(2,+1)
 */
//--------------------------------------------------------------------------
cdouble NBSwave::Y_2_p1( double x, double y, double z ) {
   
   return -sqrt(15.0/(8.0*PI)) * z * cdouble(x, y)/(x*x + y*y + z*z);
}
//--------------------------------------------------------------------------
/**
 * @brief Function for spherical harmonics Y(2,0)
 */
//--------------------------------------------------------------------------
cdouble NBSwave::Y_2_0( double x, double y, double z ) {
   
   return sqrt(5.0/(16.0*PI)) * (2.0*z*z - x*x - y*y)/(x*x + y*y + z*z);
}
//--------------------------------------------------------------------------
/**
 * @brief Function for spherical harmonics Y(2,-1)
 */
//--------------------------------------------------------------------------
cdouble NBSwave::Y_2_m1( double x, double y, double z ) {
   
   return sqrt(15.0/(8.0*PI)) * z * cdouble(x,-y)/(x*x + y*y + z*z);
}
//--------------------------------------------------------------------------
/**
 * @brief Function for spherical harmonics Y(2,-2)
 */
//--------------------------------------------------------------------------
cdouble NBSwave::Y_2_m2( double x, double y, double z ) {
   
   return sqrt(15.0/(32.0*PI)) * cdouble(x,-y)*cdouble(x,-y)/(x*x + y*y + z*z);
}
