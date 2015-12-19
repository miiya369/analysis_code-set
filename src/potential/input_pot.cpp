//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Functions for input for potential
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <potential.h>

void POTENTIAL::input_pot(){
    
    func_name = "input_pot_NBS/corr____";
    route( class_name, func_name, 1 );
    
    if( potential == NULL )
        potential = new cdouble[xyzSIZE* xyzSIZE* xyzSIZE* N_conf];
    
    if( corr1 == NULL ){
        corr1 = new cdouble[ tSIZE * N_conf ];
        corr2 = new cdouble[ tSIZE * N_conf ];
        
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
    if( Rcorr == NULL ){
        Rcorr = new R_CORRELATOR[3];
        for( int ttt=0; ttt<3; ttt++ ){
            Rcorr[ttt].set_env( xyzSIZE, tSIZE, N_conf, data_list );
            Rcorr[ttt].set_Rcorr(channel,time_slice+(ttt-1),endian_flg
                             ,spin,ang_mom,corr1,corr2);
            Rcorr_t[ttt] = Rcorr[ttt].info_time();
        }
    }
    
    for( int ttt=0; ttt<3; ttt++ ) read_time_slice_flg[ttt] = true;
    for( int ttt=0; ttt<3; ttt++ ){
        Rcorr_reread_flg[ttt] = false;
        
        if( Rcorr_t[ttt] == time_slice-1 )
            read_time_slice_flg[0] = false;
        else if( Rcorr_t[ttt] == time_slice )
            read_time_slice_flg[1] = false;
        else if( Rcorr_t[ttt] == time_slice+1 )
            read_time_slice_flg[2] = false;
        else
            Rcorr_reread_flg[ttt] = true;
    }
    for( int ttt=0; ttt<3; ttt++ )
        if( read_time_slice_flg[ttt] )
            for( int tt=0; tt<3; tt++ )
                if( Rcorr_reread_flg[tt] ){
                    Rcorr[tt].delete_Rcorr();
                    Rcorr[tt].set_Rcorr(channel,time_slice+(ttt-1),endian_flg
                                         ,spin,ang_mom,corr1,corr2);
                    Rcorr_t[tt] = Rcorr[tt].info_time();
                    Rcorr_reread_flg[tt] = false;
                }
    
    route( class_name, func_name, 0 );
}

void POTENTIAL::input_pot( double HAD1_mass, double HAD2_mass ){
    
    func_name = "input_pot_NBS/mass____";
    route( class_name, func_name, 1 );
    
    if( potential == NULL )
        potential = new cdouble[xyzSIZE* xyzSIZE* xyzSIZE* N_conf];
    
    if( Rcorr == NULL ){
        Rcorr = new R_CORRELATOR[3];
        for( int ttt=0; ttt<3; ttt++ ){
            Rcorr[ttt].set_env( xyzSIZE, tSIZE, N_conf, data_list );
            Rcorr[ttt].set_Rcorr(channel,time_slice+(ttt-1),endian_flg
                                 ,spin,ang_mom,HAD1_mass,HAD2_mass);
            Rcorr_t[ttt] = Rcorr[ttt].info_time();
        }
    }
    
    for( int ttt=0; ttt<3; ttt++ ) read_time_slice_flg[ttt] = true;
    for( int ttt=0; ttt<3; ttt++ ){
        Rcorr_reread_flg[ttt] = false;
        
        if( Rcorr_t[ttt] == time_slice-1 )
            read_time_slice_flg[0] = false;
        else if( Rcorr_t[ttt] == time_slice )
            read_time_slice_flg[1] = false;
        else if( Rcorr_t[ttt] == time_slice+1 )
            read_time_slice_flg[2] = false;
        else
            Rcorr_reread_flg[ttt] = true;
    }
    for( int ttt=0; ttt<3; ttt++ )
        if( read_time_slice_flg[ttt] )
            for( int tt=0; tt<3; tt++ )
                if( Rcorr_reread_flg[tt] ){
                    Rcorr[tt].delete_Rcorr();
                    Rcorr[tt].set_Rcorr(channel,time_slice+(ttt-1),endian_flg
                                        ,spin,ang_mom,HAD1_mass,HAD2_mass);
                    Rcorr_t[tt] = Rcorr[tt].info_time();
                    Rcorr_reread_flg[tt] = false;
                }
    
    route( class_name, func_name, 0 );
}
