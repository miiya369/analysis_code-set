//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Print the current parameters
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <fitting.h>

void FITTING::print_param(){
    
    func_name = "print_param___________";
    route( class_name, func_name, 1 );
    
    if( !fitting_flg )   error(1,"Have not fitted data yet !");
    else{
        printf(" @ Fitted parameter infomations.\n @");
        for( int loop=0; loop<N_param; loop++ )
            printf(" %lf",param_mean[loop]);
        printf("\n");
        for( int loop=0; loop<N_param; loop++ )
            printf(" @ a[%d] = %lf +/- %lf\n", loop, param_mean[loop], param_err[loop]);
    }
    
    route( class_name, func_name, 0 );
}
