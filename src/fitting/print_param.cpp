//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Print the current parameters
 * @author  Takaya Miyamoto
 * @since   Wed Jul 22 09:16:44 JST 2015
 */
//--------------------------------------------------------------------------

#include <fitting/fitting.h>

void FIT::print_param() {
   
   func_name = "print_param___________";
   analysis::route(class_name, func_name, 1);
   
   if (!fitting_flg) {
      analysis::error(1,"Data fitting has not done yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   printf(" @ Fitted parameter infomations.\n @");
   
   for (int loop=0; loop<func_type.Nparam; loop++)
      printf(" %lf",param_mean[loop]);
   printf("\n");
   
   for (int loop=0; loop<func_type.Nparam; loop++)
      printf(  " @ param[%d] = %lf +/- %lf\n"
             , loop, param_mean[loop], param_err[loop]);
   
   analysis::route(class_name, func_name, 0);
}
