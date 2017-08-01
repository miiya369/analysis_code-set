//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBS wave function
 * @brief   The function for Calculate spin, angular momentum
 * @brief   and parity projection for NBS wave function
 * @author  Takaya Miyamoto
 * @since   Tue Feb  2 18:05:03 JST 2016
 */
//--------------------------------------------------------------------------

#include <NBSwave/NBSwave.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for taking spin projection
 */
//--------------------------------------------------------------------------
void NBSwave::spin_projection(  const NBS_WAVE_ORG &orgNBS, NBS_WAVE_SRC_PRJ &NBS
                              , const SPIN_TYPE spin ) {
   NBS.mem_alloc();
   
   if (spin.number == SPIN_0_0) {
      for (   int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         for (int a=0; a<4; a++)
            NBS(n,a) = (orgNBS(n,a,1) - orgNBS(n,a,2)) / sqrt(2.0);
   }
   else if (spin.number == SPIN_1_p1) {
      for (   int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         for (int a=0; a<4; a++)
            NBS(n,a) = orgNBS(n,a,0);
   }
   else if (spin.number == SPIN_1_0) {
      for (   int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         for (int a=0; a<4; a++)
            NBS(n,a) = (orgNBS(n,a,1) + orgNBS(n,a,2)) / sqrt(2.0);
   }
   else if (spin.number == SPIN_1_0ud) {
      for (   int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         for (int a=0; a<4; a++)
            NBS(n,a) = orgNBS(n,a,1);
   }
   else if (spin.number == SPIN_1_0du) {
      for (   int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         for (int a=0; a<4; a++)
            NBS(n,a) = orgNBS(n,a,2);
   }
   else if (spin.number == SPIN_1_m1) {
      for (   int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         for (int a=0; a<4; a++)
            NBS(n,a) = orgNBS(n,a,3);
   }
   else if (spin.number == SPIN_1_ave) {
      for (   int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         for (int a=0; a<4; a++)
            NBS(n,a) = (   orgNBS(n,a,0) + orgNBS(n,a,3)
                        + (orgNBS(n,a,1) + orgNBS(n,a,2)) / sqrt(2.0) ) / 3.0;
   }
   else ERROR_COMMENTS("Invalid spin number !");
}
//--------------------------------------------------------------------------

void NBSwave::spin_projection(  const NBS_WAVE_SRC_PRJ &srcprjNBS, NBS_WAVE &NBS
                              , const SPIN_TYPE spin ) {
   NBS.mem_alloc();
   
   if (spin.number == SPIN_0_0) {
      for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         NBS(n) = (srcprjNBS(n,1) - srcprjNBS(n,2)) / sqrt(2.0);
   }
   else if (spin.number == SPIN_1_p1) {
      for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         NBS(n) = srcprjNBS(n,0);
   }
   else if (spin.number == SPIN_1_0) {
      for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         NBS(n) = (srcprjNBS(n,1) + srcprjNBS(n,2)) / sqrt(2.0);
   }
   else if (spin.number == SPIN_1_0ud) {
      for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         NBS(n) = srcprjNBS(n,1);
   }
   else if (spin.number == SPIN_1_0du) {
      for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         NBS(n) = srcprjNBS(n,2);
   }
   else if (spin.number == SPIN_1_m1) {
      for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         NBS(n) = srcprjNBS(n,3);
   }
   else if (spin.number == SPIN_1_ave) {
      for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         NBS(n) = (   srcprjNBS(n,0) + srcprjNBS(n,3)
                   + (srcprjNBS(n,1) + srcprjNBS(n,2)) / sqrt(2.0) ) / 3.0;
   }
   else ERROR_COMMENTS("Invalid spin number !");
}
//--------------------------------------------------------------------------

void NBSwave::spin_projection(  const NBS_WAVE_ORG &orgNBS, NBS_WAVE &NBS
                              , const SPIN_TYPE spin ) {
   NBS.mem_alloc();
   
   if (spin.number == SPIN_0_0) {
      for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         NBS(n) = (  orgNBS(n,1,1) - orgNBS(n,1,2)
                   - orgNBS(n,2,1) + orgNBS(n,2,2) ) / 2.0;
   }
   else if (spin.number == SPIN_1_p1) {
      for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         NBS(n) = orgNBS(n,0,0);
   }
   else if (spin.number == SPIN_1_0) {
      for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         NBS(n) = (  orgNBS(n,1,1) + orgNBS(n,1,2)
                   + orgNBS(n,2,1) + orgNBS(n,2,2) ) / 2.0;
   }
   else if (spin.number == SPIN_1_0ud) {
      for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         NBS(n) = orgNBS(n,1,1);
   }
   else if (spin.number == SPIN_1_0du) {
      for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         NBS(n) = orgNBS(n,2,2);
   }
   else if (spin.number == SPIN_1_m1) {
      for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         NBS(n) = orgNBS(n,3,3);
   }
   else if (spin.number == SPIN_1_ave) {
      for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         NBS(n) = (     orgNBS(n,0,0) + orgNBS(n,3,3)
                   + (  orgNBS(n,1,1) + orgNBS(n,1,2)
                      + orgNBS(n,2,1) + orgNBS(n,2,2) ) / 6.0 );
   }
   else ERROR_COMMENTS("Invalid spin number !");
}

//--------------------------------------------------------------------------
/**
 * @brief Function for take angular momentum (S wave) projection
 */
//--------------------------------------------------------------------------
void NBSwave::Swave_projection(NBS_WAVE &NBS) {
   NBS_WAVE tmp;
   tmp.mem_alloc();
   
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
               
               tmp(rot_xyz[0],rot_xyz[1],rot_xyz[2]) += NBS(x,y,z);
            }
   
   for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
      NBS(n) = tmp(n) / 24.0;
}

//--------------------------------------------------------------------------
/**
 * @brief Function for take angular momentum (S wave & (1-S) wave) projection
 */
//--------------------------------------------------------------------------
void NBSwave::Swave_division(NBS_WAVE &NBS, NBS_WAVE &NBS_ber) {
   NBS_WAVE tmp;
   tmp.mem_alloc();
   NBS_ber.mem_alloc();
   
   int rot_xyz[3];
   for (         int rot_type=0; rot_type < 24;     rot_type++)
      for (      int z=0;        z<analysis::zSIZE; z++)
         for (   int y=0;        y<analysis::ySIZE; y++)
            for (int x=0;        x<analysis::xSIZE; x++) {
               // projection to anguler momentum l = 0 (S wave)
               for (int k=0; k<3; k++)
                  rot_xyz[k]
                  = (  NBSwave::rot_matrix[matrix_idx(k,0,rot_type)] * x
                     + NBSwave::rot_matrix[matrix_idx(k,1,rot_type)] * y
                     + NBSwave::rot_matrix[matrix_idx(k,2,rot_type)] * z
                     + NBSwave::rot_matrix[matrix_idx(k,3,rot_type)] * 1
                     ) % analysis::xSIZE;
               
               tmp(rot_xyz[0],rot_xyz[1],rot_xyz[2]) += NBS(x,y,z);
            }
   
   for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++) {
      NBS_ber(n) = NBS(n) - tmp(n)/24.0;
      NBS(n)     = tmp(n)/24.0;
   }
}

void NBSwave::Swave_division(NBS_WAVE_SRC_PRJ &NBS, NBS_WAVE_SRC_PRJ &NBS_ber) {
   NBS_WAVE tmp;
   tmp.mem_alloc();
   NBS_ber.mem_alloc();
   
   int rot_xyz[3];
   for (int ab=0; ab<4; ab++)
   {
      for (      int z=0; z<analysis::zSIZE; z++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int x=0; x<analysis::xSIZE; x++) {
               tmp(x,y,z) = COMP_ZERO;
               
               for (int rot_type=0; rot_type < 24; rot_type++) {
                  // projection to anguler momentum l = 0 (S wave)
                  for (int k=0; k<3; k++)
                     rot_xyz[k]
                     = (  NBSwave::rot_matrix[matrix_idx(k,0,rot_type)] * x
                        + NBSwave::rot_matrix[matrix_idx(k,1,rot_type)] * y
                        + NBSwave::rot_matrix[matrix_idx(k,2,rot_type)] * z
                        + NBSwave::rot_matrix[matrix_idx(k,3,rot_type)] * 1
                        ) % analysis::xSIZE;
                  
                  tmp(x,y,z) += NBS(rot_xyz[0],rot_xyz[1],rot_xyz[2],ab);
               }
            }
      for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++) {
         NBS_ber(n,ab) = NBS(n,ab) - tmp(n)/24.0;
         NBS(n,ab)     = tmp(n)/24.0;
      }
   }
}

//--------------------------------------------------------------------------
/**
 * @brief Function for take parity projection
 */
//--------------------------------------------------------------------------
void NBSwave::parity_projection(NBS_WAVE &NBS) {
   NBS_WAVE tmp;
   tmp.mem_alloc();
      
   for (      int z=0; z<analysis::zSIZE; z++)
      for (   int y=0; y<analysis::ySIZE; y++)
         for (int x=0; x<analysis::xSIZE; x++)
            tmp(x,y,z)
            = (  NBS(x,y,z)
               + NBS(  (analysis::xSIZE-x)%analysis::xSIZE
                     , (analysis::ySIZE-y)%analysis::ySIZE
                     , (analysis::zSIZE-z)%analysis::zSIZE ) ) * 0.5;
   
   for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
      NBS(n) = tmp(n);
}

void NBSwave::parity_projection(NBS_WAVE_SRC_PRJ &NBS) {
   NBS_WAVE tmp;   tmp.mem_alloc();
   
   for (int ab=0; ab<4; ab++)
   {
      for (      int z=0; z<analysis::zSIZE; z++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int x=0; x<analysis::xSIZE; x++)
               tmp(x,y,z)
               = (  NBS(x,y,z,ab)
                  + NBS(  (analysis::xSIZE-x)%analysis::xSIZE
                        , (analysis::ySIZE-y)%analysis::ySIZE
                        , (analysis::zSIZE-z)%analysis::zSIZE, ab ) ) * 0.5;
      
      for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         NBS(n,ab) = tmp(n);
   }
}

//--------------------------------------------------------------------------
/**
 * @brief Function for zero-angular momentum and parity projection at once
 */
//--------------------------------------------------------------------------
void NBSwave::LP_projection(NBS_WAVE &NBS) {
   Swave_projection (NBS);
   parity_projection(NBS);
}

//--------------------------------------------------------------------------
/**
 * @brief Function for devide the spherical function from NBS wave function
 */
//--------------------------------------------------------------------------
void NBSwave::remove_angular(NBS_WAVE &NBS, const int m, const double factor) {
   int X,Y,Z;
   
   if (m == -2) {
      for (      int x=0; x<analysis::xSIZE; x++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int z=0; z<analysis::zSIZE; z++)
            {
               analysis::convert_origin(x,y,z, &X,&Y,&Z);
               NBS(x,y,z) /= (factor * Y_2_m2(X,Y,Z));
            }
   }
   else if (m == -1) {
      for (      int x=0; x<analysis::xSIZE; x++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int z=0; z<analysis::zSIZE; z++)
            {
               analysis::convert_origin(x,y,z, &X,&Y,&Z);
               NBS(x,y,z) /= (factor * Y_2_m1(X,Y,Z));
            }
   }
   else if (m == 0) {
      for (      int x=0; x<analysis::xSIZE; x++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int z=0; z<analysis::zSIZE; z++)
            {
               analysis::convert_origin(x,y,z, &X,&Y,&Z);
               NBS(x,y,z) /= -(factor * Y_2_0(X,Y,Z));
            }
   }
   else if (m == 1) {
      for (      int x=0; x<analysis::xSIZE; x++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int z=0; z<analysis::zSIZE; z++)
            {
               analysis::convert_origin(x,y,z, &X,&Y,&Z);
               NBS(x,y,z) /= (factor * Y_2_p1(X,Y,Z));
            }
   }
   else if (m == 2) {
      for (      int x=0; x<analysis::xSIZE; x++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int z=0; z<analysis::zSIZE; z++)
            {
               analysis::convert_origin(x,y,z, &X,&Y,&Z);
               NBS(x,y,z) /= (factor * Y_2_p2(X,Y,Z));
            }
   }
   else ERROR_COMMENTS("Invalid z-component !");
}

//--------------------------------------------------------------------------
/**
 * @brief Function for calculate tensor operator S12 * psi (src spin projected)
 */
//--------------------------------------------------------------------------
void NBSwave::S12_psi(  const NBS_WAVE_SRC_PRJ &srcprjNBS, NBS_WAVE &NBS
                      , const SPIN_TYPE spin ) {
   NBS.mem_alloc();
   int X,Y,Z;
   
   if (spin.number == SPIN_0_0) {
      for (int n=0; n<analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; n++)
         NBS(n) = COMP_ZERO;
   }
   else if (spin.number == SPIN_1_p1) {
      for (      int x=0; x<analysis::xSIZE; x++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int z=0; z<analysis::zSIZE; z++)
            {
               analysis::convert_origin(x,y,z, &X,&Y,&Z);
               NBS(x,y,z) = (  sqrt(2.0)* Y_2_0(X,Y,Z)*    srcprjNBS(x,y,z,0)
                             + sqrt(3.0)* Y_2_m1(X,Y,Z)*(  srcprjNBS(x,y,z,1)
                                                         + srcprjNBS(x,y,z,2) )
                             + sqrt(12.0)*Y_2_m2(X,Y,Z)*   srcprjNBS(x,y,z,3)
                             ) * sqrt(8.0*PI/5.0);
            }
   }
   else if (spin.number == SPIN_1_0) {
      for (      int x=0; x<analysis::xSIZE; x++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int z=0; z<analysis::zSIZE; z++)
            {
               analysis::convert_origin(x,y,z, &X,&Y,&Z);
               NBS(x,y,z) = -(  sqrt(3.0)*Y_2_p1(X,Y,Z)*  srcprjNBS(x,y,z,0)
                              + sqrt(2.0)*Y_2_0(X,Y,Z)*(  srcprjNBS(x,y,z,1)
                                                        + srcprjNBS(x,y,z,2) )
                              + sqrt(3.0)*Y_2_m1(X,Y,Z)*  srcprjNBS(x,y,z,3)
                              ) * sqrt(16.0*PI/5.0);
            }
   }
   else if (spin.number == SPIN_1_0ud || spin.number == SPIN_1_0du) {
      for (      int x=0; x<analysis::xSIZE; x++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int z=0; z<analysis::zSIZE; z++)
            {
               analysis::convert_origin(x,y,z, &X,&Y,&Z);
               NBS(x,y,z) = -(  sqrt(3.0)*Y_2_p1(X,Y,Z)*  srcprjNBS(x,y,z,0)
                              + sqrt(2.0)*Y_2_0(X,Y,Z)*(  srcprjNBS(x,y,z,1)
                                                        + srcprjNBS(x,y,z,2) )
                              + sqrt(3.0)*Y_2_m1(X,Y,Z)*  srcprjNBS(x,y,z,3)
                              ) * sqrt(8.0*PI/5.0);
            }
   }
   else if (spin.number == SPIN_1_m1) {
      for (      int x=0; x<analysis::xSIZE; x++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int z=0; z<analysis::zSIZE; z++)
            {
               analysis::convert_origin(x,y,z, &X,&Y,&Z);
               NBS(x,y,z) = (  sqrt(12.0)*Y_2_p2(X,Y,Z)*   srcprjNBS(x,y,z,0)
                             + sqrt(3.0)* Y_2_p1(X,Y,Z)*(  srcprjNBS(x,y,z,1)
                                                         + srcprjNBS(x,y,z,2) )
                             + sqrt(2.0)* Y_2_0(X,Y,Z)*    srcprjNBS(x,y,z,3)
                             ) * sqrt(8.0*PI/5.0);
            }
   }
   else if (spin.number == SPIN_1_ave) {
      for (      int x=0; x<analysis::xSIZE; x++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int z=0; z<analysis::zSIZE; z++)
            {
               analysis::convert_origin(x,y,z, &X,&Y,&Z);
               NBS(x,y,z) = (     srcprjNBS(x,y,z,0) *(  sqrt(2.0) * Y_2_0 (X,Y,Z)
                                                       - sqrt(6.0) * Y_2_p1(X,Y,Z)
                                                       + sqrt(12.0)* Y_2_p2(X,Y,Z))
                             + (  srcprjNBS(x,y,z,1)
                                + srcprjNBS(x,y,z,2))*(  sqrt(3.0) * Y_2_m1(X,Y,Z)
                                                       -      2.0  * Y_2_0 (X,Y,Z)
                                                       + sqrt(3.0) * Y_2_p1(X,Y,Z))
                             +    srcprjNBS(x,y,z,3) *(  sqrt(2.0) * Y_2_0 (X,Y,Z)
                                                       - sqrt(6.0) * Y_2_m1(X,Y,Z)
                                                       + sqrt(12.0)* Y_2_m2(X,Y,Z))
                             ) * sqrt(8.0*PI/45.0);
            }
   }
   else ERROR_COMMENTS("Invalid spin number !");
}

void NBSwave::S12_psi(const NBS_WAVE_SRC_PRJ &srcprjNBS, NBS_WAVE_SRC_PRJ &NBS) {
   NBS.mem_alloc();
   int X,Y,Z;
   
   for (      int x=0; x<analysis::xSIZE; x++)
      for (   int y=0; y<analysis::ySIZE; y++)
         for (int z=0; z<analysis::zSIZE; z++)
         {
            analysis::convert_origin(x,y,z, &X,&Y,&Z);
            
            NBS(x,y,z,0) = (  sqrt(2.0)* Y_2_0 (X,Y,Z)*   srcprjNBS(x,y,z,0)
                            + sqrt(3.0)* Y_2_m1(X,Y,Z)*(  srcprjNBS(x,y,z,1)
                                                        + srcprjNBS(x,y,z,2) )
                            + sqrt(12.0)*Y_2_m2(X,Y,Z)*   srcprjNBS(x,y,z,3)
                            ) * sqrt(8.0*PI/5.0);
            
            NBS(x,y,z,1) = -(  sqrt(3.0)*Y_2_p1(X,Y,Z)*   srcprjNBS(x,y,z,0)
                             + sqrt(2.0)*Y_2_0 (X,Y,Z)*(  srcprjNBS(x,y,z,1)
                                                        + srcprjNBS(x,y,z,2) )
                             + sqrt(3.0)*Y_2_m1(X,Y,Z)*   srcprjNBS(x,y,z,3)
                             ) * sqrt(8.0*PI/5.0);
            
            NBS(x,y,z,2) = NBS(x,y,z,1);
            
            NBS(x,y,z,3) = (  sqrt(12.0)*Y_2_p2(X,Y,Z)*   srcprjNBS(x,y,z,0)
                            + sqrt(3.0)* Y_2_p1(X,Y,Z)*(  srcprjNBS(x,y,z,1)
                                                        + srcprjNBS(x,y,z,2) )
                            + sqrt(2.0)* Y_2_0 (X,Y,Z)*   srcprjNBS(x,y,z,3)
                            ) * sqrt(8.0*PI/5.0);
         }
}

//--------------------------------------------------------------------------
/**
 * @brief Function to multiply YD^star * psi (src spin projected)
 */
//--------------------------------------------------------------------------
void NBSwave::mult_YDstar(NBS_WAVE_SRC_PRJ &NBS, const SPIN_TYPE src_spin) {
   NBS_WAVE_SRC_PRJ tmp;
   tmp.mem_alloc();
   
   int X,Y,Z;
   if (src_spin.number == SPIN_1_m1)
   {
      for (      int x=0; x<analysis::xSIZE; x++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int z=0; z<analysis::zSIZE; z++)
            {
               analysis::convert_origin(x,y,z, &X,&Y,&Z);
               
               tmp(x,y,z,0) = (  sqrt(1.0/10.0) * Y_2_0 (X,Y,Z) * NBS(x,y,z,0)
                               + sqrt(3.0/20.0) * Y_2_m1(X,Y,Z) * NBS(x,y,z,2));
               tmp(x,y,z,1) = (  sqrt(1.0/10.0) * Y_2_0 (X,Y,Z) * NBS(x,y,z,1)
                               + sqrt(3.0/20.0) * Y_2_m1(X,Y,Z) * NBS(x,y,z,3));
               tmp(x,y,z,2) = (  sqrt(3.0/20.0) * Y_2_m1(X,Y,Z) * NBS(x,y,z,0)
                               + sqrt(3.0/5.0)  * Y_2_m2(X,Y,Z) * NBS(x,y,z,2));
               tmp(x,y,z,3) = (  sqrt(3.0/20.0) * Y_2_m1(X,Y,Z) * NBS(x,y,z,1)
                               + sqrt(3.0/5.0)  * Y_2_m2(X,Y,Z) * NBS(x,y,z,3));
            }
   }
   else if (src_spin.number == SPIN_1_0)
   {
      for (      int x=0; x<analysis::xSIZE; x++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int z=0; z<analysis::zSIZE; z++)
            {
               analysis::convert_origin(x,y,z, &X,&Y,&Z);
               
               tmp(x,y,z,0) = -(  sqrt(3.0/10.0) * Y_2_p1(X,Y,Z) * NBS(x,y,z,0)
                                + sqrt(1.0/5.0)  * Y_2_0 (X,Y,Z) * NBS(x,y,z,2));
               tmp(x,y,z,1) = -(  sqrt(3.0/10.0) * Y_2_p1(X,Y,Z) * NBS(x,y,z,1)
                                + sqrt(1.0/5.0)  * Y_2_0 (X,Y,Z) * NBS(x,y,z,3));
               tmp(x,y,z,2) = -(  sqrt(1.0/5.0)  * Y_2_0 (X,Y,Z) * NBS(x,y,z,0)
                                + sqrt(3.0/10.0) * Y_2_m1(X,Y,Z) * NBS(x,y,z,2));
               tmp(x,y,z,3) = -(  sqrt(1.0/5.0)  * Y_2_0 (X,Y,Z) * NBS(x,y,z,1)
                                + sqrt(3.0/10.0) * Y_2_m1(X,Y,Z) * NBS(x,y,z,3));
            }
   }
   else if (src_spin.number == SPIN_1_p1)
   {
      for (      int x=0; x<analysis::xSIZE; x++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int z=0; z<analysis::zSIZE; z++)
            {
               analysis::convert_origin(x,y,z, &X,&Y,&Z);
               
               tmp(x,y,z,0) = (  sqrt(3.0/5.0)  * Y_2_p2(X,Y,Z) * NBS(x,y,z,0)
                               + sqrt(3.0/20.0) * Y_2_p1(X,Y,Z) * NBS(x,y,z,2));
               tmp(x,y,z,1) = (  sqrt(3.0/5.0)  * Y_2_p2(X,Y,Z) * NBS(x,y,z,1)
                               + sqrt(3.0/20.0) * Y_2_p1(X,Y,Z) * NBS(x,y,z,3));
               tmp(x,y,z,2) = (  sqrt(3.0/20.0) * Y_2_p1(X,Y,Z) * NBS(x,y,z,0)
                               + sqrt(1.0/10.0) * Y_2_0 (X,Y,Z) * NBS(x,y,z,2));
               tmp(x,y,z,3) = (  sqrt(3.0/20.0) * Y_2_p1(X,Y,Z) * NBS(x,y,z,1)
                               + sqrt(1.0/10.0) * Y_2_0 (X,Y,Z) * NBS(x,y,z,3));
            }
   }
   else ERROR_COMMENTS("Invalid spin number !");
   
   for (int n=0; n<4*analysis::xSIZE*analysis::ySIZE*analysis::zSIZE; n++)
      NBS(n) = tmp(n);
}
