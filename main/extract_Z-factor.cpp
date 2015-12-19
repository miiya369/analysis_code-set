//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup potential
 * @brief   Main part for extract Z-factor from PS and SS correlator
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <correlator.h>
#define PROJECT EXTRACT_Z_FACTOR    // Project name

int main( int argc, char **argv ){
    
//================== arguments setting and error check ==================//
    if( argc == 1 ){ usage( PROJECT );   return 0; }
    char Argv[MAX_N_ARG][MAX_LEN_PATH];
    char data_list[MAX_N_DATA][MAX_LEN_PATH];
    if( set_arg( PROJECT, argc, argv, Argv, data_list ) == -1 ) return 0;
    
    time_t start_time, end_time;
    time( &start_time );
    
    int   t_size             = atoi(Argv[3]);
    const char* conf_list    = Argv[0];
    const char* outfile_path = Argv[2];
    
    int n_conf = set_data_list( conf_list,  data_list );
    
    int  n_had       =  atoi(Argv[10]);
    int *hadron_type = new int[n_had];
    for(int loop=0; loop<n_had; loop++)
        hadron_type[loop] = name_to_hadron_type(Argv[loop+11]);
    
    printf("\n @ Arguments set :\n");
    printf(" @ #. conf    = %d\n",n_conf);
    printf(" @ t_size     = %d\n",t_size);
    printf(" @ t shift    = %s\n",data_list[N_T_SHIFT]);
    printf(" @ x shift    = %s\n",data_list[N_X_SHIFT]);
    printf(" @ y shift    = %s\n",data_list[N_Y_SHIFT]);
    printf(" @ z shift    = %s\n",data_list[N_Z_SHIFT]);
    printf(" @ snk rela   = %s\n",data_list[SNK_RELA]);
    printf(" @ src rela   = %s\n",data_list[SRC_RELA]);
    printf(" @ #. hadron  = %d\n @ hadron     = { ",n_had);
    for(int loop=0; loop<n_had; loop++)
        printf("%s ",had_type_to_name(hadron_type[loop]).c_str());
    printf("}\n @ conf list  = %s\n",conf_list);
    printf(" @ infile     = %s\n",data_list[MAIN_PATH]);
    printf(" @ outfile    = %s\n @\n",outfile_path);
    fflush(stdout);
    if( n_conf == 0 ){
        printf(" @@@@@@ No gauge configurations !\n\n");
        return 0;
    }
//=======================================================================//

    int xyz_size = 999;
    CORRELATOR *hadron = new CORRELATOR;
    
    hadron->set_env( xyz_size, t_size, n_conf, data_list );
    
    for( int had_loop=0; had_loop<n_had; had_loop++ ){
        
        hadron->set_corr( hadron_type[had_loop] );
        hadron->calc_z_factor();
        hadron->output_z_factor_err( outfile_path );
        hadron->delete_corr();
    }
    delete hadron;
    
    time( &end_time );
    printf("\n @ JOB END : ELAPSED TIME [s] = %d\n\n"
           ,(int)difftime(end_time,start_time) );
    return 0;
}
