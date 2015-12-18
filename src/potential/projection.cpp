//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBSwave
 * @brief   Calculate spin, angular momentum and parity projection
 * @brief   for NBS wave function
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <NBSwave.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for taking spin projection
 * @brief spin_flg = 0 -> spin0 , = 1~3 -> spin1, = 4 -> mean of spin1
 */
//--------------------------------------------------------------------------

void NBS_WAVE::spin_projection( int spin_flg ){
    
    func_name = "spin_projection_______";
    route( class_name, func_name, 1 );
    
    if( new_flg_NBSwave ){
        if( spin_flg == 0 ){
            for( int conf=0; conf<N_conf; conf++ )
                for(int z=0; z<xyzSIZE; z++)
                    for(int y=0; y<xyzSIZE; y++)
                        for(int x=0; x<xyzSIZE; x++){
                            
                            NBSwave[xyzn(x,y,z,conf)]
                             = (  orgNBS[NBScood(x,y,z,1,1,conf)]
                                - orgNBS[NBScood(x,y,z,2,1,conf)]
                                - orgNBS[NBScood(x,y,z,1,2,conf)]
                                + orgNBS[NBScood(x,y,z,2,2,conf)] ) * 0.5;
                        }
        }
        else if( spin_flg == 1 ){
            for( int conf=0; conf<N_conf; conf++ )
                for(int z=0; z<xyzSIZE; z++)
                    for(int y=0; y<xyzSIZE; y++)
                        for(int x=0; x<xyzSIZE; x++){
                            
                            NBSwave[xyzn(x,y,z,conf)]
                             = orgNBS[NBScood(x,y,z,0,0,conf)];
                        }
        }
        else if( spin_flg == 2 ){
            for( int conf=0; conf<N_conf; conf++ )
                for(int z=0; z<xyzSIZE; z++)
                    for(int y=0; y<xyzSIZE; y++)
                        for(int x=0; x<xyzSIZE; x++){
                            
                            NBSwave[xyzn(x,y,z,conf)]
                             = (  orgNBS[NBScood(x,y,z,1,1,conf)]
                                + orgNBS[NBScood(x,y,z,2,1,conf)]
                                + orgNBS[NBScood(x,y,z,1,2,conf)]
                                + orgNBS[NBScood(x,y,z,2,2,conf)] ) * 0.5;
                        }
        }
        else if( spin_flg == 3 ){
            for( int conf=0; conf<N_conf; conf++ )
                for(int z=0; z<xyzSIZE; z++)
                    for(int y=0; y<xyzSIZE; y++)
                        for(int x=0; x<xyzSIZE; x++){
                            
                            NBSwave[xyzn(x,y,z,conf)]
                             = orgNBS[NBScood(x,y,z,3,3,conf)];
                        }
        }
        else if( spin_flg == 4 ){
            for( int conf=0; conf<N_conf; conf++ )
                for(int z=0; z<xyzSIZE; z++)
                    for(int y=0; y<xyzSIZE; y++)
                        for(int x=0; x<xyzSIZE; x++){
                            
                            NBSwave[xyzn(x,y,z,conf)]
                             = ( orgNBS[NBScood(x,y,z,0,0,conf)]
                                + (  orgNBS[NBScood(x,y,z,1,1,conf)]
                                   + orgNBS[NBScood(x,y,z,2,1,conf)]
                                   + orgNBS[NBScood(x,y,z,1,2,conf)]
                                   + orgNBS[NBScood(x,y,z,2,2,conf)] ) * 0.5
                                + orgNBS[NBScood(x,y,z,3,3,conf)] ) / 3.0;
                        }
        }
    }else{
        error(1,"Have not setted NBSwave yet !");
    }
    route( class_name, func_name, 0 );
}

//--------------------------------------------------------------------------
/**
 * @brief Function for take angular momentum projection
 */
//--------------------------------------------------------------------------

void NBS_WAVE::ang_mom_projection( int mom_flg ){
    
    func_name = "ang_mom_projection____";
    route( class_name, func_name, 1 );
    
    if( new_flg_NBSwave ){
    cdouble *tmp;   tmp = new cdouble[ NBS_size * N_conf ];
    set_rot_matrix();
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for( int conf = 0; conf < N_conf; conf++ ){
        for(int rot_type=0; rot_type<24; rot_type++)
            for(int z=0; z<xyzSIZE; z++)
                for(int y=0; y<xyzSIZE; y++)
                    for(int x=0; x<xyzSIZE; x++){
                        int rot_xyz[3];
                        
                        // projection to anguler momentum l = 0
                        for(int k=0; k<3; k++){
                
                            rot_xyz[k]
                             =  rot_matrix[ matrix_idx(k,0,rot_type) ] * x
                              + rot_matrix[ matrix_idx(k,1,rot_type) ] * y
                              + rot_matrix[ matrix_idx(k,2,rot_type) ] * z
                              + rot_matrix[ matrix_idx(k,3,rot_type) ] * 1;
                
                            if( rot_xyz[k] == xyzSIZE ) rot_xyz[k] = 0;
                        }
                        tmp[ xyzn( rot_xyz[0], rot_xyz[1], rot_xyz[2], conf ) ]
                            += NBSwave[ xyzn(x,y,z,conf) ] ;
                    }
        for(int z=0; z<xyzSIZE; z++)
            for(int y=0; y<xyzSIZE; y++)
                for(int x=0; x<xyzSIZE; x++)
                    tmp[ xyzn(x,y,z,conf) ] /= 24.0;
    } // conf
        
    for( int n=0; n<NBS_size * N_conf; n++ ) NBSwave[n]=tmp[n];
    delete [] tmp;
    
    }else{
        error(1,"Have not setted NBSwave yet !");
    }
    route( class_name, func_name, 0 );
}

//--------------------------------------------------------------------------
/**
 * @brief Function for take parity projection
 */
//--------------------------------------------------------------------------

void NBS_WAVE::parity_projection(){
    
    func_name = "parity_projection_____";
    route( class_name, func_name, 1 );

    if( new_flg_NBSwave ){
    cdouble *tmp;   tmp = new cdouble[ NBS_size * N_conf ];
    
    for( int conf=0; conf<N_conf; conf++ )
        for(int z=0; z<xyzSIZE; z++)
            for(int y=0; y<xyzSIZE; y++)
                for(int x=0; x<xyzSIZE; x++){
    
                    tmp[ xyzn( x,y,z,conf )]
                     = (  NBSwave[ xyzn( x,y,z,conf ) ]
                        + NBSwave[ xyzn( (xyzSIZE-x)%xyzSIZE
                                       , (xyzSIZE-y)%xyzSIZE
                                       , (xyzSIZE-z)%xyzSIZE, conf ) ] ) * 0.5;
                }
        
        for( int n=0; n<NBS_size * N_conf; n++ ) NBSwave[n]=tmp[n];
        delete [] tmp;
    }else{
        error(1,"Have not setted NBSwave yet !");
    }
    route( class_name, func_name, 0 );
}

//--------------------------------------------------------------------------
/**
 * @brief Function for take spin, angular momentum and parity projection at once
 */
//--------------------------------------------------------------------------

void NBS_WAVE::projection_NBS( int spin, int ang_mom ){
    
    spin_projection( spin );
    ang_mom_projection( ang_mom );
    parity_projection();
}
