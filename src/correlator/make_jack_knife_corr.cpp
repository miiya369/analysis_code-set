//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Correlator
 * @brief   Make jack-knife samples
 * @author  Takaya Miyamoto
 * @since   Mon Jul 20 12:13:15 JST 2015
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
   cdouble *tmp;   tmp = new cdouble[analysis::tSIZE*analysis::Nconf];
   
   for (int t=0; t<analysis::tSIZE; t++) {
      cdouble sum;
      
      for (int i=0; i<analysis::Nconf; i++)
         sum += corr[nt(i,t)];
      for (int i=0; i<analysis::Nconf; i++)
         tmp[nt(i,t)] = ( sum - corr[nt(i,t)] ) / double(analysis::Nconf-1);
   }
   for (size_t n=0; n<analysis::tSIZE*analysis::Nconf; n++) corr[n]=tmp[n];
   delete [] tmp;
   
   analysis::route(class_name, func_name, 0);
}
