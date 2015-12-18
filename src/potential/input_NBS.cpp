//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBSwave
 * @brief   Function for Input NBS wave function files
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <NBSwave.h>

void NBS_WAVE::input_NBS(){
    
    func_name = "input_NBS_____________";
    route( class_name, func_name, 1 );
    
    orgNBS =  new cdouble[ orgNBS_size * N_conf ];
    new_flg_orgNBS = true;
    
    snprintf(data_list[N_TIME],sizeof(data_list[N_TIME]), "%d", time_slice);
    if( channel != 0 && channel < BOUNDARY_PARTICLE_ISOSPIN ){
        snprintf(data_list[NBS_DIRECTORY],sizeof(data_list[NBS_DIRECTORY])
                 ,"BBwave.dir");
        snprintf(data_list[N_CHANNEL],sizeof(data_list[N_CHANNEL])
                 ,"%d",channel-1);
    }else if( channel > BOUNDARY_PARTICLE_ISOSPIN ){
        snprintf(data_list[NBS_DIRECTORY],sizeof(data_list[NBS_DIRECTORY])
                 ,"BBwave.prj");
        snprintf(data_list[N_CHANNEL],sizeof(data_list[N_CHANNEL])
                 ,"%d",channel-BOUNDARY_PARTICLE_ISOSPIN-1);
    }
    
    time_t start_t, end_t;
    time( &start_t );
    printf(" @ NBSwave reading         |   0%%");
    for( int conf = 0; conf < N_conf; conf++ ){   // Read NBS wave files
            
        string NBSfile_name = set_path( data_list, conf );
        ifstream NBSfile( NBSfile_name.c_str(), ios::in | ios::binary );
        if( !NBSfile ) error(2, NBSfile_name.c_str());
        
        for( int file_loop = 0; file_loop < orgNBS_size; file_loop++ )
            NBSfile.read( (char*)&orgNBS[ file_loop + orgNBS_size* conf ]
                         , sizeof(cdouble) );
        NBSfile.close();
        
        printf("\b\b\b\b%3.0lf%%",double(conf+1)/double(N_conf)*100);
        fflush(stdout);
    } // conf
    time( &end_t );
    printf("  ( %d second )\n",(int)difftime(end_t,start_t));
    
    if( endian_flg ) endian_convert( orgNBS, orgNBS_size * N_conf );
    
    NBSwave = new cdouble[ NBS_size * N_conf ];
    new_flg_NBSwave = true;
    route( class_name, func_name, 0 );
}
