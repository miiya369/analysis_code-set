//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Functions for input for potential from binary file
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <potential.h>

void POTENTIAL::set_pot_from_binary(  const char* infile_PATH, int channel
                                    , int time_slice, bool endian_FLG ){
    
    func_name = "set_pot_from_binary___";
    route( class_name, func_name, 1 );
    
    if( potential == NULL )
        potential = new cdouble[xyzSIZE* xyzSIZE* xyzSIZE* N_conf];
    if( Rcorr != NULL ) delete_pot();
    
    Rcorr = new R_CORRELATOR;
    Rcorr->Rcorr = new cdouble[xyzSIZE* xyzSIZE* xyzSIZE* N_conf];
    
    string hadron_name = channel_to_name(channel);
    char infile1_name[1024];
    char infile2_name[1024];
    snprintf( infile1_name, sizeof(infile1_name),
             "%s/%s_potential_ccp_all_t%d"
             , infile_PATH, hadron_name.c_str(), time_slice );
    snprintf( infile2_name, sizeof(infile2_name),
             "%s/%s_Rcorr_all_t%d"
             , infile_PATH, hadron_name.c_str(), time_slice );
    
    ifstream infile1( infile1_name, ios::in | ios::binary );
    ifstream infile2( infile2_name, ios::in | ios::binary );
    
    for(int z=0; z<xyzSIZE; z++)
        for(int y=0; y<xyzSIZE; y++)
            for(int x=0; x<xyzSIZE; x++)
                for(int i=0; i<N_conf; i++){
                    infile1.read( (char*)&potential[ xyzn(x,y,z,i) ]
                                  , sizeof(cdouble) );
                    
                    infile2.read( (char*)&Rcorr->Rcorr[ xyzn(x,y,z,i) ]
                                 , sizeof(cdouble) );
                }
    infile1.close();
    infile2.close();
    
    if( endian_FLG ){
        endian_convert( potential   , xyzSIZE*xyzSIZE*xyzSIZE*N_conf );
        endian_convert( Rcorr->Rcorr, xyzSIZE*xyzSIZE*xyzSIZE*N_conf );
    }
    
    func_name = "set_pot_from_binary___";
    route( class_name, func_name, 0 );
}
