//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Calculate potential kernel of R-correlator
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <potential.h>

void POTENTIAL::calc_pot_kernel(){
    
    func_name = "calc_pot_kernel_______";
    route( class_name, func_name, 1 );
    
    if( !new_flg_pot ){
        potential = new cdouble[xyzSIZE* xyzSIZE* xyzSIZE* N_conf];
        new_flg_pot = true;
    }
    if( !new_flg_corr ){
        corr1 = new cdouble[ tSIZE * N_conf ];
        corr2 = new cdouble[ tSIZE * N_conf ];
        new_flg_corr = true;
        
        CORRELATOR *corr_org = new CORRELATOR;
        corr_org->set_env( xyzSIZE, tSIZE, N_conf, data_list );
        
        if( HAD1_type == HAD2_type ){   // when used same baryon
            corr_org->set_corr( HAD1_type );
            corr_org->make_JK_sample_corr(1);
            for(int it=0; it<tSIZE; it++) for(int conf=0; conf<N_conf; conf++){
                corr1[ nt(conf,it) ] = corr_org->corr[ nt(conf,it) ];
                corr2[ nt(conf,it) ] = corr_org->corr[ nt(conf,it) ];
            }
        }else{   // when used different baryon
            corr_org->set_corr( HAD1_type );
            corr_org->make_JK_sample_corr(1);
            for(int it=0; it<tSIZE; it++) for(int conf=0; conf<N_conf; conf++)
                corr1[ nt(conf,it) ] = corr_org->corr[ nt(conf,it) ];
            corr_org->delete_corr();
            corr_org->set_corr( HAD2_type );
            corr_org->make_JK_sample_corr(1);
            for(int it=0; it<tSIZE; it++) for(int conf=0; conf<N_conf; conf++)
                corr2[ nt(conf,it) ] = corr_org->corr[ nt(conf,it) ];
        }
        delete corr_org;
    }
    if( !new_flg_Rcorr1 ){
        Rcorr1 = new R_CORRELATOR;
        new_flg_Rcorr1 = true;
        Rcorr1->set_env( xyzSIZE, tSIZE, N_conf, data_list );
        Rcorr1->set_Rcorr(channel,time_slice,endian_flg
                          ,spin,ang_mom,corr1,corr2);
    }
    if( !new_flg_Rcorr2 ){
        Rcorr2 = new R_CORRELATOR;
        new_flg_Rcorr2 = true;
        Rcorr2->set_env( xyzSIZE, tSIZE, N_conf, data_list );
        Rcorr2->set_Rcorr(channel,time_slice+1,endian_flg
                          ,spin,ang_mom,corr1,corr2);
    }
    if( !new_flg_Rcorr3 ){
        Rcorr3 = new R_CORRELATOR;
        new_flg_Rcorr3 = true;
        Rcorr3->set_env( xyzSIZE, tSIZE, N_conf, data_list );
        Rcorr3->set_Rcorr(channel,time_slice-1,endian_flg
                          ,spin,ang_mom,corr1,corr2);
    }
    
    for(int i=0; i<N_conf; i++)
        for(int z=0; z<xyzSIZE; z++)
            for(int y=0; y<xyzSIZE; y++)
                for(int x=0; x<xyzSIZE; x++){
            
                    int xSHIFT = 0, ySHIFT = 0, zSHIFT = 0;
                    if( x==0 ) xSHIFT = xyzSIZE;
                    if( y==0 ) ySHIFT = xyzSIZE;
                    if( z==0 ) zSHIFT = xyzSIZE;
                    // It's using due to periodic b.c.
                        potential[ xyzn(x,y,z,i) ] =
            
            ( Rcorr1->Rcorr[ xyzn((x+1)%xyzSIZE,y,z,i) ]   /* Laplacian part */
            + Rcorr1->Rcorr[ xyzn(x-1+xSHIFT,y,z,i) ]
            + Rcorr1->Rcorr[ xyzn(x,(y+1)%xyzSIZE,z,i) ]
            + Rcorr1->Rcorr[ xyzn(x,y-1+ySHIFT,z,i) ]
            + Rcorr1->Rcorr[ xyzn(x,y,(z+1)%xyzSIZE,i) ]
            + Rcorr1->Rcorr[ xyzn(x,y,z-1+zSHIFT,i) ]
            - Rcorr1->Rcorr[ xyzn(x,y,z,i) ] * 6.0 ) / ( 2.0* mass )
            +
            ( Rcorr3->Rcorr[ xyzn(x,y,z,i) ]   /* time 1st difference part */
            - Rcorr2->Rcorr[ xyzn(x,y,z,i) ] ) * 0.5
            +
            ( Rcorr2->Rcorr[ xyzn(x,y,z,i) ]   /* time 2nd difference part */
            + Rcorr3->Rcorr[ xyzn(x,y,z,i) ]
            - Rcorr1->Rcorr[ xyzn(x,y,z,i) ] * 2.0 ) / ( 4.0* mass );
                }
    printf(" @ potential kernel ( t=%d ) has been calculated.\n", time_slice);
    printf(" @ Channel = %s\n", channel_to_name(channel).c_str());
    potential_type = "potential";
    
    route( class_name, func_name, 0 );
}
