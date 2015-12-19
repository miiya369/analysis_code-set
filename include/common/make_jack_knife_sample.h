//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Common header file for make jack-knife samples
 * @author  Takaya Miyamoto
 * @since   Wed Sep 16 21:47:42 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef MAKE_JK_SAMPLE_H
#define MAKE_JK_SAMPLE_H

#include <common/analysis.h>

template <class X> void CONFIG<X>::make_JK_sample() {
   
   func_name = "make_JK_sample________";
   analysis::route(class_name, func_name, 1);
   
   size_t data_size = data[0].info_data_size();
   
#ifdef _OPENMP
#pragma omp parallel for
#endif
   for (size_t n=0; n<data_size; n++) {
      cdouble sum;
      
      for (int i=0; i<num_conf; i++)
         sum += data[i](n);
      
      for (int i=0; i<num_conf; i++)
         data[i](n) = ( sum - data[i](n) ) / double(num_conf-1);
   }
   analysis::route(class_name, func_name, 0);
}

template <class X> void CONFIG<X>::make_mean_err(  double *mean, double *err
                                                 , bool is_jack_knife_data ) {
   
   func_name = "make_mean_err_________";
   analysis::route(class_name, func_name, 1);
   
   size_t  data_size = data[0].info_data_size();
   double  factor;
   double sqr_mean;
   
   if (is_jack_knife_data) {
      factor = double(num_conf-1);
   }
   else {
      factor = 1.0 / double(num_conf-1);
   }

   for (size_t n=0; n<data_size; n++) {
      mean[n]  = 0.0;
      sqr_mean = 0.0;
      
      for (int i=0; i<num_conf; i++) {
         mean[n]  +=      data[i](n).real();
         sqr_mean += pow( data[i](n).real(), 2 );
      }
      mean[n]  /= double(num_conf);
      sqr_mean /= double(num_conf);
      err[n]    = sqrt( factor * ( sqr_mean - pow( mean[n], 2 )) );
   }
   analysis::route(class_name, func_name, 0);
}

template <class X> void CONFIG<X>::make_mean_err(  cdouble *mean, cdouble *err
                                                 , bool is_jack_knife_data ) {
   
   func_name = "make_mean_err_________";
   analysis::route(class_name, func_name, 1);
   
   size_t  data_size = data[0].info_data_size();
   double  factor;
   cdouble sqr_mean;
   
   if (is_jack_knife_data) {
      factor = double(num_conf-1);
   }
   else {
      factor = 1.0 / double(num_conf-1);
   }
   
   for (size_t n=0; n<data_size; n++) {
      mean[n]  = COMP_ZERO;
      sqr_mean = COMP_ZERO;
      
      for (int i=0; i<num_conf; i++) {
         mean[n]  +=          data[i](n);
         sqr_mean += cmp_sqr( data[i](n) );
      }
      mean[n]  /= double(num_conf);
      sqr_mean /= double(num_conf);
      err[n]    = cmp_sqrt( factor * ( sqr_mean - cmp_sqr( mean[n] )) );
   }
   analysis::route(class_name, func_name, 0);
}

#endif
