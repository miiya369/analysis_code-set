//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Header file for fitting class
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#ifndef FITTING_H
#define FITTING_H

#include <analysis.h>

//--------------------------------------------------------------------------
/**
 * @brief The class for fitting
 */
//--------------------------------------------------------------------------

class FITTING{
    
private:
    string class_name, func_name;
    
protected:
    int fit_type;
    int func_type;
    int *dataSIZE;
    int N_conf, N_param;
    
    double *cood;
    double *data;
    double *err;
    double *param_ini;
    double *param_out;
    double stp_cnd;
    
    bool endian_flg, new_flg_data, new_flg_param, fitting_flg;
    
public:
    double chisq_mean;
    double chisq_err;
    double *param_mean;
    double *param_err;
    
    FITTING(){
        class_name = "FITTING_________________________";
        func_name = "______________________";
        route( class_name, func_name, 1 );
        
        endian_flg      = false;
        new_flg_data    = false;
        new_flg_param   = false;
        fitting_flg     = false;
    }
    
    ~FITTING(){
        if( new_flg_data ){
            delete [] dataSIZE;
            delete [] cood;
            delete [] data;
            delete [] err;
        }
        if( new_flg_param ){
            delete [] param_ini;
            delete [] param_out;
            delete [] param_err;
            delete [] param_mean;
        }
        
        func_name = "______________________";
        route( class_name, func_name, 0 );
    }

    int idx( int conf, int dat ){ return conf + N_conf * dat; }
    
    int prm_idx( int prm1, int prm2 ){ return prm1 + N_param * prm2; }

    void SWAP( double a, double b ){ double dummy = a;   a = b;   b = dummy; }
    
    double MAX( double a, double b ){
        double tmp;
        if( a>b ) tmp=a;
        else tmp=b;
        return tmp;
    }
    
///////////////////////////////////////////////////////////////////
    
    void delete_data(){
        
        func_name = "delete_data___________";
        route( class_name, func_name, 1 );
        
        if( new_flg_data ){
            delete [] dataSIZE;
            delete [] cood;
            delete [] data;
            delete [] err;
            new_flg_data = false;
            fitting_flg  = false;
        }
        route( class_name, func_name, 0 );
    }
    
    void reset_func(){
        
        func_name = "reset_func____________";
        route( class_name, func_name, 1 );
        
        if( new_flg_param ){
            delete [] param_ini;
            delete [] param_out;
            delete [] param_err;
            delete [] param_mean;
            new_flg_param = false;
            fitting_flg   = false;
        }
        route( class_name, func_name, 0 );
    }
    
    void input_data     ( const char*, bool );
    void set_func       ( int, double* );
    void fit_data       ( int, int, double );
    void print_func_gnu ( bool );
    void print_param    ();
    void output_param   ( const char* );
    
    double fit_functions(double, double*, double*);
    double func_const   (double, double*, double*);
    double func_exp     (double, double*, double*);
    double func_gauss   (double, double*, double*);
    double func_sgauss  (double, double*, double*);
    
//======== copy from Numerical Recipes ========//
    double  fitmrq( double*, double*, double*, double*, int );
    void    gaussj( double*, double* );
    double  mrqcof( double*, double*, double*, double*, double*, double*, int );
    void    covsrt( double* );
};

#endif
