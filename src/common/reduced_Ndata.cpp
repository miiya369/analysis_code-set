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

int analysis::reduced_Ndata()
{   
   int ret;
   
   if (analysis::xSIZE == 16) ret = 165;
   if (analysis::xSIZE == 32) ret = 969;
   if (analysis::xSIZE == 48) ret = 2925;
   if (analysis::xSIZE == 64) ret = 6545;
   if (analysis::xSIZE == 80) ret = 12341;
   if (analysis::xSIZE == 96) ret = 20825;
   
   return ret;
}
