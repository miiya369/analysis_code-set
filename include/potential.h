//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Header file for potential class
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#ifndef POTENTIAL_H
#define POTENTIAL_H

#include <R_correlator.h>

//--------------------------------------------------------------------------
/**
 * @brief The class for potential
 */
//--------------------------------------------------------------------------

class POTENTIAL : public ANALYSIS_BASE{
    
private:
    string class_name, func_name;
    
protected:
    R_CORRELATOR *Rcorr2;
    R_CORRELATOR *Rcorr3;
    cdouble *corr1;
    cdouble *corr2;
    
    string potential_type;
    int HAD1_type, HAD2_type;
    int channel, time_slice, spin, ang_mom;
    double mass;
    
    bool new_flg_pot, new_flg_Rcorr1, new_flg_Rcorr2,
         new_flg_Rcorr3, new_flg_corr, endian_flg;
    
public:
    cdouble *potential;
    R_CORRELATOR *Rcorr1;
    
    POTENTIAL(){
        class_name = "POTENTIAL:ANALYSIS_BASE_________";
        func_name = "______________________";
        route( class_name, func_name, 1 );
        
        new_flg_pot    = false;
        new_flg_Rcorr1 = false;
        new_flg_Rcorr2 = false;
        new_flg_Rcorr3 = false;
        new_flg_corr   = false;
    }
    
    ~POTENTIAL(){
        if( new_flg_pot )    delete [] potential;
        if( new_flg_Rcorr1 ) delete Rcorr1;
        if( new_flg_Rcorr2 ) delete Rcorr2;
        if( new_flg_Rcorr3 ) delete Rcorr3;
        if( new_flg_corr ){
            delete [] corr1;
            delete [] corr2;
        }
        
        func_name = "______________________";
        route( class_name, func_name, 0 );
    }
    
    void set_pot( int CHANNEL, int it, bool endian_FLG,
                  int SPIN, int ANGMOM, double MASS ){
        
        func_name = "set_pot_______________";
        route( class_name, func_name, 1 );
        
        channel = CHANNEL;
        time_slice = it;
        spin = SPIN;
        ang_mom = ANGMOM;
        endian_flg = endian_FLG;
        mass = MASS;
        
        snprintf(data_list[N_TIME],sizeof(data_list[N_TIME]),"%d", time_slice);
        
        if( channel == PROTON_NEUTRON__PROTON_NEUTRON ){
            HAD1_type = PROTON;
            HAD2_type = PROTON;
        }else if( channel == PROTON_LAMBDA__PROTON_LAMBDA  ||
                  channel == PROTON_LAMBDA__PROTON_SIGMAZ  ||
                  channel == PROTON_LAMBDA__NEUTRON_SIGMAP ||
                  channel == N_LAMBDA_12__N_LAMBDA_12      ||
                  channel == N_LAMBDA_12__N_SIGMA_12       ||
                  channel == N_LAMBDA_12__N_SIGMA_32 ){
            HAD1_type = LAMBDA;
            HAD2_type = PROTON;
        }else if( channel == PROTON_SIGMAZ__PROTON_LAMBDA   ||
                  channel == PROTON_SIGMAZ__PROTON_SIGMAZ   ||
                  channel == PROTON_SIGMAZ__NEUTRON_SIGMAP  ||
                  channel == NEUTRON_SIGMAP__PROTON_LAMBDA  ||
                  channel == NEUTRON_SIGMAP__PROTON_SIGMAZ  ||
                  channel == NEUTRON_SIGMAP__NEUTRON_SIGMAP ||
                  channel == N_SIGMA_12__N_LAMBDA_12        ||
                  channel == N_SIGMA_12__N_SIGMA_12         ||
                  channel == N_SIGMA_32__N_LAMBDA_12        ||
                  channel == N_SIGMA_32__N_SIGMA_32 ){
            HAD1_type = SIGMA;
            HAD2_type = PROTON;
        }
        route( class_name, func_name, 0 );
    }
    
    void delete_pot(){
        
        func_name = "delete_pot____________";
        route( class_name, func_name, 1 );
        
        if( new_flg_pot ){
            delete [] potential;
            new_flg_pot = false;
        }
        if( new_flg_Rcorr1 ){
            delete Rcorr1;
            new_flg_Rcorr1 = false;
        }
        if( new_flg_Rcorr2 ){
            delete Rcorr2;
            new_flg_Rcorr2 = false;
        }
        if( new_flg_Rcorr3 ){
            delete Rcorr3;
            new_flg_Rcorr3 = false;
        }
        route( class_name, func_name, 0 );
    }
    
    void delete_pot_for_coupled(){
        
        func_name = "delete_pot_for_coupled";
        route( class_name, func_name, 1 );
        
        if( new_flg_Rcorr2 ){
            delete Rcorr2;
            new_flg_Rcorr2 = false;
        }
        if( new_flg_Rcorr3 ){
            delete Rcorr3;
            new_flg_Rcorr3 = false;
        }
        if( new_flg_corr ){
            delete [] corr1;
            delete [] corr2;
            new_flg_corr = false;
        }
        route( class_name, func_name, 0 );
    }
    
    void calc_laplacian();
    void calc_1st_timediff();
    void calc_2nd_timediff();
    void calc_pot_kernel();
    void output_single_pot_all( const char* );
    void output_single_pot_err( const char* );
    void output_single_pot_fit( const char* );
    void output_couple_pot_err( const char* );
    void output_couple_pot_fit( const char* );
};

#endif
