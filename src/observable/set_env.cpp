//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup phase shift
 * @brief   Function for setting calculate environment
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <phase_shift.h>

void PHASE_SHIFT::set_env( double LATTICE_SPACE, double MASS, double ang_mom, double DEL, double R_max ){
    
    func_name = "set_env_______________";
    route( class_name, func_name, 1 );
    
    lattice_space   = LATTICE_SPACE;
    mass            = MASS;
    ll              = ang_mom;
    del             = DEL;
    r_max           = R_max;
    env_flg         = true;
    
    route( class_name, func_name, 0 );
}
