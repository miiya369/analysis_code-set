//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup scattering length
 * @brief   Function for input phase shift data
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <observable.h>

void OBSERVABLE::input_phase_shift( const char* in_file_name
                                     , int n_conf, int    E_MIN
                                     , int E_MAX,  double MASS){
    
    func_name = "input_phase_shift_____";
    route( class_name, func_name, 1 );
    
    if( new_flg_phase_shift ) error(1,"Phase shift has already calculated !");
    else{
        N_conf = n_conf;
        E_min  = E_MIN;
        E_max  = E_MAX;
        mass   = MASS;
        
        phase_shift = new double[ N_conf * (E_max-E_min+1) ];
        new_flg_phase_shift = true;
        ifstream infile( in_file_name, ios::in );
        if( !infile ) error(2, in_file_name);
        
        int Energy;
        for( int iE=E_min; iE<=E_max; iE++ )
            for( int i=0; i<N_conf; i++){
                infile >> Energy >> phase_shift[ idx(i,iE-E_min) ];
                if( Energy!=iE ) printf(" @@@@@@ WARNING @@@@@@\n");
                infile.ignore(10000, '\n');
            }
        infile.close();
    }
    
    route( class_name, func_name, 0 );
}
