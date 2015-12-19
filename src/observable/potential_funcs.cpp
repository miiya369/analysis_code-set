//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup phase shift
 * @brief   Definition for potentail using calculate schrodinger equation
 * @author  Takaya Miyamoto
 * @since   Thu Jul 23 15:00:00 JST 2015
 */
//--------------------------------------------------------------------------

#include <observable/phase_shift.h>

double PHASE_SHIFT::V( double rr, int conf ) {
   
   double x = rr / lattice_space;
   double v = 0.0;
   
        if( func_type.number == 1 )   v = func_const (x,conf);
   else if( func_type.number == 2 )   v = func_exp   (x,conf);
   else if( func_type.number == 3 ||
            func_type.number == 4 ||
            func_type.number == 5 )   v = func_gauss (x,conf);
   else if( func_type.number == 6 ||
            func_type.number == 7 )   v = func_sgauss(x,conf);
   else if( func_type.number == 0 )   v = func_test  (x);
   
   return v*hbar_c/lattice_space;
}

double PHASE_SHIFT::func_const( double x, int conf ) {
   
   return param[idx(conf,0)];
}

double PHASE_SHIFT::func_exp( double x, int conf ) {
   
   double ex;
   double y=0.0;
   
   for (int i=0; i<func_type.Nparam-1; i+=2) {
      ex = exp(-param[idx(conf,i+1)]*x);
      y += param[idx(conf,i)]*ex;
   }
   return y;
}

double PHASE_SHIFT::func_gauss( double x, int conf ) {
   
   double ex,arg;
   double y=0.0;
   
   for(int i=0; i<func_type.Nparam-1; i+=2) {
      arg= x/param[idx(conf,i+1)];
      ex = exp(-(arg*arg));
      y += param[idx(conf,i)]*ex;
   }
   return y;
}

double PHASE_SHIFT::func_sgauss( double x, int conf ) {
   
   double ex,arg;
   double y=0.0;
   
   for(int i=0; i<func_type.Nparam-1; i+=3) {
      arg= (x-param[idx(conf,i+1)])/param[idx(conf,i+2)];
      ex = exp(-(arg*arg));
      y += param[idx(conf,i)]*ex;
   }
   return y;
}

double PHASE_SHIFT::func_test( double x ) {
   
   double y=0.0;
   
   if (x <= test_r_0) y=test_V_0/hbar_c;
   
   return y;
}
