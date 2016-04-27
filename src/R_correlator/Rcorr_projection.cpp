//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup R-correlator
 * @brief   The function for projection of angular momentum
 * @author  Takaya Miyamoto
 * @since   Wed Feb  3 00:22:57 JST 2016
 */
//--------------------------------------------------------------------------

#include <NBSwave/NBSwave.h>
#include <R_correlator/R_correlator.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for taking spin projection
 */
//--------------------------------------------------------------------------
void Rcorrelator::spin_projection(  const R_CORRELATOR_SRC_PRJ &srcprjRcorr
                                  , R_CORRELATOR &Rcorr, const SPIN_TYPE spin )
{
   Rcorr.mem_alloc();
   
   if (spin.number == SPIN_0_0) {
      for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         Rcorr(n) = ( srcprjRcorr(n,1) - srcprjRcorr(n,2) ) / sqrt(2.0);
   }
   else if (spin.number == SPIN_1_p1) {
      for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         Rcorr(n) = srcprjRcorr(n,0);
   }
   else if (spin.number == SPIN_1_0) {
      for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         Rcorr(n) = ( srcprjRcorr(n,1) + srcprjRcorr(n,2) ) / sqrt(2.0);
   }
   else if (spin.number == SPIN_1_0ud) {
      for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         Rcorr(n) = srcprjRcorr(n,1);
   }
   else if (spin.number == SPIN_1_0du) {
      for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         Rcorr(n) = srcprjRcorr(n,2);
   }
   else if (spin.number == SPIN_1_m1) {
      for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         Rcorr(n) = srcprjRcorr(n,3);
   }
   else analysis::error(3,"Invalid spin number !");
}

//--------------------------------------------------------------------------
/**
 * @brief Function for take angular momentum (S wave) projection
 */
//--------------------------------------------------------------------------
void Rcorrelator::Swave_projection( R_CORRELATOR &Rcorr )
{
   R_CORRELATOR tmp;   tmp.mem_alloc();
   
   int rot_xyz[3];
   for (         int rot_type=0; rot_type < 24;     rot_type++)
      for (      int z=0;        z<analysis::zSIZE; z++)
         for (   int y=0;        y<analysis::ySIZE; y++)
            for (int x=0;        x<analysis::xSIZE; x++)
            {
               // projection to anguler momentum l = 0
               for (int k=0; k<3; k++)
                  rot_xyz[k]
                  = (  NBSwave::rot_matrix[matrix_idx(k,0,rot_type)] * x
                     + NBSwave::rot_matrix[matrix_idx(k,1,rot_type)] * y
                     + NBSwave::rot_matrix[matrix_idx(k,2,rot_type)] * z
                     + NBSwave::rot_matrix[matrix_idx(k,3,rot_type)] * 1
                     ) % analysis::xSIZE;
               
               tmp(rot_xyz[0],rot_xyz[1],rot_xyz[2]) += Rcorr(x,y,z);
            }
   
   for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
      Rcorr(n) = tmp(n) / 24.0;
}

//--------------------------------------------------------------------------
/**
 * @brief Function for take angular momentum (S wave & (1-S) wave) projection
 */
//--------------------------------------------------------------------------
void Rcorrelator::Swave_division( R_CORRELATOR &Rcorr, R_CORRELATOR &Rcorr_bar )
{
   R_CORRELATOR tmp;   tmp.mem_alloc();
   Rcorr_bar.mem_alloc();
   
   int rot_xyz[3];
   for (         int rot_type=0; rot_type < 24;     rot_type++)
      for (      int z=0;        z<analysis::zSIZE; z++)
         for (   int y=0;        y<analysis::ySIZE; y++)
            for (int x=0;        x<analysis::xSIZE; x++)
            {
               // projection to anguler momentum l = 0 (S wave)
               for (int k=0; k<3; k++)
                  rot_xyz[k]
                  = (  NBSwave::rot_matrix[matrix_idx(k,0,rot_type)] * x
                     + NBSwave::rot_matrix[matrix_idx(k,1,rot_type)] * y
                     + NBSwave::rot_matrix[matrix_idx(k,2,rot_type)] * z
                     + NBSwave::rot_matrix[matrix_idx(k,3,rot_type)] * 1
                     ) % analysis::xSIZE;
               
               tmp(rot_xyz[0],rot_xyz[1],rot_xyz[2]) += Rcorr(x,y,z);
            }
   
   for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
   {
      Rcorr_bar(n) = Rcorr(n) - tmp(n)/24.0;
      Rcorr(n)     = tmp(n)/24.0;
   }
}

