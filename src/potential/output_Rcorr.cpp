//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup R-correlator
 * @brief   Functions for output R-correlator data
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <R_correlator.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for output R-correlators data every gauge configurations
 */
//--------------------------------------------------------------------------
    
void R_CORRELATOR::output_Rcorr_all( const char* out_file_PATH ){
    
    func_name = "output_Rcorr_all______";
    route( class_name, func_name, 1 );

    if( Rcorr != NULL ){
        string hadron_name = channel_to_name(channel);
        char out_file_name[1024];
        snprintf( out_file_name, sizeof(out_file_name),
                 "%s/%s_Rcorr_all_t%d"
                 , out_file_PATH, hadron_name.c_str(), time_slice );
        ofstream out_file( out_file_name, ios::out | ios::binary );
    
        for(int z=0; z<xyzSIZE; z++)
            for(int y=0; y<xyzSIZE; y++)
                for(int x=0; x<xyzSIZE; x++)
                    for(int i=0; i<N_conf; i++)
                        out_file.write( (char*)&Rcorr[ xyzn(x,y,z,i) ]
                                      , sizeof(cdouble) );
        out_file.close();
    }else{
        error(1,"Have not setted R correlator yet !");
    }
    route( class_name, func_name, 0 );
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output mean of R-correlators data with error
 */
//--------------------------------------------------------------------------

void R_CORRELATOR::output_Rcorr_err( const char* out_file_PATH ){
    
    func_name = "output_Rcorr_err______";
    route( class_name, func_name, 1 );

    if( Rcorr != NULL ){
        int x_shift, y_shift, z_shift;
        double R;
        cdouble err, mean, sqr_mean;
        
        string hadron_name = channel_to_name(channel);
        char out_file_name[1024];
        snprintf( out_file_name, sizeof(out_file_name),
                 "%s/%s_Rcorr_err_t%d"
                 , out_file_PATH, hadron_name.c_str(), time_slice );
        ofstream out_file( out_file_name, ios::out );

        for(int z=0; z<xyzSIZE; z++)
            for(int y=0; y<xyzSIZE; y++)
                for(int x=0; x<xyzSIZE; x++){
                    mean     = cdouble( 0.0, 0.0 );
                    sqr_mean = cdouble( 0.0, 0.0 );
                    
                    for(int i=0; i<N_conf; i++){
                        mean     += Rcorr[ xyzn(x,y,z,i) ];
                        sqr_mean += cmp_sqr( Rcorr[ xyzn(x,y,z,i) ] );
                    }
                    mean     /= double(N_conf);
                    sqr_mean /= double(N_conf);
                    
                    err = cmp_sqrt( double(N_conf-1)
                                   * ( sqr_mean - cmp_sqr( mean )));
                    
                    x_shift = 0, y_shift = 0, z_shift = 0;
                    if( x > xyzSIZE/2 ){ x_shift = xyzSIZE; }
                    if( y > xyzSIZE/2 ){ y_shift = xyzSIZE; }
                    if( z > xyzSIZE/2 ){ z_shift = xyzSIZE; }
                    // make r for each coodinate with periodic boundary condition
                    R = sqrt( (x - x_shift)*(x - x_shift)
                             + (y - y_shift)*(y - y_shift)
                             + (z - z_shift)*(z - z_shift) );
                    
                    out_file << R << " " << mean.real() << " " << err.real()
                    << " " << mean.imag() << " " << err.imag() << endl;
                }
        out_file.close();
    }else{
        error(1,"Have not setted R correlator yet !");
    }
    route( class_name, func_name, 0 );
}
