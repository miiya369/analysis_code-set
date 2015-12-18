//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup R-correlator
 * @brief   Functions for calculate R-correlator
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <R_correlator.h>

//--------------------------------------------------------------------------
/**
 * @brief Calculate R-correlator by input correlator files
 */
//--------------------------------------------------------------------------

void R_CORRELATOR::input_Rcorr( int spin, int angmom ){
    
    func_name = "input_Rcorr___________";
    route( class_name, func_name, 1 );
    
    input_NBS();
    projection_NBS( spin, angmom );   // construct NBS wave
    make_JK_sample_NBS(1);
    
    HAD_name = HAD1_name;   // construct correlator 1
    input_corr();
    make_JK_sample_corr(1);
    
    cdouble *tmp_corr = new cdouble[ N_conf ];
    
    if( HAD1_name == HAD2_name ){   // when used same baryon
        for( int conf = 0; conf < N_conf; conf++ )
            tmp_corr[conf] = 2.0 * corr[ nt(conf,time_slice) ];
        delete_corr();
    }else{   // when used different baryon
        for( int conf = 0; conf < N_conf; conf++ )
            tmp_corr[conf] = corr[ nt(conf,time_slice) ];
        delete_corr();
        
        HAD_name = HAD2_name;
        input_corr();
        make_JK_sample_corr(1);
        
        for( int conf = 0; conf < N_conf; conf++ )
            tmp_corr[conf] *= corr[ nt(conf,time_slice) ];
        delete_corr();
    }
    
    Rcorr =  new cdouble[ NBS_size * N_conf ];
    new_flg_Rcorr = true;

    for( int conf = 0; conf < N_conf; conf++ )
        for(int z=0; z<xyzSIZE; z++)
            for(int y=0; y<xyzSIZE; y++)
                for(int x=0; x<xyzSIZE; x++)
            Rcorr[ xyzn(x,y,z,conf) ] = NBSwave[ xyzn(x,y,z,conf) ]
                                        / tmp_corr[conf];
    delete_NBS();
    delete [] tmp_corr;
    
    route( class_name, func_name, 0 );
}

//--------------------------------------------------------------------------
/**
 * @brief Calculate R-correlator using prepared correlator pointer
 */
//--------------------------------------------------------------------------

void R_CORRELATOR::input_Rcorr( int spin, int angmom
                               , cdouble *corr1, cdouble *corr2 ){
    
    func_name = "input_Rcorr___________";
    route( class_name, func_name, 1 );
    
    input_NBS();
    projection_NBS( spin, angmom );   // construct NBS wave
    make_JK_sample_NBS(1);
    
    Rcorr = new cdouble[ NBS_size * N_conf ];
    new_flg_Rcorr = true;
    
    for( int conf = 0; conf < N_conf; conf++ )
        for(int z=0; z<xyzSIZE; z++)
            for(int y=0; y<xyzSIZE; y++)
                for(int x=0; x<xyzSIZE; x++)
            Rcorr[ xyzn(x,y,z,conf) ] = NBSwave[ xyzn(x,y,z,conf) ]
                                       / ( corr1[ nt(conf,time_slice) ]
                                         * corr2[ nt(conf,time_slice) ] );
    delete_NBS();
    
    route( class_name, func_name, 0 );
}
