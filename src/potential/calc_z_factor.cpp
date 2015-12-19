//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Correlator
 * @brief   Function for Calculation Z-factor
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <correlator.h>

void CORRELATOR::calc_z_factor(){
    
    func_name = "calc_z_factor_________";
    route( class_name, func_name, 1 );
    
    if( corr != NULL ){
        if( Z_factor == NULL ) Z_factor = new cdouble[ tSIZE * N_conf ];
        else{
            error(1,"Z-factor has already calculated !");
            return;
        }
        cdouble *tmp_corr = new cdouble[ tSIZE * N_conf ];
        int dummy_t;
        double tmp_r, tmp_i;
        
        snprintf(data_list[N_TIME],sizeof(data_list[N_TIME]), "-1");
        snprintf(data_list[HADRON_NAME],sizeof(data_list[HADRON_NAME])
                 ,"%s",HAD_name.c_str());
        snprintf(data_list[CORR_DIRECTORY],sizeof(data_list[CORR_DIRECTORY]),"SS");
        
        time_t start_t, end_t;
        time( &start_t );
        printf(" @ correlator (SS) reading |   0%%");
        for( int conf = 0; conf < N_conf; conf++ ){  // Read correlator files
            
            string corrfile_name = set_path( data_list, conf );
            ifstream corrfile( corrfile_name.c_str(), ios::in );
            if( !corrfile ) error(2, corrfile_name.c_str());
            
            for( int k = 0; k < tSIZE; k++ ){
                corrfile >> dummy_t >> tmp_r >> tmp_i;
                corrfile.ignore(10000, '\n');
                tmp_corr[ nt( conf, dummy_t ) ] = cdouble( tmp_r, tmp_i );
            }
            corrfile.close();
            printf("\b\b\b\b%3.0lf%%",double(conf+1)/double(N_conf)*100);
            fflush(stdout);
        } // conf
        time( &end_t );
        printf("  ( %d second )\n",(int)difftime(end_t,start_t));
        
        if( fb_mean_flg ){
            
            string anti_HAD_name;   anti_HAD_name = "anti" + HAD_name;
            snprintf(data_list[HADRON_NAME],sizeof(data_list[HADRON_NAME])
                     ,"%s",anti_HAD_name.c_str());
            
            time_t start_t, end_t;
            time( &start_t );
            printf(" @ anti-corr (SS)  reading |   0%%");
            for( int conf = 0; conf < N_conf; conf++ ){ // Read anti-correlator files
                
                string anti_corrfile_name = set_path( data_list, conf );
                ifstream anti_corrfile( anti_corrfile_name.c_str(), ios::in );
                if( !anti_corrfile ) error(2, anti_corrfile_name.c_str());
                
                anti_corrfile >> dummy_t >> tmp_r >> tmp_i;
                anti_corrfile.ignore(10000, '\n');
                tmp_corr[ nt( conf, 0 ) ] = ( tmp_corr[ nt( conf, 0 ) ]
                                         + cdouble( tmp_r, tmp_i ) ) * 0.5;
                for( int k = 1; k < tSIZE; k++ ){
                    anti_corrfile >> dummy_t >> tmp_r >> tmp_i;
                    anti_corrfile.ignore(10000, '\n');
                    tmp_corr[ nt( conf, tSIZE - dummy_t ) ] =
                    ( tmp_corr[ nt( conf, tSIZE - dummy_t ) ]
                     + cdouble( tmp_r, tmp_i ) ) * 0.5;
                }
                anti_corrfile.close();
                printf("\b\b\b\b%3.0lf%%",double(conf+1)/double(N_conf)*100);
                fflush(stdout);
            } // conf
            time( &end_t );
            printf("  ( %d second )\n",(int)difftime(end_t,start_t));
        }
        
        for( int conf = 0; conf < N_conf; conf++ )
            for( int it = 0; it < tSIZE; it++ )
                Z_factor[ nt(conf,it) ] = corr[ nt(conf,it) ]
                                         / tmp_corr[ nt(conf,it) ];
        delete [] tmp_corr;
        
    }else error(1,"correlator has not been inputed yet !");
    route( class_name, func_name, 0 );
}
