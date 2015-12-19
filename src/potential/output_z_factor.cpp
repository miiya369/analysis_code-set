//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Correlator
 * @brief   Functions for output Z-factor data
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <correlator.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for output Z-factor data every gauge configurations
 */
//--------------------------------------------------------------------------

void CORRELATOR::output_z_factor_all( const char* out_file_PATH ){
    
    func_name = "output_z_factor_all___";
    route( class_name, func_name, 1 );
    
    if( Z_factor != NULL ){
        char out_file_name[1024];
        snprintf( out_file_name, sizeof(out_file_name),
                 "%s/%s_Z-factor_all", out_file_PATH, HAD_name.c_str() );
        ofstream outFILE( out_file_name, ios::out );
        
        for(int t=0; t < tSIZE; t++) for( int i=0; i<N_conf; i++)
            outFILE << t << " " << Z_factor[ nt(i,t) ].real()
                         << " " << Z_factor[ nt(i,t) ].imag() << endl;
        
        outFILE.close();
    }else{
        error(1,"Have not calculated Z-factor yet !");
    }
    route( class_name, func_name, 0 );
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output mean of Z-factor data with error
 */
//--------------------------------------------------------------------------

void CORRELATOR::output_z_factor_err( const char* out_file_PATH ){
    
    func_name = "output_z_factor_err___";
    route( class_name, func_name, 1 );
    
    if( Z_factor != NULL ){
        cdouble err, mean, sqr_mean;
        
        char out_file_name[1024];
        snprintf( out_file_name, sizeof(out_file_name),
                 "%s/%s_Z-factor_err", out_file_PATH, HAD_name.c_str() );
        ofstream outFILE( out_file_name, ios::out );
        
        for( int t=0; t < tSIZE; t++){
            mean     = cdouble( 0.0, 0.0 );
            sqr_mean = cdouble( 0.0, 0.0 );
            
            for( int i=0; i<N_conf; i++){
                mean     += Z_factor[ nt(i,t) ];
                sqr_mean += cmp_sqr( Z_factor[ nt(i,t) ] );
            }
            mean     /= double(N_conf);
            sqr_mean /= double(N_conf);
            
            err = cmp_sqrt( ( sqr_mean - cmp_sqr( mean ) ) / double(N_conf-1) );
            
            outFILE << t << " " << mean.real() << " " << err.real()
            << " " << mean.imag() << " " << err.imag() << endl;
        }
        outFILE.close();
    }else{
        error(1,"Have not calculated Z-factor yet !");
    }
    route( class_name, func_name, 0 );
}
