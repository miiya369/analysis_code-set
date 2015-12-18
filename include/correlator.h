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
    
    bool fb_mean_flg, new_flg_corr;
    
public:
    cdouble *corr;
    
    CORRELATOR(){
        class_name = "CORRELATOR:ANALYSIS_BASE________";
        func_name = "______________________";
        route( class_name, func_name, 1 );
        
        fb_mean_flg = false;
        new_flg_corr = false;
    }
    
    ~CORRELATOR(){
        if( new_flg_corr ) delete [] corr;
        
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
        
        if( new_flg_corr ) error(1,"correlator has already inputed !");
        else input_corr();
        func_name = "set_corr______________";
        route( class_name, func_name, 0 );
    }
    
    void delete_corr(){
        
        func_name = "delete_corr___________";
        route( class_name, func_name, 1 );
        
        if( new_flg_corr ){
            delete [] corr;
            new_flg_corr = false;
        }
        route( class_name, func_name, 0 );
    }
    
    void input_corr();
    void make_JK_sample_corr( int );
    void output_corr_all( const char* );
    void output_corr_err( const char* );
    void output_corr_fit( const char* );
    void output_effmass_all( const char* );
    void output_effmass_err( const char* );
};

#endif