void Rcorrelator::Swave_division(  R_CORRELATOR_SRC_PRJ &Rcorr
                                 , R_CORRELATOR_SRC_PRJ &Rcorr_bar )
{
   R_CORRELATOR tmp;   tmp.mem_alloc();
   Rcorr_bar.mem_alloc();
   
   int rot_xyz[3];
   for (int ab=0; ab<4; ab++)
   {
      for (      int z=0; z<analysis::zSIZE; z++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int x=0; x<analysis::xSIZE; x++)
            {
               tmp(x,y,z) = COMP_ZERO;
               
               for (int rot_type=0; rot_type < 24; rot_type++)
               {
                  // projection to anguler momentum l = 0 (S wave)
                  for (int k=0; k<3; k++)
                     rot_xyz[k]
                     = (  NBSwave::rot_matrix[matrix_idx(k,0,rot_type)] * x
                        + NBSwave::rot_matrix[matrix_idx(k,1,rot_type)] * y
                        + NBSwave::rot_matrix[matrix_idx(k,2,rot_type)] * z
                        + NBSwave::rot_matrix[matrix_idx(k,3,rot_type)] * 1
                        ) % analysis::xSIZE;
                  
                  tmp(x,y,z) += Rcorr(rot_xyz[0],rot_xyz[1],rot_xyz[2],ab);
               }
            }
      for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
      {
         Rcorr_bar(n,ab) = Rcorr(n,ab) - tmp(n)/24.0;
         Rcorr(n,ab)     = tmp(n)/24.0;
      }
   }
}

//--------------------------------------------------------------------------
/**
 * @brief Function for take parity projection
 */
//--------------------------------------------------------------------------
void Rcorrelator::parity_projection( R_CORRELATOR &Rcorr )
{
   R_CORRELATOR tmp;   tmp.mem_alloc();
   
   for (      int z=0; z<analysis::zSIZE; z++)
      for (   int y=0; y<analysis::ySIZE; y++)
         for (int x=0; x<analysis::xSIZE; x++)
            tmp(x,y,z)
            = (  Rcorr(x,y,z)
               + Rcorr(  (analysis::xSIZE-x)%analysis::xSIZE
                       , (analysis::ySIZE-y)%analysis::ySIZE
                       , (analysis::zSIZE-z)%analysis::zSIZE ) ) * 0.5;
   
   for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
      Rcorr(n) = tmp(n);
}

void Rcorrelator::parity_projection( R_CORRELATOR_SRC_PRJ &Rcorr )
{
   R_CORRELATOR tmp;   tmp.mem_alloc();
   
   for (int ab=0; ab<4; ab++)
   {
      for (      int z=0; z<analysis::zSIZE; z++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int x=0; x<analysis::xSIZE; x++)
               tmp(x,y,z)
               = (  Rcorr(x,y,z,ab)
                  + Rcorr(  (analysis::xSIZE-x)%analysis::xSIZE
                          , (analysis::ySIZE-y)%analysis::ySIZE
                          , (analysis::zSIZE-z)%analysis::zSIZE, ab ) ) * 0.5;
   
      for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         Rcorr(n,ab) = tmp(n);
   }
}

//--------------------------------------------------------------------------
/**
 * @brief Function for divide S-wave and S_ber-wave, and parity projection at once
 */
//--------------------------------------------------------------------------
void Rcorrelator::LP_projection( R_CORRELATOR &Rcorr ) {
   
   Swave_projection (Rcorr);
   parity_projection(Rcorr);
}

//--------------------------------------------------------------------------
/**
 * @brief Function for devide the spherical function from R-correlator
 */
//--------------------------------------------------------------------------
void Rcorrelator::remove_angular( R_CORRELATOR &Rcorr, int m, double factor ) {
   
   int X,Y,Z;
   
   if (m == -2) {
      for (      int x=0; x<analysis::xSIZE; x++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int z=0; z<analysis::zSIZE; z++)
            {
               analysis::convert_origin(x,y,z, &X,&Y,&Z);
               Rcorr(x,y,z) /= (factor * NBSwave::Y_2_m2(X,Y,Z));
            }
   }
   else if (m == -1) {
      for (      int x=0; x<analysis::xSIZE; x++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int z=0; z<analysis::zSIZE; z++)
            {
               analysis::convert_origin(x,y,z, &X,&Y,&Z);
               Rcorr(x,y,z) /= (factor * NBSwave::Y_2_m1(X,Y,Z));
            }
   }
   else if (m == 0) {
      for (      int x=0; x<analysis::xSIZE; x++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int z=0; z<analysis::zSIZE; z++)
            {
               analysis::convert_origin(x,y,z, &X,&Y,&Z);
               Rcorr(x,y,z) /= -(factor * NBSwave::Y_2_0(X,Y,Z));
            }
   }
   else if (m == 1) {
      for (      int x=0; x<analysis::xSIZE; x++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int z=0; z<analysis::zSIZE; z++)
            {
               analysis::convert_origin(x,y,z, &X,&Y,&Z);
               Rcorr(x,y,z) /= (factor * NBSwave::Y_2_p1(X,Y,Z));
            }
   }
   else if (m == 2) {
      for (      int x=0; x<analysis::xSIZE; x++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int z=0; z<analysis::zSIZE; z++)
            {
               analysis::convert_origin(x,y,z, &X,&Y,&Z);
               Rcorr(x,y,z) /= (factor * NBSwave::Y_2_p2(X,Y,Z));
            }
   }
   else analysis::error(3,"Invalid z-component !");
}

