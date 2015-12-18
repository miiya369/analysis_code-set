//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup scattering length
 * @brief   Functions for output scattering length data
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <observable.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for output scattering length data every gauge configurations
 */
//--------------------------------------------------------------------------

void OBSERVABLE::output_scatt_len_all( const char* out_file_name ){
    
    func_name = "output_scatt_len_all__";
    route( class_name, func_name, 1 );
    
    if( new_flg_scatt_length ){
        ofstream out_file( out_file_name, ios::out );
        
        for( int E=E_min; E<=E_max; E++ )
            for(int i=0; i<N_conf; i++)
                out_file << sqrt( 2 * mass * E ) << " "
                         << scatt_length[ idx(i,E-E_min) ] << endl;
        
        out_file.close();
    }else
        error(1,"Have not calculated scattering length yet !");
    
    route( class_name, func_name, 0 );
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output mean of scattering length data with error
 */
//--------------------------------------------------------------------------

void OBSERVABLE::output_scatt_len_err( const char* out_file_name ){
    
    func_name = "output_scatt_len_err__";
    route( class_name, func_name, 1 );
    
    if( new_flg_scatt_length ){
        double err, mean, sqr_mean;
        ofstream out_file( out_file_name, ios::out );
        
        for( int E=E_min; E<=E_max; E++ ){
            mean     = 0.0;
            sqr_mean = 0.0;
            
            for(int i=0; i<N_conf; i++){
                
                mean     +=      scatt_length[ idx(i,E-E_min) ];
                sqr_mean += pow( scatt_length[ idx(i,E-E_min) ],2 );
            }
            mean     /= double(N_conf);
            sqr_mean /= double(N_conf);
            
            err = sqrt( double(N_conf-1) * ( sqr_mean - pow(mean,2) ));
            
            out_file << sqrt( 2 * mass * E ) << " " << mean << " " << err << endl;
        }
        out_file.close();
    }else
        error(1,"Have not calculated scattering length yet !");
    
    route( class_name, func_name, 0 );
}
