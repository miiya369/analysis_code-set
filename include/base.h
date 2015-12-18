//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBSwave, Correlator
 * @brief   Header file for base class
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#ifndef BASE_H
#define BASE_H

#include <analysis.h>

//--------------------------------------------------------------------------
/**
 * @brief The base class for correlator and NBS wave function
 */
//--------------------------------------------------------------------------

class ANALYSIS_BASE{
    
private:
    string class_name, func_name;
    
protected:
    int xyzSIZE;
    int tSIZE;
    int N_conf;
    char data_list[MAX_N_DATA][MAX_LEN_PATH];
    
public:
    ANALYSIS_BASE(){
        class_name = "ANALYSIS_BASE___________________";
        func_name = "______________________";
        route( class_name, func_name, 1 );
    }
    
    ~ANALYSIS_BASE(){
        func_name = "______________________";
        route( class_name, func_name, 0 );
    }
    
    int NBScood( int x, int y, int z, int snk_ab, int src_ab, int conf ){
        return snk_ab + 4 *( x + xyzSIZE *( y + xyzSIZE
                          *( z + xyzSIZE *( src_ab + 4 * conf ))));
    }
    
    int xyzn( int x, int y, int z, int conf ){
        return x + xyzSIZE *( y + xyzSIZE * ( z + xyzSIZE * conf ));
    }
    int xyz( int x, int y, int z ){ return x + xyzSIZE *( y + xyzSIZE * z ); }
    
    int matrix_idx( int row, int column, int rot_type ){
        return column + 4 * ( row + 4 * rot_type );
    }   // rot_type = 0 -> E, 1~6 -> 6C4, 7~9 -> 3C2, 10~17 -> 8C3, 18~23 -> 6C2
    
    int xyznt( int x, int y, int z, int conf, int t ){
        return x + xyzSIZE *( y + xyzSIZE* ( z + xyzSIZE
                                             * ( conf + N_conf * t )));
    }
    int corr_idx( int t, int ptc_type ){ return t + tSIZE * ptc_type; }
    
    int nt( int conf, int t ){ return conf + N_conf * t; }
    
    cdouble cmp_sqr( cdouble cmp ){
        return cdouble( pow( cmp.real(), 2 ), pow( cmp.imag(), 2 ) );
    }
    cdouble cmp_sqrt( cdouble cmp ){
        return cdouble( sqrt( cmp.real() ), sqrt( cmp.imag() ) );
    }
    
////////////////////////////////////////////////////////////////////////////////
    
    void set_env( const int xyz_SIZE, const int t_SIZE, const int N_CONF,
                  char list[MAX_N_DATA][MAX_LEN_PATH] ){
        
        func_name = "set_env_______________";
        route( class_name, func_name, 1 );
        
        xyzSIZE     = xyz_SIZE;
        tSIZE       = t_SIZE;
        N_conf      = N_CONF;
        
        for(int i=0;i<MAX_N_DATA;i++) for(int j=0;j<MAX_LEN_PATH;j++)
            data_list[i][j] = list[i][j];
        
        route( class_name, func_name, 0 );
    }
};

#endif
