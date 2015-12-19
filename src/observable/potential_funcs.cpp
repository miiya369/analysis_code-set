//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup observable
 * @brief   Definition for potentail using calculate schrodinger equation
 * @author  Takaya Miyamoto
 * @since   Fri Sep  4 04:20:49 JST 2015
 */
//--------------------------------------------------------------------------

#include <observable/phase_shift.h>

double observable::V( double x, double* param, int Nparam, int func_num ) {
   
   double v = 0.0;
   
        if( func_num == 1 )   v = func_const (x, param, Nparam);
   else if( func_num == 2 )   v = func_exp   (x, param, Nparam);
   else if( func_num == 3 ||
            func_num == 4 ||
            func_num == 5 )   v = func_gauss (x, param, Nparam);
   else if( func_num == 6 ||
            func_num == 7 )   v = func_sgauss(x, param, Nparam);
   else if( func_num == 0 )   v = func_test  (x, param, Nparam);
   
   return v;
}

double observable::func_const( double x, double* param, int Nparam ) {
   
   return param[0];
}

double observable::func_exp( double x, double* param, int Nparam ) {
   
   double ex;
   double y=0.0;
   
   for (int i=0; i<Nparam; i+=2) {
      ex = exp(-param[i+1]*x);
      y += param[i]*ex;
   }
   return y;
}

double observable::func_gauss( double x, double* param, int Nparam ) {
   
   double ex,arg;
   double y=0.0;
   
   for(int i=0; i<Nparam; i+=2) {
      arg= x/param[i+1];
      ex = exp(-(arg*arg));
      y += param[i]*ex;
   }
   return y;
}

double observable::func_sgauss( double x, double* param, int Nparam ) {
   
   double ex,arg;
   double y=0.0;
   
   for(int i=0; i<Nparam; i+=3) {
      arg= (x-param[i+1])/param[i+2];
      ex = exp(-(arg*arg));
      y += param[i]*ex;
   }
   return y;
}

double observable::func_test( double x, double* param, int Nparam ) {
   
   double y=0.0;
   
   if (x <= param[0]) y=-param[1] / 197.327053;
   
   return y;
}
