//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Print the function with gnuplot-format
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <fitting.h>

void FITTING::print_func_gnu( bool ini_flg, double lat_a ){
    
    func_name = "print_func_gnu________";
    route( class_name, func_name, 1 );
    
    if( !new_flg_param ){ error(1,"Fit parameter has not setted yet !"); }
    else{
        if( ini_flg ){
            if( func_type == CONSTANT ){
                printf(" @ For gnuplot function ( Initial parameter )\n");
                printf(" @ %lf\n", param_ini[0]);
            
            }else if( func_type == ONE_EXPONENTIAL ){
                printf(" @ For gnuplot function ( Initial parameter )\n");
                printf(" @ %lf*exp(-(%lf)*x)\n"
                       , param_ini[0], param_ini[1]);
            
            }else if( func_type == TWO_GAUSSIAN ){
                printf(" @ For gnuplot function ( Initial parameter )\n");
                printf(" @ [Lat Unit] %lf*exp(-(x/(%lf))**2)+(%lf)*exp(-(x/(%lf))**2)\n"
                       , param_ini[0], param_ini[1], param_ini[2], param_ini[3]);
                printf(" @ [Mass Dim] %lf*exp(-(x/(%lf))**2)+(%lf)*exp(-(x/(%lf))**2)\n"
                       , param_ini[0]*hbar_c/lat_a, param_ini[1]*lat_a
                       , param_ini[2]*hbar_c/lat_a, param_ini[3]*lat_a);
            
            }else if( func_type == THREE_GAUSSIAN ){
                printf(" @ For gnuplot function ( Initial parameter )\n");
                printf(" @ [Lat Unit] %lf*exp(-(x/(%lf))**2)+(%lf)*exp(-(x/(%lf))**2)+(%lf)*exp(-(x/(%lf))**2)\n"
                       , param_ini[0], param_ini[1], param_ini[2], param_ini[3], param_ini[4], param_ini[5]);
                printf(" @ [Mass Dim] %lf*exp(-(x/(%lf))**2)+(%lf)*exp(-(x/(%lf))**2)+(%lf)*exp(-(x/(%lf))**2)\n"
                       , param_ini[0]*hbar_c/lat_a, param_ini[1]*lat_a, param_ini[2]*lat_a
                       , param_ini[3]*hbar_c/lat_a, param_ini[4]*lat_a, param_ini[5]*lat_a);
                
            }else if( func_type == TWO_SHIFTED_GAUSSIAN ){
                printf(" @ For gnuplot function ( Initial parameter )\n");
                printf(" @ [Lat Unit] %lf*exp(-((x-(%lf))/(%lf))**2)+(%lf)*exp(-((x-(%lf))/(%lf))**2)\n"
                       , param_ini[0], param_ini[1], param_ini[2], param_ini[3], param_ini[4], param_ini[5]);
                printf(" @ [Mass Dim] %lf*exp(-((x-(%lf))/(%lf))**2)+(%lf)*exp(-((x-(%lf))/(%lf))**2)\n"
                       , param_ini[0]*hbar_c/lat_a, param_ini[1]*lat_a, param_ini[2]*hbar_c/lat_a
                       , param_ini[3]*lat_a, param_ini[4]*hbar_c/lat_a, param_ini[5]*lat_a);
            }
        }else{
            if( func_type == CONSTANT ){
                printf(" @ For gnuplot function ( Fitted parameter )\n");
                printf(" @ %lf\n", param_mean[0]);
            
            }else if( func_type == ONE_EXPONENTIAL ){
                printf(" @ For gnuplot function ( Fitted parameter )\n");
                printf(" @ %lf*exp(-(%lf)*x)\n"
                       , param_mean[0], param_mean[1]);
            
            }else if( func_type == TWO_GAUSSIAN ){
                printf(" @ For gnuplot function ( Fitted parameter )\n");
                printf(" @ [Lat Unit] %lf*exp(-(x/(%lf))**2)+(%lf)*exp(-(x/(%lf))**2)\n"
                       , param_mean[0], param_mean[1], param_mean[2], param_mean[3]);
                printf(" @ [Mass Dim] %lf*exp(-(x/(%lf))**2)+(%lf)*exp(-(x/(%lf))**2)\n"
                       , param_mean[0]*hbar_c/lat_a, param_mean[1]*lat_a
                       , param_mean[2]*hbar_c/lat_a, param_mean[3]*lat_a);
            
            }else if( func_type == THREE_GAUSSIAN ){
                printf(" @ For gnuplot function ( Fitted parameter )\n");
                printf(" @ [Lat Unit] %lf*exp(-(x/(%lf))**2)+(%lf)*exp(-(x/(%lf))**2)+(%lf)*exp(-(x/(%lf))**2)\n"
                       , param_mean[0], param_mean[1], param_mean[2]
                       , param_mean[3], param_mean[4], param_mean[5]);
                printf(" @ [Mass Dim] %lf*exp(-(x/(%lf))**2)+(%lf)*exp(-(x/(%lf))**2)+(%lf)*exp(-(x/(%lf))**2)\n"
                       , param_mean[0]*hbar_c/lat_a, param_mean[1]*lat_a, param_mean[2]*hbar_c/lat_a
                       , param_mean[3]*lat_a, param_mean[4]*hbar_c/lat_a, param_mean[5]*lat_a);
                
            }else if( func_type == TWO_SHIFTED_GAUSSIAN ){
                printf(" @ For gnuplot function ( Fitted parameter )\n");
                printf(" @ [Lat Unit] %lf*exp(-((x-(%lf))/(%lf))**2)+(%lf)*exp(-((x-(%lf))/(%lf))**2)\n"
                       , param_mean[0], param_mean[1], param_mean[2]
                       , param_mean[3], param_mean[4], param_mean[5]);
                printf(" @ [Mass Dim] %lf*exp(-((x-(%lf))/(%lf))**2)+(%lf)*exp(-((x-(%lf))/(%lf))**2)\n"
                       , param_mean[0]*hbar_c/lat_a, param_mean[1]*lat_a, param_mean[2]*lat_a
                       , param_mean[3]*hbar_c/lat_a, param_mean[4]*lat_a, param_mean[5]*lat_a);
            }
        }
    }
    route( class_name, func_name, 0 );
}
