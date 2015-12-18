//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup phase shift
 * @brief   Function for test calculation of phase shift
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <phase_shift.h>

void PHASE_SHIFT::set_test( double r_0, double V_0, double MASS, double DEL, double R_max ){
    
    func_name = "set_test______________";
    route( class_name, func_name, 1 );
    
    lattice_space   = 1;
    mass            = MASS;
    ll              = 0;
    del             = DEL;
    r_max           = R_max;
    env_flg         = true;
    new_flg_param   = true;
    N_conf          = 1;
    func_type       = TEST;
    test_r_0        = r_0;
    test_V_0        = V_0;
    
    route( class_name, func_name, 0 );
}
