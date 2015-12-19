//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup potential
 * @brief   Main part for calculate coupled channel potential
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <potential.h>
#define PROJECT COUPLED_CHANNEL_POT   // Project name

int main( int argc, char **argv ){
    
//================== arguments setting and error check ==================//
    if( argc == 1 ){ usage( PROJECT );   return 0; }
    char Argv[MAX_N_ARG][MAX_LEN_PATH];
    char data_list[MAX_N_DATA][MAX_LEN_PATH];
    if( set_arg( PROJECT, argc, argv, Argv, data_list ) == -1 ) return 0;
    
    time_t start_time, end_time, start_time2;
    time( &start_time );
    
    bool endian_flg  = false, calc_fit_flg = false, flg_for_miyamoto = false;
    int    channel[4];
    double mass[4];
    
    int t_size               = atoi(Argv[0]);
    int xyz_size             = atoi(Argv[1]);
    int time_slice           = atoi(Argv[2]);
    int spin                 = atoi(Argv[3]);
    int spin_z               = atoi(Argv[4]);
    int ang_mom              = atoi(Argv[5]);
    const char* conf_list    = Argv[6];
    const char* outfile_path = Argv[7];
    
#define idx(Row,Colomn) (Colomn+ 2* Row)
    for(int R=0; R<2; R++) for(int C=0; C<2; C++){
        channel[idx(R,C)] = name_to_channel(Argv[8+idx(R,C)]);
        mass[idx(R,C)]    = atof(Argv[12+idx(R,C)]);
    }
    if(Argv[21][0] == 'y') endian_flg    = true;
    if(Argv[22][0] == 'y') calc_fit_flg  = true;
    if(Argv[25][0] == 'y') flg_for_miyamoto = true;
    
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
    printf(" @ spin       = %d\n",spin);
    printf(" @ spin z cmp = %d\n",spin_z);
    printf(" @ ang mom    = %d\n",ang_mom);
    for(int R=0; R<2; R++) for(int C=0; C<2; C++){
        printf(" @ channel %d%d  = %s\n"
               ,R,C,channel_to_name(channel[idx(R,C)]).c_str());
        printf(" @ mass    %d%d  = %lf\n"
               ,R,C,mass[idx(R,C)]);
    }
    printf(" @ conf list  = %s\n",conf_list);
    printf(" @ infile     = %s\n",data_list[MAIN_PATH]);
    printf(" @ outfile    = %s\n @\n",outfile_path);
    printf(" @ endian cnv = %s\n",Argv[21]);
    printf(" @ calc fit   = %s\n\n",Argv[22]);
    fflush(stdout);
    if( n_conf == 0 ){
        printf(" @@@@@@ No gauge configurations !\n\n");
        return 0;
    }
//=======================================================================//
    
    POTENTIAL *pot = new POTENTIAL[4];
    int n_size = xyz_size * xyz_size * xyz_size * n_conf;
    cdouble *R_corr_ptr[4];
    
    for(int i=0; i<4; i++){
        pot[i].set_env( xyz_size, t_size, n_conf, data_list );
        
        if( !flg_for_miyamoto ){
            pot[i].set_pot(  channel[i], time_slice
                                      , endian_flg, spin, ang_mom
                                      , mass[i] );
            pot[i].calc_pot_kernel();
            
            pot[i].Rcorr[1].output_Rcorr_all("./tmp");
            pot[i].Rcorr[1].output_Rcorr_err("./tmp");
            pot[i].output_single_pot_err("./tmp");
            pot[i].output_couple_pot_all("./tmp");
            R_corr_ptr[i] = pot[i].Rcorr[1].Rcorr;

            pot[i].delete_pot_corr();
            pot[i].Rcorr[0].delete_Rcorr();
            pot[i].Rcorr[2].delete_Rcorr();
        }else{
            pot[i].set_pot_from_binary(  "./tmp", channel[i]
                                              , time_slice, endian_flg );
            R_corr_ptr[i] = pot[i].Rcorr->Rcorr;
        }
    }

    cdouble *coupled_channel_pot = new cdouble[ n_size * 4 ];
    cdouble R_corr_inv[4];
    cdouble R_corr_det;
    
    time( &start_time2 );
    printf(" @ calculating coupled channel potental |   0%%");
    for(int loop=0; loop<n_size; loop++){
        
        R_corr_det = (  R_corr_ptr[idx(0,0)][loop]
                      * R_corr_ptr[idx(1,1)][loop]
                      - R_corr_ptr[idx(0,1)][loop]
                      * R_corr_ptr[idx(1,0)][loop] );
        
        R_corr_inv[idx(0,0)] =  R_corr_ptr[idx(1,1)][loop] / R_corr_det;
        R_corr_inv[idx(1,0)] = -R_corr_ptr[idx(1,0)][loop] / R_corr_det;
        R_corr_inv[idx(0,1)] = -R_corr_ptr[idx(0,1)][loop] / R_corr_det;
        R_corr_inv[idx(1,1)] =  R_corr_ptr[idx(0,0)][loop] / R_corr_det;
        
        for(int R=0; R<2; R++) for(int C=0; C<2; C++) for(int k=0; k<2; k++)
            coupled_channel_pot[loop + n_size * idx(R,C)]
             += pot[idx(R,k)].potential[loop] * R_corr_inv[idx(k,C)];
#undef idx
        printf("\b\b\b\b%3.0lf%%",double(loop+1)/double(n_size)*100);
        fflush(stdout);
    }
    printf("\n");
    time( &end_time );
    printf(
    " @ Coupled channel potential has been calculated : ELAPSED TIME [s] = %d\n"
           ,(int)difftime(end_time,start_time2) );
    
    for(int i=0; i<4; i++)
            for(int loop=0; loop<n_size; loop++)
                pot[i].potential[loop]
                    = coupled_channel_pot[loop + n_size * i];
    delete [] coupled_channel_pot;
    
    for(int i=0; i<4; i++){
        pot[i].output_couple_pot_err( outfile_path );
        if( calc_fit_flg )
            pot[i].output_couple_pot_fit( outfile_path );
    }
    delete [] pot;
    
    time( &end_time );
    printf("\n @ JOB END : ELAPSED TIME [s] = %d\n\n"
           ,(int)difftime(end_time,start_time) );
    return 0;
}
