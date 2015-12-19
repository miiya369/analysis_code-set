//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Correlator
 * @brief   Function for Input correlator files
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <correlator.h>

void CORRELATOR::input_corr(){
    
    func_name = "input_corr____________";
    route( class_name, func_name, 1 );
    
    corr = new cdouble[ tSIZE * N_conf ];
    
    int dummy_t;
    double tmp_r, tmp_i;
    
    snprintf(data_list[N_TIME],sizeof(data_list[N_TIME]), "-1");
    snprintf(data_list[HADRON_NAME],sizeof(data_list[HADRON_NAME])
             ,"%s",HAD_name.c_str());
    snprintf(data_list[CORR_DIRECTORY],sizeof(data_list[CORR_DIRECTORY]),"PS");
    
    time_t start_t, end_t;
    time( &start_t );
    printf(" @ correlator reading      |   0%%");
    for( int conf = 0; conf < N_conf; conf++ ){  // Read correlator files
        
        string corrfile_name = set_path( data_list, conf );
        ifstream corrfile( corrfile_name.c_str(), ios::in );
        if( !corrfile ) error(2, corrfile_name.c_str());
        
        for( int k = 0; k < tSIZE; k++ ){
            corrfile >> dummy_t >> tmp_r >> tmp_i;
            corrfile.ignore(10000, '\n');
            corr[ nt( conf, dummy_t ) ] = cdouble( tmp_r, tmp_i );
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
        printf(" @ anti-correlator reading |   0%%");
        for( int conf = 0; conf < N_conf; conf++ ){ // Read anti-correlator files
            
            string anti_corrfile_name = set_path( data_list, conf );
            ifstream anti_corrfile( anti_corrfile_name.c_str(), ios::in );
            if( !anti_corrfile ) error(2, anti_corrfile_name.c_str());
            
            anti_corrfile >> dummy_t >> tmp_r >> tmp_i;
            anti_corrfile.ignore(10000, '\n');
            corr[ nt( conf, 0 ) ] = ( corr[ nt( conf, 0 ) ]
                                     + cdouble( tmp_r, tmp_i ) ) * 0.5;
            for( int k = 1; k < tSIZE; k++ ){
                anti_corrfile >> dummy_t >> tmp_r >> tmp_i;
                anti_corrfile.ignore(10000, '\n');
                corr[ nt( conf, tSIZE - dummy_t ) ] =
                ( corr[ nt( conf, tSIZE - dummy_t ) ] + cdouble( tmp_r, tmp_i ) )
                * 0.5;
            }
            anti_corrfile.close();
            printf("\b\b\b\b%3.0lf%%",double(conf+1)/double(N_conf)*100);
            fflush(stdout);
        } // conf
        time( &end_t );
        printf("  ( %d second )\n",(int)difftime(end_t,start_t));
    }
    printf(" @ Finished input correlator       : %s\n", HAD_name.c_str());
    route( class_name, func_name, 0 );
}
