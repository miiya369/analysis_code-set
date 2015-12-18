//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup phase shift, scattering length
 * @brief   Header file for observable class
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <analysis.h>

//--------------------------------------------------------------------------
/**
 * @brief The class for calculate observables
 */
//--------------------------------------------------------------------------

class OBSERVABLE{
    
private:
    string class_name, func_name;
    
protected:
    int    N_conf, E_min, E_max;
    double mass;
    double hbar_c;
    
    bool new_flg_phase_shift, new_flg_scatt_length;
    
public:
    double *phase_shift;
    double *scatt_length;
    
    OBSERVABLE(){
        class_name = "OBSERVABLE______________________";
        func_name = "______________________";
        route( class_name, func_name, 1 );
        
        hbar_c = 197.327;   // MeV*fm
        new_flg_phase_shift  = false;
        new_flg_scatt_length = false;
    }
    
    ~OBSERVABLE(){
        if( new_flg_phase_shift )  delete [] phase_shift;
        if( new_flg_scatt_length ) delete [] scatt_length;
        
        func_name = "______________________";
        route( class_name, func_name, 0 );
    }
    
    int idx( int conf, int dat ){ return conf + N_conf * dat; }
    
////////////////////////////////////////////////////////////////////////////////
    
    void delete_phase_shift(){
        
        func_name = "delete_phase_shift____";
        route( class_name, func_name, 1 );
        
        if( new_flg_phase_shift ){
            delete [] phase_shift;
            new_flg_phase_shift = false;
        }
        route( class_name, func_name, 0 );
    }
    
    void delete_scatt_length(){
        
        func_name = "delete_scatt_length___";
        route( class_name, func_name, 1 );
        
        if( new_flg_scatt_length ){
            delete [] scatt_length;
            new_flg_scatt_length = false;
        }
        route( class_name, func_name, 0 );
    }
    
    void input_phase_shift( const char*, int, int, int, double );
    void calc_scatt_len();
    void output_scatt_len_all( const char* );
    void output_scatt_len_err( const char* );
};

#endif
