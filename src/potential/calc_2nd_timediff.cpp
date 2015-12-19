//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Calculate 2nd time-difference term of potential
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <potential.h>

void POTENTIAL::calc_2nd_timediff(){
    
    func_name = "calc_2nd_timediff_____";
    route( class_name, func_name, 1 );
    
    cdouble *Rcorr_ptr1 = NULL;
    cdouble *Rcorr_ptr2 = NULL;
    cdouble *Rcorr_ptr3 = NULL;
    
    for( int ttt=0; ttt<3; ttt++ ){
        if( Rcorr_t[ttt] == time_slice-1 )
            Rcorr_ptr1 = Rcorr[ttt].Rcorr;
        else if( Rcorr_t[ttt] == time_slice )
            Rcorr_ptr2 = Rcorr[ttt].Rcorr;
        else if( Rcorr_t[ttt] == time_slice+1 )
            Rcorr_ptr3 = Rcorr[ttt].Rcorr;
    }
    
    for(int i=0; i<N_conf; i++)
        for(int z=0; z<xyzSIZE; z++)
            for(int y=0; y<xyzSIZE; y++)
                for(int x=0; x<xyzSIZE; x++)
                    
                    potential[ xyzn(x,y,z,i) ] = ( Rcorr_ptr1[ xyzn(x,y,z,i) ]
                                                 + Rcorr_ptr3[ xyzn(x,y,z,i) ]
                                                 - Rcorr_ptr2[ xyzn(x,y,z,i) ]
                                                    * 2.0) / ( 4.0* reduced_mass );
    
    printf(" @ Finished calculate potential time 2nd difference part : %s, t=%d\n"
           , channel_to_name(channel).c_str(), time_slice);
    potential_type = "potT2term";
    
    route( class_name, func_name, 0 );
}
