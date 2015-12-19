//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBS wave function
 * @brief   Wrapper of data reduction list
 * @author  Takaya Miyamoto
 * @since   Wed Jul 22 03:19:20 JST 2015
 */
//--------------------------------------------------------------------------

#include <NBSwave/NBSwave.h>

size_t NBSwave::data_reduction( size_t idx_num ) {
   
   size_t ret = 0;
   
   if (analysis::xSIZE == 16) ret = data_reduction_16( idx_num );
   if (analysis::xSIZE == 32) ret = data_reduction_32( idx_num );
   
   return ret;
}
