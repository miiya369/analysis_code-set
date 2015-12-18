//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Functions for output potential data
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <potential.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for output potentials data every gauge configurations
 */
//--------------------------------------------------------------------------

void POTENTIAL::output_single_pot_all( const char* out_file_PATH ){
    
    func_name = "output_single_pot_all_";
    route( class_name, func_name, 1 );

    if( new_flg_pot ){
        string hadron_name = channel_to_name(channel);
        char out_file_name[1024];
        snprintf( out_file_name, sizeof(out_file_name),
                 "%s/%s_%s_all_t%d"
                 , out_file_PATH, hadron_name.c_str()
                 , potential_type.c_str(), time_slice );
        ofstream out_file( out_file_name, ios::out );
        
        cdouble tmp;
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
            
                    for(int i=0; i<N_conf; i++){
                        tmp =         potential[ xyzn(x,y,z,i) ]
                                / Rcorr1->Rcorr[ xyzn(x,y,z,i) ];
                        
                        out_file << R << " " << tmp.real() << " " << tmp.imag()
                                 << endl;
                    }
                }
        out_file.close();
    }else{
        error(1,"Have not calculated potential yet !");
    }
    route( class_name, func_name, 0 );
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output mean of potentials data with error
 */
//--------------------------------------------------------------------------

