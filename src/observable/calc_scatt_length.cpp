//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup phase shift
 * @brief   Function for calculate scattering length
 * @author  Takaya Miyamoto
 * @since   Tue Jul  5 04:55:58 JST 2016
 */
//--------------------------------------------------------------------------

#include <observable/phase_shift.h>

cdouble observable::calc_scatt_length(  double *param, int Nparam, int func_type
                                      , double energy, double lat_space
                                      , double mm, double del, double r_max ) {
   
   PHASE_SHIFT phase;
   phase.mem_alloc( energy, energy, 1 );
   double tmp = 0.0;
   
   observable::calc_phase_shift_dif(  &phase, param, &mm, &tmp, 1
                                    , &Nparam, &func_type
                                    , lat_space, del, r_max );
   
   return -tan(phase(0)*PI/(double)180)*197.327053/sqrt(2*energy*mm);
}
