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
    cdouble *corr1;
    cdouble *corr2;
    
    string potential_type;
    int HAD1_type, HAD2_type;
    int channel, time_slice, spin, ang_mom;
    int Rcorr_t[3];
    double reduced_mass;
    
    bool endian_flg, Rcorr_reread_flg[3], read_time_slice_flg[3];
    
public:
    cdouble *potential;
    R_CORRELATOR *Rcorr;
    
    POTENTIAL(){
        class_name = "POTENTIAL:ANALYSIS_BASE_________";
        func_name = "______________________";
        route( class_name, func_name, 1 );
        
        potential = NULL;
        Rcorr     = NULL;
        corr1     = NULL;
        corr2     = NULL;
    }
    
    ~POTENTIAL(){
        if( potential != NULL ) delete [] potential;
        if( Rcorr     != NULL ) delete [] Rcorr;
        if( corr1     != NULL ){
            delete [] corr1;
            delete [] corr2;
        }
        
        func_name = "______________________";
        route( class_name, func_name, 0 );
    }
    
    void set_pot( int CHANNEL, int it, bool endian_FLG,
                  int SPIN, int ANGMOM, double REDUCED_MASS ){
        
        func_name = "set_pot_Rcorr_NBS/corr";
        route( class_name, func_name, 1 );
        
        channel = CHANNEL;
        time_slice = it;
        spin = SPIN;
        ang_mom = ANGMOM;
        endian_flg = endian_FLG;
        reduced_mass = REDUCED_MASS;
        
        if( channel == PROTON_NEUTRON__PROTON_NEUTRON ){
            HAD1_type = PROTON;
            HAD2_type = PROTON;
        }else if( channel == PROTON_LAMBDA__PROTON_LAMBDA  ||
                  channel == PROTON_LAMBDA__PROTON_SIGMAZ  ||
                  channel == PROTON_LAMBDA__NEUTRON_SIGMAP ||
                  channel == N_LAMBDA_12__N_LAMBDA_12      ||
                  channel == N_LAMBDA_12__N_SIGMA_12       ||
                  channel == N_LAMBDA_12__N_SIGMA_32 ){
            HAD1_type = PROTON;
            HAD2_type = LAMBDA;
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
            HAD1_type = PROTON;
            HAD2_type = SIGMA;
        }
        input_pot();
        func_name = "set_pot_Rcorr_NBS/corr";
        route( class_name, func_name, 0 );
    }
    
    void set_pot( int CHANNEL, int it, bool endian_FLG,
                 int SPIN, int ANGMOM, double HAD1_mass, double HAD2_mass ){
        
        func_name = "set_pot_Rcorr_NBS/mass";
        route( class_name, func_name, 1 );
        
        channel = CHANNEL;
        time_slice = it;
        spin = SPIN;
        ang_mom = ANGMOM;
        endian_flg = endian_FLG;
        reduced_mass = HAD1_mass*HAD2_mass/(HAD1_mass+HAD2_mass);
        
        if( channel == PROTON_NEUTRON__PROTON_NEUTRON ){
            HAD1_type = PROTON;
            HAD2_type = PROTON;
        }else if( channel == PROTON_LAMBDA__PROTON_LAMBDA  ||
                 channel == PROTON_LAMBDA__PROTON_SIGMAZ  ||
                 channel == PROTON_LAMBDA__NEUTRON_SIGMAP ||
                 channel == N_LAMBDA_12__N_LAMBDA_12      ||
                 channel == N_LAMBDA_12__N_SIGMA_12       ||
                 channel == N_LAMBDA_12__N_SIGMA_32 ){
            HAD1_type = PROTON;
            HAD2_type = LAMBDA;
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
            HAD1_type = PROTON;
            HAD2_type = SIGMA;
        }
        input_pot( HAD1_mass, HAD2_mass );
        func_name = "set_pot_Rcorr_NBS/mass";
        route( class_name, func_name, 0 );
    }
    
    void delete_pot(){
        
        func_name = "delete_pot____________";
        route( class_name, func_name, 1 );
        
        if( Rcorr != NULL ){
            delete [] Rcorr;
            Rcorr = NULL;
        }
        if( corr1 != NULL ){
            delete [] corr1;
            delete [] corr2;
            corr1 = NULL;
            corr2 = NULL;
        }
        route( class_name, func_name, 0 );
    }
    
    void delete_pot_corr(){
        
        func_name = "delete_pot_corr_______";
        route( class_name, func_name, 1 );
        
        if( corr1 != NULL ){
            delete [] corr1;
            delete [] corr2;
            corr1 = NULL;
            corr2 = NULL;
        }
        route( class_name, func_name, 0 );
    }
    
    void calc_laplacian();
    void calc_1st_timediff();
    void calc_2nd_timediff();
    void calc_pot_kernel();
    void input_pot();
    void input_pot( double, double );
    void set_pot_from_binary( const char*, int, int, bool );
    void output_single_pot_all( const char* );
    void output_single_pot_err( const char* );
    void output_single_pot_fit( const char* );
    void output_couple_pot_all( const char* );
    void output_couple_pot_err( const char* );
    void output_couple_pot_fit( const char* );
};

#endif
