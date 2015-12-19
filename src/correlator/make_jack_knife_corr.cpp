//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Correlator
 * @brief   Make jack-knife samples
 * @author  Takaya Miyamoto
 * @since   Tue Jul 28 23:09:31 JST 2015
 */
//--------------------------------------------------------------------------

#include <correlator/correlator.h>

void CORRELATOR::make_JK_sample_corr( int bin ) {
   
   func_name = "make_JK_sample_corr___";
   analysis::route(class_name, func_name, 1);
   
   if (corr == NULL) {
      analysis::error(1, "Correlator has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   
   for (int t=0; t<analysis::tSIZE; t++) {
      cdouble sum;
      
      for (int i=0; i<analysis::Nconf; i++)
         sum += corr[nt(i,t)];
      
      for (int i=0; i<analysis::Nconf; i++)
         corr[nt(i,t)] = ( sum - corr[nt(i,t)] ) / double(analysis::Nconf-1);
   }
   
   analysis::route(class_name, func_name, 0);
}
