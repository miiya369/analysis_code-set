//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Print the current parameters
 * @author  Takaya Miyamoto
 * @since   Thu Sep  3 03:07:46 JST 2015
 */
//--------------------------------------------------------------------------

#include <fitting/fitting.h>

void FIT::print_param() {
   
   func_name = "print_param___________";
   analysis::route(class_name, func_name, 1);
   
   if (param == NULL) {
      analysis::error(1,"Fit function has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   printf(" @ parameter infomations.\n @");
   
   for (int loop=0; loop<func_type.Nparam; loop++)
      printf(" %lf",param[loop]);
   printf("\n");
   
   analysis::route(class_name, func_name, 0);
}