void POTENTIAL::output_single_pot_err( const char* out_file_PATH ){
    
    func_name = "output_single_pot_err_";
    route( class_name, func_name, 1 );
    
    if( new_flg_pot ){
        int x_shift, y_shift, z_shift;
        double R;
        cdouble err, mean, sqr_mean;
        
        string hadron_name = channel_to_name(channel);
        char out_file_name[1024];
        snprintf( out_file_name, sizeof(out_file_name),
                 "%s/%s_%s_err_t%d"
                 , out_file_PATH, hadron_name.c_str()
                 , potential_type.c_str(), time_slice );
        ofstream out_file( out_file_name, ios::out );
        
        for(int z=0; z<xyzSIZE; z++)
            for(int y=0; y<xyzSIZE; y++)
                for(int x=0; x<xyzSIZE; x++){
                    mean     = cdouble( 0.0, 0.0 );
                    sqr_mean = cdouble( 0.0, 0.0 );
                    
                    for(int i=0; i<N_conf; i++){
                        mean     += (      potential[ xyzn(x,y,z,i) ]
                                     / Rcorr1->Rcorr[ xyzn(x,y,z,i) ] );
                        sqr_mean += cmp_sqr(      potential[ xyzn(x,y,z,i) ]
                                            / Rcorr1->Rcorr[ xyzn(x,y,z,i) ] );
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
                    R = sqrt(  (x - x_shift)*(x - x_shift)
                             + (y - y_shift)*(y - y_shift)
                             + (z - z_shift)*(z - z_shift) );
                    
                    out_file << R << " " << mean.real() << " " << err.real()
                    << " " << mean.imag() << " " << err.imag() << endl;
                }
        out_file.close();
    }else{
        error(1,"Have not calculated potential yet !");
    }
    route( class_name, func_name, 0 );
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output miyamoto-format fitting data of potentail
 */
//--------------------------------------------------------------------------

void POTENTIAL::output_single_pot_fit( const char* out_file_PATH ){
    
    func_name = "output_single_pot_fit_";
    route( class_name, func_name, 1 );
    
    if( new_flg_pot ){
        double *err             = new double[xyzSIZE*xyzSIZE*xyzSIZE];
        double *potential_tmp   = new double[xyzSIZE*xyzSIZE*xyzSIZE * N_conf];
        double mean, sqr_mean;
        cdouble tmp;
        
        for(int z=0; z<xyzSIZE; z++)
            for(int y=0; y<xyzSIZE; y++)
                for(int x=0; x<xyzSIZE; x++)
                    for(int i=0; i<N_conf; i++){
                        tmp = potential[ xyzn(x,y,z,i) ]
                        / Rcorr1->Rcorr[ xyzn(x,y,z,i) ];
                        potential_tmp[ xyzn(x,y,z,i) ] = tmp.real();
                    }
        for(int z=0; z<xyzSIZE; z++)
            for(int y=0; y<xyzSIZE; y++)
                for(int x=0; x<xyzSIZE; x++){
                    mean     = 0.0;
                    sqr_mean = 0.0;
            
                    for(int i=0; i<N_conf; i++){
                        mean     += potential_tmp[ xyzn(x,y,z,i) ];
                        sqr_mean += pow( potential_tmp[ xyzn(x,y,z,i) ],2 );
                    }
                    mean     /= double(N_conf);
                    sqr_mean /= double(N_conf);
            
                    err[ xyz(x,y,z) ] = sqrt( double(N_conf-1)
                                             * ( sqr_mean - mean*mean ) );
                }
        
        int test1[1000];   // <- For debug !
        int test2[1000];   // <- For debug !
        
        int fit_type = 2;
        int data_count;
        
        //! If data reduction is wrong, chenge this one
        double reduction_precision = 0.0000000001;
        
        double R;
        int x_shift, y_shift, z_shift;
        
        string hadron_name = channel_to_name(channel);
        char out_file_name[1024];
        snprintf( out_file_name, sizeof(out_file_name),
                 "%s/%s_%s_fitdata_t%d"
                 , out_file_PATH, hadron_name.c_str()
                 , potential_type.c_str(), time_slice );
        ofstream out_file( out_file_name, ios::out | ios::binary );
        out_file.write( (char*)&fit_type, sizeof(int) );
        out_file.write( (char*)&N_conf, sizeof(int) );   // write file header
        
        for( int i=0; i<N_conf; i++){
            data_count = xyzSIZE*xyzSIZE*xyzSIZE;   // data reduction
            
            for(int j=0; j<xyzSIZE*xyzSIZE*xyzSIZE-1; j++){
                if( potential_tmp[ j + xyzSIZE*xyzSIZE*xyzSIZE* i ] == 0.0 )
                    continue;
                for( int k=j+1; k<xyzSIZE*xyzSIZE*xyzSIZE; k++ ){
                    if( potential_tmp[ j + xyzSIZE*xyzSIZE*xyzSIZE* i ]
                       + reduction_precision >=
                        potential_tmp[ k + xyzSIZE*xyzSIZE*xyzSIZE* i ] &&
                        potential_tmp[ j + xyzSIZE*xyzSIZE*xyzSIZE* i ]
                       - reduction_precision <=
                        potential_tmp[ k + xyzSIZE*xyzSIZE*xyzSIZE* i ] ){
                        
                        potential_tmp[ k + xyzSIZE*xyzSIZE*xyzSIZE* i ] = 0.0;
                        data_count--;
                    }
                }
            }
            printf(" @ Data count = %d\n",data_count);   // <- For debug !
            out_file.write( (char*)&data_count, sizeof(int) );
            test1[i]=data_count;   // <- For debug !
        }
        for(int i=0; i<N_conf; i++){
            data_count = 0;
            for(int z=0; z<xyzSIZE; z++)
                for(int y=0; y<xyzSIZE; y++)
                    for(int x=0; x<xyzSIZE; x++){
                        if( potential_tmp[ xyzn(x,y,z,i) ] != 0.0 ){
                    
                            x_shift = 0, y_shift = 0, z_shift = 0;
                            if( x > xyzSIZE/2 ){ x_shift = xyzSIZE; }
                            if( y > xyzSIZE/2 ){ y_shift = xyzSIZE; }
                            if( z > xyzSIZE/2 ){ z_shift = xyzSIZE; }
                            
                            R = sqrt( (x - x_shift)*(x - x_shift)
                                    + (y - y_shift)*(y - y_shift)
                                    + (z - z_shift)*(z - z_shift) );
                    
                            out_file.write( (char*)&R, sizeof(double) );
                            out_file.write((char*)&potential_tmp[ xyzn(x,y,z,i) ]
                                           , sizeof(double) );   // write data
                            out_file.write( (char*)&err[ xyz(x,y,z) ]
                                           , sizeof(double) );
                            data_count++;
                        }
                    }
            test2[i]=data_count;   // <- For debug !
        }
        out_file.close();
        delete [] err;
        delete [] potential_tmp;
        
        int check_data=0;                                   // <- For debug !
        for( int i=0; i<N_conf; i++ )                       // <- For debug !
            if(test1[i]!=test2[i]) check_data++;            // <- For debug !
        printf(" @@@@@@ DATA CHECKING : %d\n",check_data);  // <- For debug !
        
    }else{
        error(1,"Have not calculated potential yet !");
    }
    route( class_name, func_name, 0 );
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output mean of potentials data with error (for CCP)
 */
//--------------------------------------------------------------------------

void POTENTIAL::output_couple_pot_err( const char* out_file_PATH ){
    
    func_name = "output_couple_pot_err_";
    route( class_name, func_name, 1 );
    
    if( new_flg_pot ){
        int x_shift, y_shift, z_shift;
        double R;
        cdouble err, mean, sqr_mean;
        
        string hadron_name = channel_to_name(channel);
        char out_file_name[1024];
        snprintf( out_file_name, sizeof(out_file_name),
                 "%s/%s_%s_err_t%d"
                 , out_file_PATH, hadron_name.c_str()
                 , potential_type.c_str(), time_slice );
        ofstream out_file( out_file_name, ios::out );
        
        for(int z=0; z<xyzSIZE; z++)
            for(int y=0; y<xyzSIZE; y++)
                for(int x=0; x<xyzSIZE; x++){
                    mean     = cdouble( 0.0, 0.0 );
                    sqr_mean = cdouble( 0.0, 0.0 );
                    
                    for(int i=0; i<N_conf; i++){
                        mean     += potential[ xyzn(x,y,z,i) ];
                        sqr_mean += cmp_sqr( potential[ xyzn(x,y,z,i) ] );
                    }
                    mean     /= double(N_conf);
                    sqr_mean /= double(N_conf);
                    
                    err = cmp_sqrt( double( N_conf-1 )
                                   * ( sqr_mean - cmp_sqr( mean )));
                    
                    x_shift = 0, y_shift = 0, z_shift = 0;
                    if( x > xyzSIZE/2 ){ x_shift = xyzSIZE; }
                    if( y > xyzSIZE/2 ){ y_shift = xyzSIZE; }
                    if( z > xyzSIZE/2 ){ z_shift = xyzSIZE; }
                    // make r for each coodinate with periodic boundary condition
                    R = sqrt(  (x - x_shift)*(x - x_shift)
                             + (y - y_shift)*(y - y_shift)
                             + (z - z_shift)*(z - z_shift) );
                    
                    out_file << R << " " << mean.real() << " " << err.real()
                             << " " << mean.imag() << " " << err.imag() << endl;
                }
        out_file.close();

    }else{
        error(1,"Have not calculated potential yet !");
    }
    route( class_name, func_name, 0 );
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output miyamoto-format fitting data of potentail (for CCP)
 */
//--------------------------------------------------------------------------

void POTENTIAL::output_couple_pot_fit( const char* out_file_PATH ){
    
    func_name = "output_couple_pot_fit_";
    route( class_name, func_name, 1 );
    
    if( new_flg_pot ){
        double *err             = new double[xyzSIZE*xyzSIZE*xyzSIZE];
        double *potential_tmp   = new double[xyzSIZE*xyzSIZE*xyzSIZE * N_conf];
        double mean, sqr_mean;
        
        for(int z=0; z<xyzSIZE; z++)
            for(int y=0; y<xyzSIZE; y++)
                for(int x=0; x<xyzSIZE; x++)
                    for(int i=0; i<N_conf; i++){
                        potential_tmp[ xyzn(x,y,z,i) ]
                          = potential[ xyzn(x,y,z,i) ].real();
                    }
        for(int z=0; z<xyzSIZE; z++)
            for(int y=0; y<xyzSIZE; y++)
                for(int x=0; x<xyzSIZE; x++){
                    mean     = 0.0;
                    sqr_mean = 0.0;
                    
                    for(int i=0; i<N_conf; i++){
                        mean     += potential_tmp[ xyzn(x,y,z,i) ];
                        sqr_mean += pow( potential_tmp[ xyzn(x,y,z,i) ],2 );
                    }
                    mean     /= double(N_conf);
                    sqr_mean /= double(N_conf);
                    
                    err[ xyz(x,y,z) ] = sqrt( double(N_conf-1)
                                             * ( sqr_mean - mean*mean ) );
                }
        
        int test1[1000];   // <- For debug !
        int test2[1000];   // <- For debug !
        
        int fit_type = 2;
        int data_count;
        
        //! If data reduction is wrong, chenge this one
        double reduction_precision = 0.0000000001;
        
        double R;
        int x_shift, y_shift, z_shift;
        
        string hadron_name = channel_to_name(channel);
        char out_file_name[1024];
        snprintf( out_file_name, sizeof(out_file_name),
                 "%s/%s_%s_fitdata_t%d"
                 , out_file_PATH, hadron_name.c_str()
                 , potential_type.c_str(), time_slice );
        ofstream out_file( out_file_name, ios::out | ios::binary );
        out_file.write( (char*)&fit_type, sizeof(int) );
        out_file.write( (char*)&N_conf, sizeof(int) );   // write file header
        
        for( int i=0; i<N_conf; i++){
            data_count = xyzSIZE*xyzSIZE*xyzSIZE;   // data reduction
            
            for(int j=0; j<xyzSIZE*xyzSIZE*xyzSIZE-1; j++){
                if( potential_tmp[ j + xyzSIZE*xyzSIZE*xyzSIZE* i ] == 0.0 )
                    continue;
                for( int k=j+1; k<xyzSIZE*xyzSIZE*xyzSIZE; k++ ){
                    if( potential_tmp[ j + xyzSIZE*xyzSIZE*xyzSIZE* i ]
                       + reduction_precision >=
                       potential_tmp[ k + xyzSIZE*xyzSIZE*xyzSIZE* i ] &&
                       potential_tmp[ j + xyzSIZE*xyzSIZE*xyzSIZE* i ]
                       - reduction_precision <=
                       potential_tmp[ k + xyzSIZE*xyzSIZE*xyzSIZE* i ] ){
                        
                        potential_tmp[ k + xyzSIZE*xyzSIZE*xyzSIZE* i ] = 0.0;
                        data_count--;
                    }
                }
            }
            printf(" @ Data count = %d\n",data_count);   // <- For debug !
            out_file.write( (char*)&data_count, sizeof(int) );
            test1[i]=data_count;   // <- For debug !
        }
        for(int i=0; i<N_conf; i++){
            data_count = 0;
            for(int z=0; z<xyzSIZE; z++)
                for(int y=0; y<xyzSIZE; y++)
                    for(int x=0; x<xyzSIZE; x++){
                        if( potential_tmp[ xyzn(x,y,z,i) ] != 0.0 ){
                            
                            x_shift = 0, y_shift = 0, z_shift = 0;
                            if( x > xyzSIZE/2 ){ x_shift = xyzSIZE; }
                            if( y > xyzSIZE/2 ){ y_shift = xyzSIZE; }
                            if( z > xyzSIZE/2 ){ z_shift = xyzSIZE; }
                            
                            R = sqrt( (x - x_shift)*(x - x_shift)
                                     + (y - y_shift)*(y - y_shift)
                                     + (z - z_shift)*(z - z_shift) );
                            
                            out_file.write( (char*)&R, sizeof(double) );
                            out_file.write((char*)&potential_tmp[ xyzn(x,y,z,i) ]
                                           , sizeof(double) );   // write data
                            out_file.write( (char*)&err[ xyz(x,y,z) ]
                                           , sizeof(double) );
                            data_count++;
                        }
                    }
            test2[i]=data_count;   // <- For debug !
        }
        out_file.close();
        delete [] err;
        delete [] potential_tmp;
        
        int check_data=0;                                   // <- For debug !
        for( int i=0; i<N_conf; i++ )                       // <- For debug !
            if(test1[i]!=test2[i]) check_data++;            // <- For debug !
        printf(" @@@@@@ DATA CHECKING : %d\n",check_data);  // <- For debug !
        
    }else{
        error(1,"Have not calculated potential yet !");
    }
    route( class_name, func_name, 0 );
}
