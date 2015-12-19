//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBS wave function
 * @brief   Make jack-knife samples
 * @author  Takaya Miyamoto
 * @since   Tue Jul 28 23:41:23 JST 2015
 */
//--------------------------------------------------------------------------

#include <NBSwave/NBSwave.h>

//--------------------------------------------------------------------------
/**
 * @brief Make Jack-Knife sample for spin projected NBS wave func.
 */
//--------------------------------------------------------------------------
void NBS_WAVE::make_JK_sample_NBS( int bin ) {
   
   func_name = "make_JK_sample_NBS____";
   analysis::route(class_name, func_name, 1);
   
   if (wave == NULL) {
      analysis::error(1, "NBS wave function has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   
#ifdef _OPENMP
#pragma omp parallel for
#endif
   for (size_t loop=0; loop<NBSwave::xyzSIZE; loop++) {
      cdouble sum;
      
      for (int i=0; i<analysis::Nconf; i++)
         sum += wave[idx(loop,0,0,i)];
      
      for (int i=0; i<analysis::Nconf; i++)
         wave[idx(loop,0,0,i)]
            = ( sum - wave[idx(loop,0,0,i)] ) / double(analysis::Nconf-1);;
   }
   
   analysis::route(class_name, func_name, 0);
}
