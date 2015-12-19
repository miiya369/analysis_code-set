//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Function for convert the number of channel and hadron names
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <analysis.h>

string channel_to_name( int N_channel ){
    
    string hadron_name;
//================ particle base (S=0) ================//
    if( N_channel == PROTON_NEUTRON__PROTON_NEUTRON )
        hadron_name = "Pro-Neu_Pro-Neu";
//================ particle base (S=1) ================//
    else if( N_channel == PROTON_LAMBDA__PROTON_LAMBDA )
        hadron_name = "Pro-Lam_Pro-Lam";
    else if( N_channel == PROTON_LAMBDA__PROTON_SIGMAZ )
        hadron_name = "Pro-Lam_Pro-Sig0";
    else if( N_channel == PROTON_LAMBDA__NEUTRON_SIGMAP )
        hadron_name = "Pro-Lam_Neu-Sig+";
    else if( N_channel == PROTON_SIGMAZ__PROTON_LAMBDA )
        hadron_name = "Pro-Sig0_Pro-Lam";
    else if( N_channel == PROTON_SIGMAZ__PROTON_SIGMAZ )
        hadron_name = "Pro-Sig0_Pro-Sig0";
    else if( N_channel == PROTON_SIGMAZ__NEUTRON_SIGMAP )
        hadron_name = "Pro-Sig0_Neu-Sig+";
    else if( N_channel == NEUTRON_SIGMAP__PROTON_LAMBDA )
        hadron_name = "Neu-Sig+_Pro-Lam";
    else if( N_channel == NEUTRON_SIGMAP__PROTON_SIGMAZ )
        hadron_name = "Neu-Sig+_Pro-Sig0";
    else if( N_channel == NEUTRON_SIGMAP__NEUTRON_SIGMAP )
        hadron_name = "Neu-Sig+_Neu-Sig+";
//================ isospin base (I=1/2) ================//
    else if( N_channel == N_LAMBDA_12__N_LAMBDA_12 )
        hadron_name = "Nuc-Lam12_Nuc-Lam12";
    else if( N_channel == N_LAMBDA_12__N_SIGMA_12 )
        hadron_name = "Nuc-Lam12_Nuc-Sig12";
    else if( N_channel == N_SIGMA_12__N_LAMBDA_12 )
        hadron_name = "Nuc-Sig12_Nuc-Lam12";
    else if( N_channel == N_SIGMA_12__N_SIGMA_12 )
        hadron_name = "Nuc-Sig12_Nuc-Sig12";
//================ isospin base (I=3/2) ================//
    else if( N_channel == N_LAMBDA_12__N_SIGMA_32 )
        hadron_name = "Nuc-Lam12_Nuc-Sig32";
    else if( N_channel == N_SIGMA_32__N_LAMBDA_12 )
        hadron_name = "Nuc-Sig32_Nuc-Lam12";
    else if( N_channel == N_SIGMA_32__N_SIGMA_32 )
        hadron_name = "Nuc-Sig32_Nuc-Sig32";
    else
        hadron_name = "UNKNOWN";
    
    return hadron_name;
}

int name_to_channel( char* hadron_name ){
    
    int N_channel;
//================ particle base (S=0) ================//
    if( strcmp(hadron_name,"Pro-Neu_Pro-Neu" )==0 )
        N_channel = PROTON_NEUTRON__PROTON_NEUTRON;
//================ particle base (S=1) ================//
    else if( strcmp( hadron_name,"Pro-Lam_Pro-Lam" )==0 )
            N_channel = PROTON_LAMBDA__PROTON_LAMBDA;
    else if( strcmp( hadron_name,"Pro-Lam_Pro-Sig0" )==0 )
            N_channel = PROTON_LAMBDA__PROTON_SIGMAZ;
    else if( strcmp( hadron_name,"Pro-Lam_Neu-Sig+" )==0 )
            N_channel = PROTON_LAMBDA__NEUTRON_SIGMAP;
    else if( strcmp( hadron_name,"Pro-Sig0_Pro-lam" )==0 )
            N_channel = PROTON_SIGMAZ__PROTON_LAMBDA;
    else if( strcmp( hadron_name,"Pro-Sig0_Pro-Sig0" )==0 )
            N_channel = PROTON_SIGMAZ__PROTON_SIGMAZ;
    else if( strcmp( hadron_name,"Pro-Sig0_Neu-Sig+" )==0 )
            N_channel = PROTON_SIGMAZ__NEUTRON_SIGMAP;
    else if( strcmp( hadron_name,"Neu-Sig+_Pro-lam" )==0 )
            N_channel = NEUTRON_SIGMAP__PROTON_LAMBDA;
    else if( strcmp( hadron_name,"Neu-Sig+_Pro-Sig0" )==0 )
            N_channel = NEUTRON_SIGMAP__PROTON_SIGMAZ;
    else if( strcmp( hadron_name,"Neu-Sig+_Neu-Sig+" )==0 )
            N_channel = NEUTRON_SIGMAP__NEUTRON_SIGMAP;
//================ isospin base (I=1/2) ================//
    else if( strcmp( hadron_name,"Nuc-Lam12_Nuc-Lam12" )==0 )
            N_channel = N_LAMBDA_12__N_LAMBDA_12;
    else if( strcmp( hadron_name,"Nuc-Lam12_Nuc-Sig12" )==0 )
            N_channel = N_LAMBDA_12__N_SIGMA_12;
    else if( strcmp( hadron_name,"Nuc-Sig12_Nuc-Lam12" )==0 )
            N_channel = N_SIGMA_12__N_LAMBDA_12;
    else if( strcmp( hadron_name,"Nuc-Sig12_Nuc-Sig12" )==0 )
            N_channel = N_SIGMA_12__N_SIGMA_12;
//================ isospin base (I=3/2) ================//
    else if( strcmp( hadron_name,"Nuc-Lam12_Nuc-Sig32" )==0 )
            N_channel = N_LAMBDA_12__N_SIGMA_32;
    else if( strcmp( hadron_name,"Nuc-Sig32_Nuc-Lam12" )==0 )
            N_channel = N_SIGMA_32__N_LAMBDA_12;
    else if( strcmp( hadron_name,"Nuc-Sig32_Nuc-Sig32" )==0 )
            N_channel = N_SIGMA_32__N_SIGMA_32;
    else
        N_channel = -1;
    
    return N_channel;
}
