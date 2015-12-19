//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup R-correlator
 * @brief   The function for projection of angular momentum
 * @author  Takaya Miyamoto
 * @since   Wed Dec 16 06:17:08 JST 2015
 */
//--------------------------------------------------------------------------

#include <R_correlator/R_correlator.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for take angular momentum (S wave) projection
 */
//--------------------------------------------------------------------------
void Rcorrelator::Swave_projection( R_CORRELATOR &Rcorr ) {
   
   R_CORRELATOR tmp;   tmp.mem_alloc();
   
   for (         int rot_type=0; rot_type < 24;     rot_type++)
      for (      int z=0;        z<analysis::zSIZE; z++)
         for (   int y=0;        y<analysis::ySIZE; y++)
            for (int x=0;        x<analysis::xSIZE; x++) {
               int rot_xyz[3];
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
void Rcorrelator::Swave_division( R_CORRELATOR &Rcorr, R_CORRELATOR &Rcorr_bar ) {
   
   R_CORRELATOR tmp;   tmp.mem_alloc();
   Rcorr_bar.mem_alloc();
   
   for (         int rot_type=0; rot_type < 24;     rot_type++)
      for (      int z=0;        z<analysis::zSIZE; z++)
         for (   int y=0;        y<analysis::ySIZE; y++)
            for (int x=0;        x<analysis::xSIZE; x++) {
               int rot_xyz[3];
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

//--------------------------------------------------------------------------
/**
 * @brief Function for take parity projection
 */
//--------------------------------------------------------------------------
void Rcorrelator::parity_projection( R_CORRELATOR &Rcorr ) {
   
   R_CORRELATOR tmp;   tmp.mem_alloc();
   
   for (      int z=0;    z<analysis::zSIZE;     z++)
      for (   int y=0;    y<analysis::ySIZE;     y++)
         for (int x=0;    x<analysis::xSIZE;     x++)
            tmp(x,y,z)
            = (  Rcorr(x,y,z)
               + Rcorr(  (analysis::xSIZE-x)%analysis::xSIZE
                       , (analysis::ySIZE-y)%analysis::ySIZE
                       , (analysis::zSIZE-z)%analysis::zSIZE ) ) * 0.5;
   
   for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
      Rcorr(n) = tmp(n);
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
   
   if (m == -2) {
      for (      int x=0; x<analysis::xSIZE; x++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int z=0; z<analysis::zSIZE; z++)
               Rcorr(x,y,z) /= (factor * NBSwave::Y_2_m2(x,y,z));
   }
   else if (m == -1) {
      for (      int x=0; x<analysis::xSIZE; x++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int z=0; z<analysis::zSIZE; z++)
               Rcorr(x,y,z) /= (factor * NBSwave::Y_2_m1(x,y,z));
   }
   else if (m == 0) {
      for (      int x=0; x<analysis::xSIZE; x++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int z=0; z<analysis::zSIZE; z++)
               Rcorr(x,y,z) /= -(factor * NBSwave::Y_2_0(x,y,z));
   }
   else if (m == 1) {
      for (      int x=0; x<analysis::xSIZE; x++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int z=0; z<analysis::zSIZE; z++)
               Rcorr(x,y,z) /= (factor * NBSwave::Y_2_p1(x,y,z));
   }
   else if (m == 2) {
      for (      int x=0; x<analysis::xSIZE; x++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int z=0; z<analysis::zSIZE; z++)
               Rcorr(x,y,z) /= (factor * NBSwave::Y_2_p2(x,y,z));
   }
   else analysis::error(3,"Invalid z-component !");
}
