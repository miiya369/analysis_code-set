//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Function for output parameter list file
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <fitting.h>

void FITTING::output_param( const char* outFILE_name ){
    
    func_name = "input_data____________";
    route( class_name, func_name, 1 );
    
    if( !fitting_flg ) error(1,"Have not fitted data yet !");
    else{
        ofstream outfile( outFILE_name, ios::out | ios::binary );
        
        outfile.write( (char*)&N_conf, sizeof(int) );
        outfile.write( (char*)&N_param, sizeof(int) );   // write file header
        outfile.write( (char*)&func_type, sizeof(int) );
        
        for( int loop=0; loop<N_param; loop++ ) for( int conf=0; conf<N_conf; conf++ )
            outfile.write( (char*)&param_out[ idx(conf,loop) ], sizeof(double) );
        
        outfile.close();
    }
    route( class_name, func_name, 0 );
}
