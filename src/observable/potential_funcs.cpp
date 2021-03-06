//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup observable
 * @brief   Definition for potentail using calculate schrodinger equation
 * @author  Takaya Miyamoto
 * @since   Sat Nov 12 03:33:54 JST 2016
 */
//--------------------------------------------------------------------------

#include <observable/phase_shift.h>

double observable::V(  const double x, const double* param
                     , const int Nparam, const int func_num ) {
   double v = 0.0;
   
        if ( func_num == 1 )   v = func_const (x, param, Nparam);
   else if ( func_num == 2 )   v = func_exp   (x, param, Nparam);
   else if ( func_num == 3 ||
             func_num == 4 ||
             func_num == 5 ||
             func_num == 8 )   v = func_gauss (x, param, Nparam);
   else if ( func_num == 6 ||
             func_num == 7 )   v = func_sgauss(x, param, Nparam);
   else if ( func_num == 9 )   v = func_1g1y  (x, param, Nparam);
   else if ( func_num == 0 )   v = func_test  (x, param, Nparam);
   
   return v;
}

double observable::Vp(  const double p1, const double p2, const double* param
                      , const int Nparam, const int func_num ) {
   double v = 0.0;
   
   if( func_num == 3 ||
       func_num == 4 ||
       func_num == 5 ||
       func_num == 8 )   v = func_gauss_mom (p1, p2, param, Nparam);
   else ERROR_COMMENTS("invalid function type !");
   
   return v;
}

double observable::func_const(  const double x, const double* param
                              , const int Nparam ) {
   return param[0];
}

double observable::func_exp(  const double x, const double* param
                            , const int Nparam ) {
   double ex;
   double y=0.0;
   
   for (int i=0; i<Nparam; i+=2) {
      ex = exp(-param[i+1]*x);
      y += param[i]*ex;
   }
   return y;
}

double observable::func_gauss(  const double x, const double* param
                              , const int Nparam ) {
   double ex,arg;
   double y=0.0;
   
   for (int i=0; i<Nparam; i+=2) {
      arg = x/param[i+1];
      ex  = exp(-(arg*arg));
      y  += param[i]*ex;
   }
   return y;
}

double observable::func_sgauss(  const double x, const double* param
                               , const int Nparam ) {
   double ex,arg;
   double y=0.0;
   
   for (int i=0; i<Nparam; i+=3) {
      arg = (x-param[i+1])/param[i+2];
      ex  = exp(-(arg*arg));
      y  += param[i]*ex;
   }
   return y;
}

double observable::func_1g1y(  const double x, const double* param
                             , const int Nparam ) {
   double ex, arg;
   double y=0.0;
   
   if (x == 0.0) return param[0];
   
   for (int i=0; i<2; i+=2) {
      arg = x/param[i+1];
      ex  = exp(-(arg*arg));
      y  += param[i]*ex;
   }
   for (int i=2; i<5; i+=3) {
      arg = x/param[i];
      ex  = exp(-(arg*arg));
      y  += (1.0-ex) * exp(-param[i+1]*x) / (param[i+2]*x);
   }
   return y;
}

double observable::func_test(  const double x, const double* param
                             , const int Nparam ) {
   double y=0.0;
   
   if (x <= param[0]) y=-param[1] / hbar_c;
   
   return y;
}

double observable::func_gauss_mom(  const double p1, const double p2
                                  , const double* param, const int Nparam ) {
   double ex;
   double y=0.0;
   
   if (p1!=0.0 && p2!=0.0) {
      for (int i=0; i<Nparam; i+=2) {
         ex = (  exp(-(pow((p1-p2)*param[i+1]/2.0,2)))
               - exp(-(pow((p1+p2)*param[i+1]/2.0,2))) );
         y += param[i]*param[i+1]*ex/(p1*p2);
      }
   }
   else if (p1!=0.0 && p2==0.0) {
      for (int i=0; i<Nparam; i+=2) {
         ex = exp(-(pow(p1*param[i+1]/2.0,2)));
         y += param[i]*pow(param[i+1],3)*ex;
      }
   }
   else {
      for (int i=0; i<Nparam; i+=2) {
         ex = exp(-(pow(p2*param[i+1]/2.0,2)));
         y += param[i]*pow(param[i+1],3)*ex;
      }
   }
   return y/(2.0*sqrt(PI));
}
