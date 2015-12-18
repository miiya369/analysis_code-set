//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup scattering length
 * @brief   Function for calculate scattering length
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <observable.h>

void OBSERVABLE::calc_scatt_len(){

    func_name = "calc_scatt_len________";
    route( class_name, func_name, 1 );
    
    if( new_flg_scatt_length )
        error(1,"Scattering length has already calculated !");
    else{
        if( new_flg_phase_shift ){
            scatt_length = new double[ N_conf * (E_max-E_min+1) ];
            new_flg_scatt_length = true;
            
            for( int iE=E_min; iE<=E_max; iE++ )
                for( int i=0; i<N_conf; i++)
                    scatt_length[idx(i,iE-E_min)]
                     = -tan( phase_shift[idx(i,iE-E_min)]*PI/180.0 )
                        * hbar_c / sqrt( 2.0* mass* iE);
        }
        else error(1,"Phase shift has not been calculated yet !");
    }
    
    route( class_name, func_name, 0 );
}