//--------------------------------------------------------------------------
/**
 * @brief Function to multiply YD^star * Rcorr (src spin projected)
 */
//--------------------------------------------------------------------------
using namespace NBSwave;
void Rcorrelator::mult_YDstar(  R_CORRELATOR_SRC_PRJ &Rcorr
                              , const SPIN_TYPE src_spin )
{
   R_CORRELATOR_SRC_PRJ tmp;
   tmp.mem_alloc();
   
   int X,Y,Z;
   if (src_spin.number == SPIN_1_m1)
   {
      for (      int x=0; x<analysis::xSIZE; x++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int z=0; z<analysis::zSIZE; z++)
            {
               analysis::convert_origin(x,y,z, &X,&Y,&Z);
               
               tmp(x,y,z,0) = (  sqrt(1.0/10.0) * Y_2_0 (X,Y,Z) * Rcorr(x,y,z,0)
                               + sqrt(3.0/20.0) * Y_2_m1(X,Y,Z) * Rcorr(x,y,z,2));
               tmp(x,y,z,1) = (  sqrt(1.0/10.0) * Y_2_0 (X,Y,Z) * Rcorr(x,y,z,1)
                               + sqrt(3.0/20.0) * Y_2_m1(X,Y,Z) * Rcorr(x,y,z,3));
               tmp(x,y,z,2) = (  sqrt(3.0/20.0) * Y_2_m1(X,Y,Z) * Rcorr(x,y,z,0)
                               + sqrt(3.0/5.0)  * Y_2_m2(X,Y,Z) * Rcorr(x,y,z,2));
               tmp(x,y,z,3) = (  sqrt(3.0/20.0) * Y_2_m1(X,Y,Z) * Rcorr(x,y,z,1)
                               + sqrt(3.0/5.0)  * Y_2_m2(X,Y,Z) * Rcorr(x,y,z,3));
            }
   }
   else if (src_spin.number == SPIN_1_0)
   {
      for (      int x=0; x<analysis::xSIZE; x++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int z=0; z<analysis::zSIZE; z++)
            {
               analysis::convert_origin(x,y,z, &X,&Y,&Z);
               
               tmp(x,y,z,0) = -(  sqrt(3.0/10.0) * Y_2_p1(X,Y,Z) * Rcorr(x,y,z,0)
                                + sqrt(1.0/5.0)  * Y_2_0 (X,Y,Z) * Rcorr(x,y,z,2));
               tmp(x,y,z,1) = -(  sqrt(3.0/10.0) * Y_2_p1(X,Y,Z) * Rcorr(x,y,z,1)
                                + sqrt(1.0/5.0)  * Y_2_0 (X,Y,Z) * Rcorr(x,y,z,3));
               tmp(x,y,z,2) = -(  sqrt(1.0/5.0)  * Y_2_0 (X,Y,Z) * Rcorr(x,y,z,0)
                                + sqrt(3.0/10.0) * Y_2_m1(X,Y,Z) * Rcorr(x,y,z,2));
               tmp(x,y,z,3) = -(  sqrt(1.0/5.0)  * Y_2_0 (X,Y,Z) * Rcorr(x,y,z,1)
                                + sqrt(3.0/10.0) * Y_2_m1(X,Y,Z) * Rcorr(x,y,z,3));
            }
   }
   else if (src_spin.number == SPIN_1_p1)
   {
      for (      int x=0; x<analysis::xSIZE; x++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int z=0; z<analysis::zSIZE; z++)
            {
               analysis::convert_origin(x,y,z, &X,&Y,&Z);
               
               tmp(x,y,z,0) = (  sqrt(3.0/5.0)  * Y_2_p2(X,Y,Z) * Rcorr(x,y,z,0)
                               + sqrt(3.0/20.0) * Y_2_p1(X,Y,Z) * Rcorr(x,y,z,2));
               tmp(x,y,z,1) = (  sqrt(3.0/5.0)  * Y_2_p2(X,Y,Z) * Rcorr(x,y,z,1)
                               + sqrt(3.0/20.0) * Y_2_p1(X,Y,Z) * Rcorr(x,y,z,3));
               tmp(x,y,z,2) = (  sqrt(3.0/20.0) * Y_2_p1(X,Y,Z) * Rcorr(x,y,z,0)
                               + sqrt(1.0/10.0) * Y_2_0 (X,Y,Z) * Rcorr(x,y,z,2));
               tmp(x,y,z,3) = (  sqrt(3.0/20.0) * Y_2_p1(X,Y,Z) * Rcorr(x,y,z,1)
                               + sqrt(1.0/10.0) * Y_2_0 (X,Y,Z) * Rcorr(x,y,z,3));
            }
   }
   else analysis::error(3,"Invalid spin number !");
   
   for (int n=0; n<4*analysis::xSIZE*analysis::ySIZE*analysis::zSIZE; n++)
      Rcorr(n) = tmp(n);
}
