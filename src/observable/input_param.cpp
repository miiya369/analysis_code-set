//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup phase shift
 * @brief   Function for input the ( miyamoto-format ) fit parameter data
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <phase_shift.h>

void PHASE_SHIFT::input_param( const char *INFILE_NAME, bool endian_flg ){
    
    func_name = "input_param___________";
    route( class_name, func_name, 1 );
    
    if( new_flg_param ) error(1,"Fit parameter has already inputed !");
    else{
        
        printf(" @ parameter data reading |   0%%");
        ifstream infile( INFILE_NAME, ios::in | ios::binary );
        if( !infile ) error(2, INFILE_NAME);
        
        infile.read( (char*)&N_conf, sizeof(int) );
        infile.read( (char*)&N_param, sizeof(int) );   // read file header
        infile.read( (char*)&func_type, sizeof(int) );
        
        param = new double[ N_conf * N_param ];
        new_flg_param = true;

        for( int loop=0; loop<N_param; loop++ ){
            for( int conf=0; conf<N_conf; conf++ )
                infile.read( (char*)&param[ idx(conf,loop) ],sizeof(double) );
            printf("\b\b\b\b%3.0lf%%",double(loop+1)/double(N_param)*100);
            fflush(stdout);
        }
        printf("\n @ function type = %s\n", N_func_to_name(func_type).c_str());
        infile.close();
        
    }
    route( class_name, func_name, 0 );
}
