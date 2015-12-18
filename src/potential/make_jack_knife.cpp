//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBSwave, Correlator
 * @brief   Make jack-knife samples
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <NBSwave.h>
#include <correlator.h>

void NBS_WAVE::make_JK_sample_NBS( int bin ){
    
    func_name = "make_JK_sample_NBS____";
    route( class_name, func_name, 1 );
    
    if( new_flg_NBSwave ){
        cdouble sum;
        cdouble *tmp;   tmp = new cdouble[ NBS_size * N_conf ];
        
        for(int z=0; z<xyzSIZE; z++) for(int y=0; y<xyzSIZE; y++) for(int x=0; x<xyzSIZE; x++){
            
            sum = cdouble( 0.0, 0.0 );
          
            for(int i=0; i<N_conf; i++) sum += NBSwave[ xyzn(x,y,z,i) ];
            for(int i=0; i<N_conf; i++) tmp[ xyzn(x,y,z,i) ] =
                ( sum - NBSwave[ xyzn(x,y,z,i) ] ) / double( N_conf-1 );
        }
        for( int n=0; n<NBS_size * N_conf; n++ ) NBSwave[n]=tmp[n];
        delete [] tmp;
    }else{
        error(1,"Have not setted NBSwave yet !");
    }
    
    route( class_name, func_name, 0 );
}

//--------------------------------------------------------------------------

void CORRELATOR::make_JK_sample_corr( int bin ){
    
    func_name = "make_JK_sample_corr___";
    route( class_name, func_name, 1 );

    if( new_flg_corr ){
        cdouble sum;
        cdouble *tmp;   tmp = new cdouble[ tSIZE * N_conf ];
        
        for(int t=0; t<tSIZE; t++){
            
            sum = cdouble( 0.0, 0.0 );
            
            for(int i=0; i<N_conf; i++) sum += corr[ nt(i,t) ];
            for(int i=0; i<N_conf; i++) tmp[ nt(i,t) ] =
                ( sum - corr[ nt(i,t) ] ) / double( N_conf-1 );
        }
        for( int n=0; n<tSIZE * N_conf; n++ ) corr[n]=tmp[n];
        delete [] tmp;
    }else{
        error(1,"Have not setted correlator yet !");
    }
    
    route( class_name, func_name, 0 );
}
