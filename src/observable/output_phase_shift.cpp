//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup phase shift
 * @brief   Functions for output phase shift data
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <phase_shift.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for output phase shift data every gauge configurations
 */
//--------------------------------------------------------------------------

void PHASE_SHIFT::output_phase_shift_all( const char* out_file_name ){
    
    func_name = "output_phase_shift_all";
    route( class_name, func_name, 1 );

    if( new_flg_phase_shift ){
        ofstream out_file( out_file_name, ios::out );
    
        for( int E=E_min; E<=E_max; E++ )
            for(int i=0; i<N_conf; i++)
                out_file << E << " " << phase_shift[ idx(i,E-E_min) ] << endl;
        
        out_file.close();
    }else
        error(1,"Have not calculated phase shift yet !");

    route( class_name, func_name, 0 );
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output mean of phase shift data with error
 */
//--------------------------------------------------------------------------

void PHASE_SHIFT::output_phase_shift_err( const char* out_file_name ){
    
    func_name = "output_phase_shift_err";
    route( class_name, func_name, 1 );

    if( new_flg_phase_shift ){
        double err, mean, sqr_mean;
        ofstream out_file( out_file_name, ios::out );

        for( int E=E_min; E<=E_max; E++ ){
            mean     = 0.0;
            sqr_mean = 0.0;
            
            for(int i=0; i<N_conf; i++){

                mean     +=      phase_shift[ idx(i,E-E_min) ];
                sqr_mean += pow( phase_shift[ idx(i,E-E_min) ],2 );
            }
            mean     /= double(N_conf);
            sqr_mean /= double(N_conf);

            err = sqrt( double(N_conf-1) * (sqr_mean - mean*mean) );
            
            out_file << E << " " << mean << " " << err << endl;
        }
        out_file.close();
    }else
        error(1,"Have not calculated phase shift yet !");
    
    route( class_name, func_name, 0 );
}
