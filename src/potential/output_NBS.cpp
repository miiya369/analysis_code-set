//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBSwave
 * @brief   Functions for output NBS wave function data
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <NBSwave.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for output NBS wave functions data every gauge configurations
 */
//--------------------------------------------------------------------------

void NBS_WAVE::output_NBS_all( const char* out_file_PATH ){
    
    func_name = "output_NBS_all________";
    route( class_name, func_name, 1 );

    if( new_flg_NBSwave ){
        string hadron_name = channel_to_name(channel);
        char out_file_name[1024];
        snprintf( out_file_name, sizeof(out_file_name),
                 "%s/%s_NBS_all_t%d"
                 , out_file_PATH, hadron_name.c_str(), time_slice );
        ofstream out_file( out_file_name, ios::out );
        
        double R, x_shift, y_shift, z_shift;
        for(int z=0; z<xyzSIZE; z++)
            for(int y=0; y<xyzSIZE; y++)
                for(int x=0; x<xyzSIZE; x++){
                    
                    x_shift = 0, y_shift = 0, z_shift = 0;
                    if( x > xyzSIZE/2 ){ x_shift = xyzSIZE; }
                    if( y > xyzSIZE/2 ){ y_shift = xyzSIZE; }
                    if( z > xyzSIZE/2 ){ z_shift = xyzSIZE; }
                    // make r for each coodinate with periodic boundary condition
                    R = sqrt( (x - x_shift)*(x - x_shift)
                            + (y - y_shift)*(y - y_shift)
                            + (z - z_shift)*(z - z_shift) );
        
                    for(int i=0; i<N_conf; i++)
                        out_file << R << " " << NBSwave[ xyzn(x,y,z,i) ].real()
                        << " " << NBSwave[ xyzn(x,y,z,i) ].imag() << endl;
                }
        out_file.close();
    }else{
        error(1,"Have not setted NBSwave yet !");
    }
    route( class_name, func_name, 0 );
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output mean of NBS wave functions data with error
 */
//--------------------------------------------------------------------------

void NBS_WAVE::output_NBS_err( const char* out_file_PATH ){
    
    func_name = "output_NBS_err________";
    route( class_name, func_name, 1 );

    if( new_flg_NBSwave ){
        int x_shift, y_shift, z_shift;
        double R;
        cdouble err, mean, sqr_mean;
        
        string hadron_name = channel_to_name(channel);
        char out_file_name[1024];
        snprintf( out_file_name, sizeof(out_file_name),
                 "%s/%s_NBS_err_t%d"
                 , out_file_PATH, hadron_name.c_str(), time_slice );
        ofstream out_file( out_file_name, ios::out );

        for(int z=0; z<xyzSIZE; z++)
            for(int y=0; y<xyzSIZE; y++)
                for(int x=0; x<xyzSIZE; x++){
                    mean     = cdouble( 0.0, 0.0 );
                    sqr_mean = cdouble( 0.0, 0.0 );
                    
                    for(int i=0; i<N_conf; i++){
                        mean     += NBSwave[ xyzn(x,y,z,i) ];
                        sqr_mean += cmp_sqr( NBSwave[ xyzn(x,y,z,i) ] );
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
        error(1,"Have not setted NBSwave yet !");
    }
    route( class_name, func_name, 0 );
}
