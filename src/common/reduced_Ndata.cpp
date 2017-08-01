//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Definition of number of reduced data points
 * @author  Takaya Miyamoto
 * @since   Thu Sep  3 01:26:45 JST 2015
 */
//--------------------------------------------------------------------------

#include <common/analysis.h>

int analysis::reduced_Ndata() {
   int ret = 0;
   
   if      (analysis::xSIZE == 16) ret = 165;
   else if (analysis::xSIZE == 32) ret = 969;
   else if (analysis::xSIZE == 48) ret = 2925;
   else if (analysis::xSIZE == 64) ret = 6545;
   else if (analysis::xSIZE == 80) ret = 12341;
   else if (analysis::xSIZE == 96) ret = 20825;
   else {
      for (      int z=0; z<analysis::zSIZE; z++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int x=0; x<analysis::xSIZE; x++) {
               if (x>analysis::xSIZE/2 || y>x || z>y) continue;
               ret++;
            }
   }
   
   return ret;
}
