//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Definition for fit functions, For Numerical Recipe notation
 * @author  Takaya Miyamoto
 * @since   Sat Nov 12 03:33:54 JST 2016
 */
//--------------------------------------------------------------------------

#ifndef FITTING_FROM_NR_H
#define FITTING_FROM_NR_H

#include <Numerical_Recipe/nr3.h>
#include <Numerical_Recipe/gaussj.h>
#include <Numerical_Recipe/fitmrq.h>

namespace fit_funcs
{
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
   
   void func_1g1y(const Doub x, VecDoub_I &a, Doub &y, VecDoub_O &dyda) {
      Doub fac,ex,arg;
      y=0.;
      
      if (x != 0.) {
         for (int i=0; i<2; i+=2) {
            arg = x/a[i+1];
            ex  = exp(-SQR(arg));
            y  += a[i]*ex;
            dyda[i  ] = ex;
            dyda[i+1] = a[i]*ex*2.*arg*arg/a[i+1];
         }
         for (int i=2; i<5; i+=3) {
            arg = x/a[i];
            ex  = exp(-SQR(arg));
            fac = exp(-a[i+1]*x) / (a[i+2]*x);
            y  += (1.0-ex) * fac;
            dyda[i  ] = fac*ex*2.*arg*arg/a[i];
            dyda[i+1] = -(1.0-ex) * fac * a[i+1];
            dyda[i+2] = -(1.0-ex) * fac / a[i+2];
         }
      } else {
         y       = a[0];
         dyda[0] = 1.0;
         dyda[1] = 0.0;
         dyda[2] = 0.0;
         dyda[3] = 0.0;
         dyda[4] = 0.0;
      }
   }
}

#endif
