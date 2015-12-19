//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Common header file for make jack-knife samples
 * @author  Takaya Miyamoto
 * @since   Fri Dec 11 02:09:30 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef MAKE_JK_SAMPLE_H
#define MAKE_JK_SAMPLE_H

#include <common/analysis.h>

template <class X> void CONFIG<X>::make_JK_sample() {
   
   func_name = "make_JK_sample________";
   analysis::route(class_name, func_name, 1);
   
   int data_size = data[0].data_size();
   
#ifdef _OPENMP
#pragma omp parallel for
#endif
   for (int n=0; n<data_size; n++) {
      cdouble sum;
      
      for (int i=0; i<num_conf; i++)
         sum += data[i](n);
      
      for (int i=0; i<num_conf; i++)
         data[i](n) = ( sum - data[i](n) ) / double(num_conf-1);
   }
   analysis::route(class_name, func_name, 0);
}

template <class X> void CONFIG<X>::make_JK_sample( CONFIG<X>& data_bin ) {
   
   func_name = "make_JK_sample_bin____";
   analysis::route(class_name, func_name, 1);
   
   int data_size = data[0].data_size();
   int num_bin   = data_bin.Nconf();
   
   for (int i=0; i<num_bin; i++) data_bin(i).mem_alloc( data_size );
   
   if (num_conf % num_bin != 0)
      analysis::error(3, "#.conf is indivisible by #.bin.");
   if (num_bin == 1)
      analysis::error(3, "#.bin = 1 is not allowed.");
   
   int bin_size = num_conf / num_bin;
   
#ifdef _OPENMP
#pragma omp parallel for
#endif
   for (int n=0; n<data_size; n++) {
      cdouble sum;
      
      for (int i=0; i<num_conf; i++)
         sum += data[i](n);
      
      for (   int i=0; i<num_bin;  i++) {
         cdouble sum_bin;
         for (int b=0; b<bin_size; b++)
            sum_bin += data[b + bin_size*i](n);
         
         data_bin(i)(n) = ( sum - sum_bin ) / double(num_conf-bin_size);
      }
   }
   analysis::route(class_name, func_name, 0);
}

template <class X> void CONFIG<X>::make_mean_err(  double *mean, double *err
                                                 , bool is_jack_knife_data ) {
   
   func_name = "make_mean_err_________";
   analysis::route(class_name, func_name, 1);
   
   int     data_size = data[0].data_size();
   double  factor;
   double  sqr_mean;
   
   if (is_jack_knife_data)
      factor = double(num_conf-1);
   else
      factor = 1.0 / double(num_conf-1);

   for (int n=0; n<data_size; n++) {
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
   
   int     data_size = data[0].data_size();
   double  factor;
   cdouble sqr_mean;
   
   if (is_jack_knife_data)
      factor = double(num_conf-1);
   else
      factor = 1.0 / double(num_conf-1);
   
   for (int n=0; n<data_size; n++) {
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
