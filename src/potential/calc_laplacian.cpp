//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Calculate laplacian term of potential
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <potential.h>

void POTENTIAL::calc_laplacian(){
    
    func_name = "calc_laplacian________";
    route( class_name, func_name, 1 );
    
    cdouble *Rcorr_ptr = NULL;
    
    for( int ttt=0; ttt<3; ttt++ )
        if( Rcorr_t[ttt] == time_slice )
            Rcorr_ptr = Rcorr[ttt].Rcorr;
    
    int xSHIFT, ySHIFT, zSHIFT;
    for(int i=0; i<N_conf; i++)
        for(int z=0; z<xyzSIZE; z++)
            for(int y=0; y<xyzSIZE; y++)
                for(int x=0; x<xyzSIZE; x++){
            
                    xSHIFT = 0; ySHIFT = 0; zSHIFT = 0;
                    if( x==0 ) xSHIFT = xyzSIZE;
                    if( y==0 ) ySHIFT = xyzSIZE;
                    if( z==0 ) zSHIFT = xyzSIZE;
                    // It's using due to periodic b.c.
                    potential[ xyzn(x,y,z,i) ]
                     = (  Rcorr_ptr[ xyzn((x+1)%xyzSIZE,y,z,i) ]
                        + Rcorr_ptr[ xyzn(x-1+xSHIFT,y,z,i) ]
                        + Rcorr_ptr[ xyzn(x,(y+1)%xyzSIZE,z,i) ]
                        + Rcorr_ptr[ xyzn(x,y-1+ySHIFT,z,i) ]
                        + Rcorr_ptr[ xyzn(x,y,(z+1)%xyzSIZE,i) ]
                        + Rcorr_ptr[ xyzn(x,y,z-1+zSHIFT,i) ]
                        - Rcorr_ptr[ xyzn(x,y,z,i) ] * 6.0 ) / ( 2.0* mass );
                }
    printf(" @ Finished calculate potential laplacian part           : %s, t=%d\n"
           , channel_to_name(channel).c_str(), time_slice);
    potential_type = "potLapterm";
    
    route( class_name, func_name, 0 );
}
