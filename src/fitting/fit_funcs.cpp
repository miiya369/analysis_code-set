//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Definition for fit functions
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <fitting.h>

double FITTING::fit_functions(double x, double *a, double *dyda){
    
    double y = 0.0;
    
         if( func_type == CONSTANT )                y = func_const (x, a, dyda);
    else if( func_type == ONE_EXPONENTIAL )         y = func_exp   (x, a, dyda);
    else if( func_type == TWO_GAUSSIAN )            y = func_gauss (x, a, dyda);
    else if( func_type == THREE_GAUSSIAN )          y = func_gauss (x, a, dyda);
    else if( func_type == TWO_SHIFTED_GAUSSIAN )    y = func_sgauss(x, a, dyda);
    
    return y;
}

double FITTING::func_const(double x, double *a, double *dyda){
    
    double y=a[0];
    
    dyda[0]=1;
    
    return y;
}

double FITTING::func_exp(double x, double *a, double *dyda){
    
    double ex;
    double y=0.0;
    
    for( int i=0; i<N_param-1; i+=2){
        ex=exp(-a[i+1]*x);
        y += a[i]*ex;
        
        dyda[i]=ex;
        dyda[i+1]=-x*a[i]*ex;
    }
    return y;
}

double FITTING::func_gauss(double x, double *a, double *dyda){
    
    double fac,ex,arg;
    double y=0.0;
    
    for( int i=0; i<N_param-1; i+=2){
        arg=x/a[i+1];
        ex=exp(-(arg*arg));
        fac=a[i]*ex*2.0*arg*arg;
        y += a[i]*ex;
        
        dyda[i]=ex;
        dyda[i+1]=fac/a[i+1];
    }
    return y;
}

double FITTING::func_sgauss(double x, double *a, double *dyda){
    
    double fac,ex,arg;
    double y=0.0;
    
    for( int i=0; i<N_param-1; i+=3){
        arg=(x-a[i+1])/a[i+2];
        ex=exp(-(arg*arg));
        fac=a[i]*ex*2.0*arg;
        y += a[i]*ex;
        
        dyda[i]=ex;
        dyda[i+1]=fac/a[i+2];
        dyda[i+2]=fac*arg/a[i+2];
    }
    return y;
}
