//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup phase shift
 * @brief   Header file for phase shift class
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#ifndef PHASE_SHIFT_H
#define PHASE_SHIFT_H

#include <observable.h>

//--------------------------------------------------------------------------
/**
 * @brief The class for calculate phase shift
 */
//--------------------------------------------------------------------------

class PHASE_SHIFT : public OBSERVABLE{
    
private:
    string class_name, func_name;
    
protected:
    int func_type;
    int N_param;
    
    double *param;
    double lattice_space;
    double ll;
    double del;
    double r_max;
    double test_r_0;
    double test_V_0;
    
    bool endian_flg, new_flg_param, env_flg;
    
public:
    PHASE_SHIFT(){
        class_name = "PHASE_SHIFT:OBSERVABLE__________";
        func_name = "______________________";
        route( class_name, func_name, 1 );
        
        endian_flg          = false;
        new_flg_param       = false;
        env_flg             = false;
    }
    
    ~PHASE_SHIFT(){
        if( new_flg_phase_shift ){
            delete [] phase_shift;
            new_flg_phase_shift = false;
        }
        if( new_flg_param ) delete [] param;
        
        func_name = "______________________";
        route( class_name, func_name, 0 );
    }
    
////////////////////////////////////////////////////////////////////////////////
    
    void delete_param(){
        
        func_name = "delete_param__________";
        route( class_name, func_name, 1 );
        
        if( new_flg_param ){
            delete [] param;
            new_flg_param = false;
        }
        route( class_name, func_name, 0 );
    }
    
    void    input_param( const char*, bool );
    void    set_env( double, double, double, double, double );
    void    calc_phase_shift( int, int );
    double  V( double, int );
    double  func_const   ( double, int );
    double  func_exp     ( double, int );
    double  func_gauss   ( double, int );
    double  func_sgauss  ( double, int );
    void    output_phase_shift_all( const char* );
    void    output_phase_shift_err( const char* );
    void    set_test( double, double, double, double, double );
    double  func_test( double, int );
};

#endif
