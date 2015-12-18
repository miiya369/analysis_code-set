//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBS wave
 * @brief   Header file for NBSwave class
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#ifndef NBSWAVE_H
#define NBSWAVE_H

#include <base.h>

class NBS_WAVE : virtual public ANALYSIS_BASE{
    
private:
    string class_name, func_name;
    
protected:
    int rot_matrix[384];
    int orgNBS_size, NBS_size, time_slice, channel;
    string NBS_dir;
    
    bool endian_flg, new_flg_orgNBS, new_flg_NBSwave;
    
public:
    cdouble *orgNBS;
    cdouble *NBSwave;
    
    NBS_WAVE(){
        class_name = "NBS_WAVE:ANALYSIS_BASE__________";
        func_name = "______________________";
        route( class_name, func_name, 1 );
        
        endian_flg = false;
        new_flg_orgNBS = false;
        new_flg_NBSwave = false;
    }
    
    ~NBS_WAVE(){
        if( new_flg_orgNBS ) delete [] orgNBS;
        if( new_flg_NBSwave ) delete [] NBSwave;
        
        func_name = "______________________";
        route( class_name, func_name, 0 );
    }
    
    void set_NBS( int CHANNEL, int it, bool endian_FLG ){
        
        func_name = "set_NBS_______________";
        route( class_name, func_name, 1 );
        
        channel = CHANNEL;
        time_slice = it;
        endian_flg = endian_FLG;
        
        orgNBS_size = 4*4* xyzSIZE * xyzSIZE * xyzSIZE;
        NBS_size    = xyzSIZE * xyzSIZE * xyzSIZE;
        
        if( new_flg_orgNBS ) error(1,"NBSwave has already inputed !");
        else input_NBS();
        func_name = "set_NBS_______________";
        route( class_name, func_name, 0 );
    }
    
    void delete_NBS(){
        
        func_name = "delete_NBS____________";
        route( class_name, func_name, 1 );
        
        if( new_flg_orgNBS ){
            delete [] orgNBS;
            new_flg_orgNBS = false;
        }
        if( new_flg_NBSwave ){
            delete [] NBSwave;
            new_flg_NBSwave = false;
        }
        route( class_name, func_name, 0 );
    }
    
    void input_NBS();
    void set_rot_matrix();
    void make_JK_sample_NBS( int );
    void spin_projection( int );
    void ang_mom_projection( int );
    void parity_projection();
    void projection_NBS( int, int );
    void output_NBS_all( const char* );
    void output_NBS_err( const char* );
};

#endif
