//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBS wave function
 * @brief   Calculate spin, angular momentum and parity projection
 * @brief   for NBS wave function
 * @author  Takaya Miyamoto
 * @since   Tue Jul 21 01:11:33 JST 2015
 */
//--------------------------------------------------------------------------

#include <NBSwave/NBSwave.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for taking spin projection
 * @brief spin_flg = 0 -> spin0 , = 1~3 -> spin1, = 4 -> mean of spin1
 */
//--------------------------------------------------------------------------
void NBS_WAVE::spin_projection( int spin_flg ) {
   
   func_name = "spin_projection_______";
   analysis::route(class_name, func_name, 1);
   
   if (orgNBS == NULL) {
      analysis::error(1,"NBS wave function has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   if (projNBS == NULL) projNBS = new cdouble[NBSwave::xyznSIZE];
   
   if ( spin_flg == 0 ) {
      for (         int conf=0; conf<analysis::Nconf; conf++ )
         for (      int z=0; z<analysis::zSIZE; z++)
            for (   int y=0; y<analysis::ySIZE; y++)
               for (int x=0; x<analysis::xSIZE; x++) {
                  
                  projNBS[idx(x,y,z,conf)]
                     = (  orgNBS[NBSidx(x,y,z,1,1,conf)]
                        - orgNBS[NBSidx(x,y,z,2,1,conf)]
                        - orgNBS[NBSidx(x,y,z,1,2,conf)]
                        + orgNBS[NBSidx(x,y,z,2,2,conf)] ) * 0.5;
               }
   }
   else if ( spin_flg == 1 ) {
      for (         int conf=0; conf<analysis::Nconf; conf++ )
         for (      int z=0; z<analysis::zSIZE; z++)
            for (   int y=0; y<analysis::ySIZE; y++)
               for (int x=0; x<analysis::xSIZE; x++) {
                  
                  projNBS[idx(x,y,z,conf)]
                     = orgNBS[NBSidx(x,y,z,0,0,conf)];
               }
   }
   else if( spin_flg == 2 ){
      for (         int conf=0; conf<analysis::Nconf; conf++ )
         for (      int z=0; z<analysis::zSIZE; z++)
            for (   int y=0; y<analysis::ySIZE; y++)
               for (int x=0; x<analysis::xSIZE; x++) {
                  
                  projNBS[idx(x,y,z,conf)]
                     = (  orgNBS[NBSidx(x,y,z,1,1,conf)]
                        + orgNBS[NBSidx(x,y,z,2,1,conf)]
                        + orgNBS[NBSidx(x,y,z,1,2,conf)]
                        + orgNBS[NBSidx(x,y,z,2,2,conf)] ) * 0.5;
               }
   }
   else if( spin_flg == 3 ){
      for (         int conf=0; conf<analysis::Nconf; conf++ )
         for (      int z=0; z<analysis::zSIZE; z++)
            for (   int y=0; y<analysis::ySIZE; y++)
               for (int x=0; x<analysis::xSIZE; x++) {
                  
                  projNBS[idx(x,y,z,conf)]
                     = orgNBS[NBSidx(x,y,z,3,3,conf)];
               }
   }
   else if( spin_flg == 4 ){
      for (         int conf=0; conf<analysis::Nconf; conf++ )
         for (      int z=0; z<analysis::zSIZE; z++)
            for (   int y=0; y<analysis::ySIZE; y++)
               for (int x=0; x<analysis::xSIZE; x++) {
                  
                  projNBS[idx(x,y,z,conf)]
                     = ( orgNBS[NBSidx(x,y,z,0,0,conf)]
                        + (  orgNBS[NBSidx(x,y,z,1,1,conf)]
                           + orgNBS[NBSidx(x,y,z,2,1,conf)]
                           + orgNBS[NBSidx(x,y,z,1,2,conf)]
                           + orgNBS[NBSidx(x,y,z,2,2,conf)] ) * 0.5
                        + orgNBS[NBSidx(x,y,z,3,3,conf)] ) / 3.0;
               }
   }
   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Function for take angular momentum projection
 */
//--------------------------------------------------------------------------
void NBS_WAVE::ang_mom_projection( int mom_flg ) {
    
   func_name = "ang_mom_projection____";
   analysis::route(class_name, func_name, 1);
   
   if (projNBS == NULL) {
      analysis::error(1,"NBS wave function has not projected yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   cdouble *tmp;   tmp = new cdouble[NBSwave::xyznSIZE];
   
   set_rot_matrix();
   
#ifdef _OPENMP
#pragma omp parallel for
#endif
   for (            int conf = 0;   conf<analysis::Nconf; conf++) {
      for (         int rot_type=0; rot_type < 24;        rot_type++)
         for (      int z=0;        z<analysis::zSIZE;    z++)
            for (   int y=0;        y<analysis::ySIZE;    y++)
               for (int x=0;        x<analysis::xSIZE;    x++) {
                  
                  int rot_xyz[3];
                  // projection to anguler momentum l = 0
                  for (int k=0; k<3; k++)
                     rot_xyz[k]
                        = (  rot_matrix[matrix_idx(k,0,rot_type)] * x
                           + rot_matrix[matrix_idx(k,1,rot_type)] * y
                           + rot_matrix[matrix_idx(k,2,rot_type)] * z
                           + rot_matrix[matrix_idx(k,3,rot_type)] * 1
                          ) % analysis::xSIZE;
                     
                  tmp[idx(rot_xyz[0],rot_xyz[1],rot_xyz[2],conf)]
                     += projNBS[idx(x,y,z,conf)];
               }
      for (      int z=0; z<analysis::zSIZE; z++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int x=0; x<analysis::xSIZE; x++)
               tmp[idx(x,y,z,conf)] /= 24.0;
   } // conf
        
   for(size_t n=0; n<NBSwave::xyznSIZE; n++) projNBS[n]=tmp[n];
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

   if (projNBS == NULL) {
      analysis::error(1,"NBS wave function has not projected yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   cdouble *tmp;   tmp = new cdouble[NBSwave::xyznSIZE];
    
   for (         int conf=0; conf<analysis::Nconf; conf++)
      for (      int z=0;    z<analysis::zSIZE;     z++)
         for (   int y=0;    y<analysis::ySIZE;     y++)
            for (int x=0;    x<analysis::xSIZE;     x++) {
               
               tmp[idx(x,y,z,conf)]
                  = (  projNBS[idx(x,y,z,conf)]
                     + projNBS[idx(  (analysis::xSIZE-x)%analysis::xSIZE
                                   , (analysis::ySIZE-y)%analysis::ySIZE
                                   , (analysis::zSIZE-z)%analysis::zSIZE
                                   , conf ) ] ) * 0.5;
               }
   for (size_t n=0; n<NBSwave::xyznSIZE; n++) projNBS[n]=tmp[n];
   delete [] tmp;
   
   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Function for take spin, angular momentum and parity projection at once
 */
//--------------------------------------------------------------------------
void NBS_WAVE::projection( int spin, int ang_mom ) {
    
   spin_projection( spin );
   ang_mom_projection( ang_mom );
   parity_projection();
}
