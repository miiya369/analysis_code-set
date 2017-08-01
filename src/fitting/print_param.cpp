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
   DEBUG_LOG
   
   if (param == NULL) {
      WORNING_COMMENTS("Fit function has not set yet !");
      return;
   }
   printf(" @ parameter infomations.\n @");
   
   for (int loop=0; loop<func_type.Nparam; loop++)
      if (param[loop] > 0.0001)
         printf(" %lf",param[loop]);
      else
         printf(" %e",param[loop]);
   printf("\n");
}
