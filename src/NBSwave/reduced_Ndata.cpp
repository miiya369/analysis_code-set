//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBS wave function
 * @brief   Definition of number of reduced data points
 * @author  Takaya Miyamoto
 * @since   Wed Jul 29 01:18:47 JST 2015
 */
//--------------------------------------------------------------------------

#include <NBSwave/NBSwave.h>

int NBSwave::reduced_Ndata() {
   
   int ret;
   
   if (analysis::xSIZE == 16) ret = 165;
   if (analysis::xSIZE == 32) ret = 969;
   if (analysis::xSIZE == 48) ret = 2925;
   if (analysis::xSIZE == 64) ret = 6545;
   if (analysis::xSIZE == 80) ret = 12341;
   if (analysis::xSIZE == 96) ret = 20825;
   
   return ret;
}
