//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Common header file for make jack-knife samples
 * @author  Takaya Miyamoto
 * @since   Mon Nov  7 16:14:42 JST 2016
 */
//--------------------------------------------------------------------------

#ifndef MAKE_JK_SAMPLE_H
#define MAKE_JK_SAMPLE_H

#include <common/config_tmp.h>

template <class X> void CONFIG<X>::make_JK_sample() {
   DEBUG_LOG
   
#ifdef _OPENMP
#pragma omp parallel for
#endif
   for (int n=0; n<(*this)(0).data_size(); n++) {
      cdouble sum;
      
      for (int i=0; i<(*this).Nconf(); i++)
         sum += (*this)(i)(n);
      for (int i=0; i<(*this).Nconf(); i++)
         (*this)(i)(n) = (sum - (*this)(i)(n)) / double((*this).Nconf()-1);
   }
}

template <class X> void CONFIG<X>::make_JK_sample(CONFIG<X>& BinData) {
   DEBUG_LOG
   
   int Nbin = BinData.Nconf();
   
   for (int i=0; i<Nbin; i++) BinData(i).mem_alloc((*this)(0).data_size());
   
   if ((*this).Nconf() % Nbin != 0)
      ERROR_COMMENTS("#.conf is indivisible by #.bin.");
   if (Nbin == 1) ERROR_COMMENTS("#.bin = 1 is not allowed.");
   
   int BinSize = (*this).Nconf() / Nbin;
   
#ifdef _OPENMP
#pragma omp parallel for
#endif
   for (int n=0; n<(*this)(0).data_size(); n++) {
      cdouble sum = COMP_ZERO;
      
      for (int i=0; i<(*this).Nconf(); i++)
         sum += (*this)(i)(n);
      
      for (int i=0; i<Nbin;  i++) {
         cdouble sum_bin = COMP_ZERO;
         for (int b=0; b<BinSize; b++)
            sum_bin += (*this)(b + BinSize*i)(n);
         
         BinData(i)(n) = (sum - sum_bin) / double((*this).Nconf()-BinSize);
      }
   }
}

template <class X> void CONFIG<X>::make_mean_err(  double *mean, double *err
                                                 , bool is_jack_knife_data ) {
   DEBUG_LOG
   
   double factor;
   double sqr_mean;
   
   if (is_jack_knife_data)
      factor = double((*this).Nconf()-1);
   else
      factor = 1.0 / double((*this).Nconf()-1);
   
   for (int n=0; n<(*this)(0).data_size(); n++) {
      mean[n]  = 0.0;
      sqr_mean = 0.0;
      
      for (int i=0; i<(*this).Nconf(); i++) {
         mean[n]  +=     (*this)(i)(n).real();
         sqr_mean += pow((*this)(i)(n).real(), 2);
      }
      mean[n]  /= double((*this).Nconf());
      sqr_mean /= double((*this).Nconf());
      err[n]    = sqrt(factor * (sqr_mean - pow(mean[n], 2)));
   }
}

template <class X> void CONFIG<X>::make_mean_err(  cdouble *mean, cdouble *err
                                                 , bool is_jack_knife_data ) {
   DEBUG_LOG
   
   double  factor;
   cdouble sqr_mean;
   
   if (is_jack_knife_data)
      factor = double((*this).Nconf()-1);
   else
      factor = 1.0 / double((*this).Nconf()-1);
   
   for (int n=0; n<(*this)(0).data_size(); n++) {
      mean[n]  = COMP_ZERO;
      sqr_mean = COMP_ZERO;
      
      for (int i=0; i<(*this).Nconf(); i++) {
         mean[n]  +=         (*this)(i)(n);
         sqr_mean += cmp_sqr((*this)(i)(n));
      }
      mean[n]  /= double((*this).Nconf());
      sqr_mean /= double((*this).Nconf());
      err[n]    = cmp_sqrt(factor * (sqr_mean - cmp_sqr(mean[n])));
   }
}

#endif
