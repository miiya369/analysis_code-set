//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup R-correlator
 * @brief   Header file for R-correlator class
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#ifndef R_CORRELATOR_H
#define R_CORRELATOR_H

#include <NBSwave.h>
#include <correlator.h>

//--------------------------------------------------------------------------
/**
 * @brief The class for R-correlator
 */
//--------------------------------------------------------------------------

class R_CORRELATOR : public NBS_WAVE, public CORRELATOR{
    
private:
    string class_name, func_name;
    
protected:
    string HAD1_name, HAD2_name;
    int    HAD1_type, HAD2_type;
    
    bool new_flg_Rcorr;
    
public:
    cdouble *Rcorr;
    
    R_CORRELATOR(){
        class_name = "R_CORRELATOR:CORRELATOR,NBS_WAVE";
        func_name = "______________________";
        route( class_name, func_name, 1 );
        
        new_flg_Rcorr = false;
    }
    
    ~R_CORRELATOR(){
        if( new_flg_Rcorr ) delete [] Rcorr;
        
        func_name = "______________________";
        route( class_name, func_name, 0 );
    }
    
    void set_Rcorr( int CHANNEL, int it, bool endian_FLG, int SPIN, int ANGMOM ){
        
        func_name = "set_Rcorr_____________";
        route( class_name, func_name, 1 );
        
        channel = CHANNEL;
        time_slice = it;
        endian_flg = endian_FLG;
        
        if( channel == PROTON_NEUTRON__PROTON_NEUTRON ){
            HAD1_type = PROTON;
            HAD2_type = PROTON;
        }else if( channel == PROTON_LAMBDA__PROTON_LAMBDA ||
                 channel == PROTON_LAMBDA__PROTON_SIGMAZ  ||
                 channel == PROTON_LAMBDA__NEUTRON_SIGMAP ||
                 channel == N_LAMBDA_12__N_LAMBDA_12      ||
                 channel == N_LAMBDA_12__N_SIGMA_12       ||
                 channel == N_LAMBDA_12__N_SIGMA_32 ){
            HAD1_type = LAMBDA;
            HAD2_type = PROTON;
        }else if( channel == PROTON_SIGMAZ__PROTON_LAMBDA  ||
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
        HAD1_name = had_type_to_name(HAD1_type);
        HAD2_name = had_type_to_name(HAD2_type);
        if( HAD1_type > MESON_BARYON_BOUNDARY ){
            HAD1_name += "_";
            HAD1_name += data_list[SNK_RELA];
            HAD1_name += "_";
            HAD1_name += data_list[SRC_RELA];
        }
        if( HAD2_type > MESON_BARYON_BOUNDARY ){
            HAD2_name += "_";
            HAD2_name += data_list[SNK_RELA];
            HAD2_name += "_";
            HAD2_name += data_list[SRC_RELA];
        }
        
        fb_mean_flg = true;
        orgNBS_size = 4*4* xyzSIZE * xyzSIZE * xyzSIZE;
        NBS_size    = xyzSIZE * xyzSIZE * xyzSIZE;
        
        if( new_flg_Rcorr ) error(1,"R correlator has already inputed !");
        else input_Rcorr( SPIN, ANGMOM );
        func_name = "set_Rcorr_____________";
        route( class_name, func_name, 0 );
    }
    
    void set_Rcorr( int CHANNEL, int it, bool endian_FLG, int SPIN, int ANGMOM,
                   cdouble *corr1, cdouble *corr2 ){
        
        func_name = "set_Rcorr_____________";
        route( class_name, func_name, 1 );
        
        channel = CHANNEL;
        time_slice = it;
        endian_flg = endian_FLG;
        
        fb_mean_flg = true;
        orgNBS_size = 4*4* xyzSIZE * xyzSIZE * xyzSIZE;
        NBS_size    = xyzSIZE * xyzSIZE * xyzSIZE;
        
        if( new_flg_Rcorr ) error(1,"R correlator has already inputed !");
        else input_Rcorr( SPIN, ANGMOM, corr1, corr2 );
        func_name = "set_Rcorr_____________";
        route( class_name, func_name, 0 );
    }
    
    void delete_Rcorr(){
        
        func_name = "delete_Rcorr__________";
        route( class_name, func_name, 1 );
        
        if( new_flg_Rcorr ){
            delete [] Rcorr;
            new_flg_Rcorr = false;
        }
        route( class_name, func_name, 0 );
    }
    
    void input_Rcorr( int, int );
    void input_Rcorr( int, int, cdouble*, cdouble* );
    void output_Rcorr_all( const char* );
    void output_Rcorr_err( const char* );
};

#endif
