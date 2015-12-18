//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Function for setting the fit function
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <fitting.h>

void FITTING::set_func( int FUNC_TYPE, double *PARAM ){
    
    func_name = "set_func______________";
    route( class_name, func_name, 1 );
    
    if( new_flg_param ) error(1,"Fit function has already setted !");
    else{
        func_type = FUNC_TYPE;
        
        if( func_type == CONSTANT ){
            N_param = 1;
            printf(" @ fit function = constant\n");
            
        }else if( func_type == ONE_EXPONENTIAL ){
            N_param = 2;
            printf(" @ fit function = one exponential\n");
            
        }else if( func_type == TWO_GAUSSIAN ){
            N_param = 4;
            printf(" @ fit function = two gaussians\n");
            
        }else if( func_type == THREE_GAUSSIAN ){
            N_param = 6;
            printf(" @ fit function = two x-shifted gaussians\n");
            
        }else if( func_type == TWO_SHIFTED_GAUSSIAN ){
            N_param = 6;
            printf(" @ fit function = two x-shifted gaussians\n");
        }
        
        param_ini   = new double[N_param];
        param_out   = new double[N_param * N_conf];
        param_err   = new double[N_param];
        param_mean  = new double[N_param];
        new_flg_param = true;
        for( int loop=0; loop<N_param; loop++ )
            param_ini[loop] = PARAM[loop];
    }
    
    route( class_name, func_name, 0 );
}
