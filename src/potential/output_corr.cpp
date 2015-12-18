//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Correlator
 * @brief   Functions for output correlator / effective mass data
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <correlator.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for output correlators data every gauge configurations
 */
//--------------------------------------------------------------------------

void CORRELATOR::output_corr_all( const char* out_file_PATH ){
    
    func_name = "output_corr_all_______";
    route( class_name, func_name, 1 );
    
    if( corr != NULL ){
        char out_file_name[1024];
        snprintf( out_file_name, sizeof(out_file_name),
                 "%s/%s_corr_all", out_file_PATH, HAD_name.c_str() );
        ofstream outFILE( out_file_name, ios::out | ios::binary );
        
        for(int t=0; t < tSIZE; t++) for( int i=0; i<N_conf; i++)
            outFILE.write( (char*)&corr[ nt(i,t) ], sizeof(cdouble) );
        
        outFILE.close();
    }else{
        error(1,"Have not setted correlator yet !");
    }
    route( class_name, func_name, 0 );
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output mean of correlators data with error
 */
//--------------------------------------------------------------------------

void CORRELATOR::output_corr_err( const char* out_file_PATH ){
    
    func_name = "output_corr_err_______";
    route( class_name, func_name, 1 );
    
    if( corr != NULL ){
        cdouble err, mean, sqr_mean;
        
        char out_file_name[1024];
        snprintf( out_file_name, sizeof(out_file_name),
                 "%s/%s_corr_err", out_file_PATH, HAD_name.c_str() );
        ofstream outFILE( out_file_name, ios::out );
        
        for( int t=0; t < tSIZE; t++){
            mean     = cdouble( 0.0, 0.0 );
            sqr_mean = cdouble( 0.0, 0.0 );
            
            for( int i=0; i<N_conf; i++){
                mean     += corr[ nt(i,t) ];
                sqr_mean += cmp_sqr( corr[ nt(i,t) ] );
            }
            mean     /= double(N_conf);
            sqr_mean /= double(N_conf);
            
            err = cmp_sqrt( ( sqr_mean - cmp_sqr( mean ) ) * double(N_conf-1) );
            
            outFILE << t << " " << mean.real() << " " << err.real()
            << " " << mean.imag() << " " << err.imag() << endl;
        }
        outFILE.close();
    }else{
        error(1,"Have not setted correlator yet !");
    }
    route( class_name, func_name, 0 );
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output miyamoto-format fitting data of correlators
 */
//--------------------------------------------------------------------------

void CORRELATOR::output_corr_fit( const char* out_file_PATH ){
 
    func_name = "output_corr_fit_______";
    route( class_name, func_name, 1 );
    
    if( corr != NULL ){
        double err, mean, sqr_mean;
        double *tmp_corr = new double[ tSIZE * N_conf ];
        for( int i=0; i<N_conf; i++) for( int t=0; t < tSIZE; t++)
            tmp_corr[ nt(i,t) ] = corr[ nt(i,t) ].real();
        
        int fit_type = 1;
        char out_file_name[1024];
        snprintf( out_file_name, sizeof(out_file_name),
                 "%s/%s_corr_fitdata", out_file_PATH, HAD_name.c_str() );
        ofstream outFILE( out_file_name, ios::out | ios::binary );
        outFILE.write( (char*)&fit_type, sizeof(int) );
        outFILE.write( (char*)&N_conf, sizeof(int) );
        outFILE.write( (char*)&tSIZE, sizeof(int) );   // write file header
        
        for( int t=0; t < tSIZE; t++){
            mean     = 0.0;
            sqr_mean = 0.0;
            
            for( int i=0; i<N_conf; i++){
                mean     += tmp_corr[ nt(i,t) ];
                sqr_mean += pow( tmp_corr[ nt(i,t) ],2 );
            }
            mean     /= double(N_conf);
            sqr_mean /= double(N_conf);
            
            err = sqrt( ( sqr_mean - mean*mean ) * double(N_conf-1) );
            
            outFILE.write( (char*)&err, sizeof(double) );   // write fit data
            for( int i=0; i<N_conf; i++)
                outFILE.write( (char*)&tmp_corr[ nt(i,t) ], sizeof(double) );
        }
        outFILE.close();
        delete [] tmp_corr;
    }else{
        error(1,"Have not setted correlator yet !");
    }
    route( class_name, func_name, 0 );
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output effective masses data every gauge configurations
 */
//--------------------------------------------------------------------------

void CORRELATOR::output_effmass_all( const char* out_file_PATH ){
    
    func_name = "output_effmass_all____";
    route( class_name, func_name, 1 );

    if( corr != NULL ){
        cdouble *eff_mass = new cdouble[ (tSIZE-1) * N_conf ];
        
        for( int t=0; t < tSIZE - 1; t++) for( int i=0; i<N_conf; i++)
            eff_mass[ nt(i,t) ] = -log( corr[ nt(i,t+1) ] / corr[ nt(i,t) ] );
        
        char out_file_name[1024];
        snprintf( out_file_name, sizeof(out_file_name),
                 "%s/%s_effmass_all", out_file_PATH, HAD_name.c_str() );
        ofstream outFILE( out_file_name, ios::out | ios::binary );
        
        for(int t=0; t < tSIZE-1; t++) for( int i=0; i<N_conf; i++)
            outFILE.write( (char*)&eff_mass[ nt(i,t) ], sizeof(cdouble) );
        
        outFILE.close();
        delete [] eff_mass;
    }else{
        error(1,"Have not setted correlator yet !");
    }
    route( class_name, func_name, 0 );
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output mean of effective masses data with error
 */
//--------------------------------------------------------------------------

void CORRELATOR::output_effmass_err( const char* out_file_PATH ){
    
    func_name = "output_effmass_err____";
    route( class_name, func_name, 1 );
    
    if( corr != NULL ){
        cdouble *eff_mass = new cdouble[ (tSIZE-1) * N_conf ];
        
        for( int t=0; t < tSIZE - 1; t++) for( int i=0; i<N_conf; i++)
            eff_mass[ nt(i,t) ] = -log( corr[ nt(i,t+1) ] / corr[ nt(i,t) ] );
        
        cdouble err, mean, sqr_mean;
        
        char out_file_name[1024];
        snprintf( out_file_name, sizeof(out_file_name),
                 "%s/%s_effmass_err", out_file_PATH, HAD_name.c_str() );
        ofstream outFILE( out_file_name, ios::out );
        
        for( int t=0; t < tSIZE-1 ; t++){
            mean     = cdouble( 0.0, 0.0 );
            sqr_mean = cdouble( 0.0, 0.0 );
            
            for( int i=0; i<N_conf; i++){
                mean     += eff_mass[ nt(i,t) ];
                sqr_mean += cmp_sqr( eff_mass[ nt(i,t) ] );
            }
            mean     /= double(N_conf);
            sqr_mean /= double(N_conf);
            
            err = cmp_sqrt( ( sqr_mean - cmp_sqr( mean ) ) * double(N_conf-1) );
            
            outFILE << t << " " << mean.real() << " " << err.real()
            << " " << mean.imag() << " " << err.imag() << endl;
        }
        outFILE.close();
        delete [] eff_mass;
    }else{
        error(1,"Have not setted correlator yet !");
    }
    route( class_name, func_name, 0 );
}
