//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup potential
 * @brief   Main part for potential calculation
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <potential.h>
#define PROJECT CALC_POTENTIAL   // Project name

int main( int argc, char **argv ){
    
//================== arguments setting and error check ==================//
    if( argc == 1 ){ usage( PROJECT );   return 0; }
    char Argv[MAX_N_ARG][MAX_LEN_PATH];
    char data_list[MAX_N_DATA][MAX_LEN_PATH];
    if( set_arg( PROJECT, argc, argv, Argv, data_list ) == -1 ) return 0;
    
    time_t start_time, end_time;
    time( &start_time );
    
    bool endian_flg   = false, calc_flg_pot = false, calc_flg_lap = false;
    bool calc_flg_t1  = false, calc_flg_t2  = false, calc_flg_fit = false;
    bool calc_flg_NBS = false, calc_flg_Rcorr = false;
    int t_size               = atoi(Argv[21]);
    int xyz_size             = atoi(Argv[0]);
    int time_min             = atoi(Argv[1]);
    int time_max             = atoi(Argv[2]);
    int channel              = name_to_channel(Argv[3]);
    double mass              = atof(Argv[4]);
    int spin                 = atoi(Argv[5]);
    int spin_z               = atoi(Argv[6]);
    int ang_mom              = atoi(Argv[7]);
    const char* conf_list    = Argv[8];
    const char* outfile_path = Argv[9];
    
    if(Argv[10][0] == 'y') endian_flg     = true;
    if(Argv[11][0] == 'y') calc_flg_pot   = true;
    if(Argv[12][0] == 'y') calc_flg_lap   = true;
    if(Argv[13][0] == 'y') calc_flg_t1    = true;
    if(Argv[14][0] == 'y') calc_flg_t2    = true;
    if(Argv[15][0] == 'y') calc_flg_fit   = true;
    if(Argv[24][0] == 'y') calc_flg_NBS   = true;
    if(Argv[25][0] == 'y') calc_flg_Rcorr = true;

    int n_conf = set_data_list( conf_list,  data_list );
    
    printf("\n @ Arguments set :\n");
    printf(" @ #. conf    = %d\n",n_conf);
    printf(" @ t_size     = %d\n",t_size);
    printf(" @ xyz_size   = %d\n",xyz_size);
    printf(" @ t shift    = %s\n",data_list[N_T_SHIFT]);
    printf(" @ x shift    = %s\n",data_list[N_X_SHIFT]);
    printf(" @ y shift    = %s\n",data_list[N_Y_SHIFT]);
    printf(" @ z shift    = %s\n",data_list[N_Z_SHIFT]);
    printf(" @ snk rela   = %s\n",data_list[SNK_RELA]);
    printf(" @ src rela   = %s\n",data_list[SRC_RELA]);
    printf(" @ time_min   = %d\n",time_min);
    printf(" @ time_max   = %d\n",time_max);
    printf(" @ channel    = %s\n",channel_to_name(channel).c_str());
    printf(" @ mass       = %lf\n",mass);
    printf(" @ spin       = %d\n",spin);
    printf(" @ spin z cmp = %d\n",spin_z);
    printf(" @ ang mom    = %d\n",ang_mom);
    printf(" @ conf list  = %s\n",conf_list);
    printf(" @ infile     = %s\n",data_list[MAIN_PATH]);
    printf(" @ outfile    = %s\n",outfile_path);
    printf(" @ endian cnv = %s\n",Argv[10]);
    printf(" @ out pot    = %s\n",Argv[11]);
    printf(" @ out lap    = %s\n",Argv[12]);
    printf(" @ out t1 dif = %s\n",Argv[13]);
    printf(" @ out t2 dif = %s\n",Argv[14]);
    printf(" @ out fit    = %s\n",Argv[15]);
    printf(" @ out NBS    = %s\n",Argv[24]);
    printf(" @ out Rcorr  = %s\n\n",Argv[25]);
    fflush(stdout);
    if( n_conf == 0 ){
        printf(" @@@@@@ No gauge configurations !\n\n");
        return 0;
    }
//=======================================================================//
    
    POTENTIAL *pot = NULL;
    R_CORRELATOR *Rcorrelator = NULL;
    
    if( calc_flg_fit||calc_flg_pot||calc_flg_lap||calc_flg_t1||calc_flg_t2){
        
        pot = new POTENTIAL;
        pot->set_env( xyz_size, t_size, n_conf, data_list );
    }
    if( calc_flg_NBS || calc_flg_Rcorr ){
        
        Rcorrelator = new R_CORRELATOR;
        Rcorrelator->set_env( xyz_size, t_size, n_conf, data_list );
    }
    
    for( int it=time_min; it<=time_max; it++ ){

        if( calc_flg_fit||calc_flg_pot||calc_flg_lap||calc_flg_t1||calc_flg_t2)
            pot->set_pot( channel, it, endian_flg, spin, ang_mom, mass );
        
        if(calc_flg_fit){
            pot->calc_pot_kernel();
            pot->output_single_pot_fit( outfile_path );
        }
        if(calc_flg_pot){
            if(!calc_flg_fit) pot->calc_pot_kernel();
            pot->output_single_pot_err( outfile_path );
        }
        if(calc_flg_lap){
            pot->calc_laplacian();
            pot->output_single_pot_err( outfile_path );
        }
        if(calc_flg_t1){
            pot->calc_1st_timediff();
            pot->output_single_pot_err( outfile_path );
        }
        if(calc_flg_t2){
            pot->calc_2nd_timediff();
            pot->output_single_pot_err( outfile_path );
        }
        
        if( calc_flg_NBS ){
            
            Rcorrelator->set_NBS( channel, it, endian_flg );
            Rcorrelator->projection_NBS( spin, ang_mom );
            Rcorrelator->make_JK_sample_NBS(1);
            Rcorrelator->output_NBS_err( outfile_path );
            Rcorrelator->delete_NBS();
        }
        if( calc_flg_Rcorr ){
            
            Rcorrelator->set_Rcorr( channel, it, endian_flg, spin, ang_mom );
            Rcorrelator->output_Rcorr_err( outfile_path );
            Rcorrelator->delete_Rcorr();
        }
    }
    if( calc_flg_fit||calc_flg_pot||calc_flg_lap||calc_flg_t1||calc_flg_t2)
        delete pot;
    if( calc_flg_NBS || calc_flg_Rcorr )
        delete Rcorrelator;
    
    time( &end_time );
    printf("\n @ JOB END : ELAPSED TIME [s] = %d\n\n"
           ,(int)difftime(end_time,start_time) );
    return 0;
}
