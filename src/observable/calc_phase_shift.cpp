//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup phase shift
 * @brief   Function for calculate phase shift
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <phase_shift.h>

void PHASE_SHIFT::calc_phase_shift( int E_MIN, int E_MAX ){
    
    func_name = "calc_phase_shift______";
    route( class_name, func_name, 1 );
    
    if( new_flg_phase_shift ) error(1,"Phase shift has already calculated !");
    else if( !new_flg_param ) error(1,"Fit parameter has not been inputed yet !");
    else if( !env_flg  ) error(1,"Calculate environment has not been set yet !");
    else{
        E_max = E_MAX;
        E_min = E_MIN;
        phase_shift = new double[ N_conf * (E_max-E_min+1) ];
        new_flg_phase_shift = true;
        
        double phi[3], phi_diff=0, tmp1, tmp2;
        double max_r_iter = r_max / del;
        double Coe, r, k;
        
        printf(" @ phase shift calculating |   0%%");
        for( int E=E_min; E<=E_max; E++ ){
            k = sqrt( 2 * mass * E );
            
            for( int conf=0; conf<N_conf; conf++ ){
                phi[0]  = 0.0;
                phi[1]  = del;   // <- Initialize of wave function
                r       = del;
                
                for( int iter=0; iter<max_r_iter; iter++ ){
                    r   += del;
                    Coe = (2*mass)/pow(hbar_c,2)*(E-V(r,conf))-ll*(ll+1)/pow(r,2);
                    phi[2]   = (2-Coe*pow(del,2))*phi[1]-phi[0];
                    phi_diff = (phi[2]-phi[0])/(2.0*del);
                    phi[0]   = phi[1];
                    phi[1]   = phi[2];
                    
//                    tmp1 = atan((k*phi[0])/(hbar_c*phi_diff)) //
//                                -k*r/hbar_c+ll*PI/2.0;        //
//                    tmp2 = fmod( tmp1, PI );                  //
//                    if(      tmp2 >=  PI/2.0 ) tmp2 -= PI;    //  For Denug !
//                    else if( tmp2 <= -PI/2.0 ) tmp2 += PI;    //
//                    printf("%lf %lf\n",r, tmp2* 180.0 / PI);  //
                }
                tmp1 = atan((k*phi[0])/(hbar_c*phi_diff))-k*r/hbar_c+ll*PI/2.0;
                tmp2 = fmod( tmp1, PI );
                if(      tmp2 >=  PI/2.0 ) tmp2 -= PI;
                else if( tmp2 <= -PI/2.0 ) tmp2 += PI;
                phase_shift[ idx(conf,E-E_min) ] = tmp2 * 180.0 / PI;
            }
            printf("\b\b\b\b%3.0lf%%",double(E-E_min)/double(E_max-E_min)*100);
            fflush(stdout);
        }
        printf("\n");
    }
    route( class_name, func_name, 0 );
}
