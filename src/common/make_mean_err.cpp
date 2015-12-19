//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Function for make mean and error of configuration data
 * @author  Takaya Miyamoto
 * @since   Wed Sep  2 17:49:50 JST 2015
 */
//--------------------------------------------------------------------------

#include <common/analysis.h>

void analysis::make_mean_err( double *data, double &mean, double &err
                             , bool is_jack_knife_data ) {
   
   double  factor;
   
   if (is_jack_knife_data) {
      factor = double(analysis::Nconf-1);
   }
   else {
      factor = 1.0 / double(analysis::Nconf-1);
   }
   
   mean            = 0.0;
   double sqr_mean = 0.0;
   
   for (int i=0; i<analysis::Nconf; i++) {
      mean     +=      data[i];
      sqr_mean += pow( data[i], 2 );
   }
   mean     /= double(analysis::Nconf);
   sqr_mean /= double(analysis::Nconf);
   err       = sqrt( factor * ( sqr_mean - pow( mean, 2 )) );
}

void analysis::make_mean_err( cdouble *data, cdouble &mean, cdouble &err
                             , bool is_jack_knife_data ) {
   
   double  factor;
   
   if (is_jack_knife_data) {
      factor = double(analysis::Nconf-1);
   }
   else {
      factor = 1.0 / double(analysis::Nconf-1);
   }
   
   mean             = COMP_ZERO;
   cdouble sqr_mean = COMP_ZERO;
   
   for (int i=0; i<analysis::Nconf; i++) {
      mean     +=          data[i];
      sqr_mean += cmp_sqr( data[i] );
   }
   mean     /= double(analysis::Nconf);
   sqr_mean /= double(analysis::Nconf);
   err       = cmp_sqrt( factor * ( sqr_mean - cmp_sqr( mean )) );
}
