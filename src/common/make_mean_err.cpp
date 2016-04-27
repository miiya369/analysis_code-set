//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Function for make mean and error of configuration data
 * @author  Takaya Miyamoto
 * @since   Wed Apr 27 01:19:22 JST 2016
 */
//--------------------------------------------------------------------------

#include <common/analysis.h>

void analysis::make_mean_err(  const double *data, double &mean, double &err
                             , const int Ndata, const bool is_jack_knife_data )
{
   double factor;
   
   if (is_jack_knife_data) factor = double(Ndata-1);
   else                    factor = 1.0 / double(Ndata-1);
   
   mean            = 0.0;
   double sqr_mean = 0.0;
   
   for (int i=0; i<Ndata; i++)
   {
      mean     +=     data[i];
      sqr_mean += pow(data[i], 2);
   }
   mean     /= double(Ndata);
   sqr_mean /= double(Ndata);
   err       = sqrt(factor * (sqr_mean - pow(mean, 2)));
}

void analysis::make_mean_err(  const cdouble *data, cdouble &mean, cdouble &err
                             , const int Ndata, const bool is_jack_knife_data )
{
   double factor;
   
   if (is_jack_knife_data) factor = double(Ndata-1);
   else                    factor = 1.0 / double(Ndata-1);
   
   mean             = COMP_ZERO;
   cdouble sqr_mean = COMP_ZERO;
   
   for (int i=0; i<Ndata; i++)
   {
      mean     +=         data[i];
      sqr_mean += cmp_sqr(data[i]);
   }
   mean     /= double(Ndata);
   sqr_mean /= double(Ndata);
   err       = cmp_sqrt(factor * (sqr_mean - cmp_sqr(mean)));
}
