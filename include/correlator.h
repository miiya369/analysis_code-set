//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Correlator
 * @brief   Header file for correlator class
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#ifndef CORRELATOR_H
#define CORRELATOR_H

#include <base.h>

//--------------------------------------------------------------------------
/**
 * @brief The class for correlator
 */
//--------------------------------------------------------------------------

class CORRELATOR : virtual public ANALYSIS_BASE{
    
private:
    string class_name, func_name;
    
protected:
    string HAD_name;
    
    bool fb_mean_flg;
    
public:
    cdouble *corr;
    cdouble *Z_factor;
    
    CORRELATOR(){
        class_name = "CORRELATOR:ANALYSIS_BASE________";
        func_name = "______________________";
        route( class_name, func_name, 1 );
        
        corr     = NULL;
        Z_factor = NULL;
        fb_mean_flg = false;
    }
    
    ~CORRELATOR(){
        if( corr     != NULL ) delete [] corr;
        if( Z_factor != NULL ) delete [] Z_factor;
        
        func_name = "______________________";
        route( class_name, func_name, 0 );
    }
    
    void set_corr( int HAD_TYPE ){
        
        func_name = "set_corr______________";
        route( class_name, func_name, 1 );
        
        HAD_name = had_type_to_name(HAD_TYPE);
        if( HAD_TYPE < MESON_BARYON_BOUNDARY ) fb_mean_flg = false;
        if( HAD_TYPE > MESON_BARYON_BOUNDARY ){
            fb_mean_flg = true;
            HAD_name += "_";
            HAD_name += data_list[SNK_RELA];
            HAD_name += "_";
            HAD_name += data_list[SRC_RELA];
        }
        
        if( corr != NULL ) error(1,"correlator has already inputed !");
        else input_corr();
        func_name = "set_corr______________";
        route( class_name, func_name, 0 );
    }
    
    void delete_corr(){
        
        func_name = "delete_corr___________";
        route( class_name, func_name, 1 );
        
        if( corr != NULL ){
            delete [] corr;
            corr = NULL;
        }
        if( Z_factor != NULL ){
            delete [] Z_factor;
            Z_factor = NULL;
        }
        route( class_name, func_name, 0 );
    }
    
    void input_corr();
    void make_JK_sample_corr( int );
    void calc_z_factor();
    void output_corr_all( const char* );
    void output_corr_err( const char* );
    void output_corr_fit( const char* );
    void output_effmass_all( const char* );
    void output_effmass_err( const char* );
    void output_z_factor_all( const char* );
    void output_z_factor_err( const char* );
};

#endif
