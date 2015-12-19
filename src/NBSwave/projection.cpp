//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBS wave function
 * @brief   Calculate spin, angular momentum and parity projection
 * @brief   for NBS wave function
 * @author  Takaya Miyamoto
 * @since   Mon Aug 31 18:38:21 JST 2015
 */
//--------------------------------------------------------------------------

#include <NBSwave/NBSwave.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for taking spin projection
 * @brief spin=1 & spin_z=999 -> mean of the spin_z=1,0,-1
 */
//--------------------------------------------------------------------------
void NBS_WAVE::spin_projection( NBS_WAVE_ORG &orgNBS, int spin, int spin_z ) {
   
   if (spin == 0) {
      for (      int z=0; z<analysis::zSIZE; z++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int x=0; x<analysis::xSIZE; x++) {
               
               wave[idx(x,y,z)] = (  orgNBS(x,y,z,1,1)
                                   - orgNBS(x,y,z,2,1)
                                   - orgNBS(x,y,z,1,2)
                                   + orgNBS(x,y,z,2,2) ) * 0.5;
            }
   }
   else if (spin == 1 && spin_z == 1) {
      for (      int z=0; z<analysis::zSIZE; z++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int x=0; x<analysis::xSIZE; x++) {
               
               wave[idx(x,y,z)] = orgNBS(x,y,z,0,0);
            }
   }
   else if(spin == 1 && spin_z == 0){
      for (      int z=0; z<analysis::zSIZE; z++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int x=0; x<analysis::xSIZE; x++) {
               
               wave[idx(x,y,z)] = (  orgNBS(x,y,z,1,1)
                                   + orgNBS(x,y,z,2,1)
                                   + orgNBS(x,y,z,1,2)
                                   + orgNBS(x,y,z,2,2) ) * 0.5;
            }
   }
   else if(spin == 1 && spin_z == -1){
      for (      int z=0; z<analysis::zSIZE; z++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int x=0; x<analysis::xSIZE; x++) {
               
               wave[idx(x,y,z)] = orgNBS(x,y,z,3,3);
            }
   }
   else if(spin == 1 && spin_z == 999){
      for (      int z=0; z<analysis::zSIZE; z++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int x=0; x<analysis::xSIZE; x++) {
               
               wave[idx(x,y,z)] = ( orgNBS(x,y,z,0,0)
                                   + (  orgNBS(x,y,z,1,1)
                                      + orgNBS(x,y,z,2,1)
                                      + orgNBS(x,y,z,1,2)
                                      + orgNBS(x,y,z,2,2) ) * 0.5
                                   + orgNBS(x,y,z,3,3) ) / 3.0;
            }
   }
   else analysis::error(3,"Invalid spin number !");
}

//--------------------------------------------------------------------------
/**
 * @brief Function for take angular momentum projection
 */
//--------------------------------------------------------------------------
void NBS_WAVE::ang_mom_projection( int ang_mom ) {
    
   func_name = "ang_mom_projection____";
   analysis::route(class_name, func_name, 1);
   
   if (wave == NULL) {
      analysis::error(1,"NBS wave function has not been set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   
   size_t xyzSIZE = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   cdouble *tmp;   tmp = new cdouble[xyzSIZE];
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
               
               tmp[idx(rot_xyz[0],rot_xyz[1],rot_xyz[2])] += wave[idx(x,y,z)];
            }
   
   for (size_t n=0; n<xyzSIZE; n++) wave[n] = tmp[n] / 24.0;
   delete [] tmp;

   analysis::route( class_name, func_name, 0 );
}

//--------------------------------------------------------------------------
/**
 * @brief Function for take parity projection
 */
//--------------------------------------------------------------------------
void NBS_WAVE::parity_projection() {
    
   func_name = "parity_projection_____";
   analysis::route(class_name, func_name, 1);

   if (wave == NULL) {
      analysis::error(1,"NBS wave function has not been set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   size_t xyzSIZE = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   cdouble *tmp;   tmp = new cdouble[xyzSIZE];
      
   for (      int z=0;    z<analysis::zSIZE;     z++)
      for (   int y=0;    y<analysis::ySIZE;     y++)
         for (int x=0;    x<analysis::xSIZE;     x++)
            tmp[idx(x,y,z)]
            = (  wave[idx(x,y,z)]
               + wave[idx(  (analysis::xSIZE-x)%analysis::xSIZE
                          , (analysis::ySIZE-y)%analysis::ySIZE
                          , (analysis::zSIZE-z)%analysis::zSIZE )] ) * 0.5;
   
   for (size_t n=0; n<xyzSIZE; n++) wave[n] = tmp[n];
   
   delete [] tmp;
   
   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Function for angular momentum and parity projection at once
 */
//--------------------------------------------------------------------------
void NBS_WAVE::projection( int ANG_MOM ) {
   
   ang_mom_projection(ANG_MOM);
   parity_projection();
}
