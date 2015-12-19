//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Definition for fit functions, For Numerical Recipe notation
 * @author  Takaya Miyamoto
 * @since   Wed Jul 22 21:02:33 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef FITTING_FROM_NR_H
#define FITTING_FROM_NR_H

#include <Numerical_Recipe/nr3.h>
#include <Numerical_Recipe/gaussj.h>
#include <Numerical_Recipe/fitmrq.h>

namespace fit_funcs {
   
   void func_const(const Doub x, VecDoub_I &a, Doub &y, VecDoub_O &dyda) {
      y=a[0];
      dyda[0]=1;
   }
   
   void func_exp(const Doub x, VecDoub_I &a, Doub &y, VecDoub_O &dyda) {
      Int na=a.size();
      Doub ex;
      y=0.;
      for (int i=0; i<na-1; i+=2) {
         ex=exp(-a[i+1]*x);
         y += a[i]*ex;
         dyda[i]=ex;
         dyda[i+1]=-x*a[i]*ex;
      }
   }
   
   void func_gauss(const Doub x, VecDoub_I &a, Doub &y, VecDoub_O &dyda) {
      Int na=a.size();
      Doub fac,ex,arg;
      y=0.;
      for (int i=0; i<na-1; i+=2) {
         arg=x/a[i+1];
         ex=exp(-SQR(arg));
         fac=a[i]*ex*2.*arg*arg;
         y += a[i]*ex;
         dyda[i]=ex;
         dyda[i+1]=fac/a[i+1];
      }
   }
   
   void func_sgauss(const Doub x, VecDoub_I &a, Doub &y, VecDoub_O &dyda) {
      Int na=a.size();
      Doub fac,ex,arg;
      y=0.;
      for (int i=0; i<na-1; i+=3) {
         arg=(x-a[i+1])/a[i+2];
         ex=exp(-SQR(arg));
         fac=a[i]*ex*2.*arg;
         y += a[i]*ex;
         dyda[i]=ex;
         dyda[i+1]=fac/a[i+2];
         dyda[i+2]=fac*arg/a[i+2];
      }
   }
}

#endif
