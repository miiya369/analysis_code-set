//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup observable
 * @brief   Main part for observable
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <phase_shift.h>
#define PROJECT CALC_OBSERVABLE   // Project name

int main( int argc, char **argv ){
    
//================== arguments setting and error check ==================//
    if( argc == 1 ){ usage( PROJECT );   return 0; }
    char Argv[MAX_N_ARG][MAX_LEN_PATH];
    char data_list[MAX_N_DATA][MAX_LEN_PATH];
    if( set_arg( PROJECT, argc, argv, Argv, data_list ) == -1 ) return 0;
    
    time_t start_time, end_time;
    time( &start_time );
    
    bool endian_flg = false, test_flg   = false, calc_scatt_len_flg = false;
    double lattice_space     = atof(Argv[0]);
    double mass              = atof(Argv[1]);
    double ang_mom           = atof(Argv[2]);
    int    E_min             = atoi(Argv[3]);
    int    E_max             = atoi(Argv[4]);
    double delta             = atof(Argv[5]);
    double max_r             = atof(Argv[6]);
    double V_0               = atof(Argv[7]);
    double r_0               = atof(Argv[8]);
    const char* infile       = Argv[9];
    const char* outfile      = Argv[10];
    
    if(Argv[11][0] == 'y') endian_flg         = true;
    if(Argv[12][0] == 'y') test_flg           = true;
    if(Argv[13][0] == 'y') calc_scatt_len_flg = true;
    
    printf("\n @ Arguments set :\n");
    printf(" @ lat space  = %lf\n",lattice_space);
    printf(" @ mass       = %lf\n",mass);
    printf(" @ ang mom    = %lf\n",ang_mom);
    printf(" @ E min      = %d\n",E_min);
    printf(" @ E max      = %d\n",E_max);
    printf(" @ delta      = %lf\n",delta);
    printf(" @ max_r      = %lf\n",max_r);
    printf(" @ endian cnv = %s\n",Argv[11]);
    printf(" @ infile     = %s\n",infile);
    printf(" @ outfile    = %s\n @\n",outfile);
    printf(" @ calc test      = %s\n",Argv[12]);
    printf(" @ V_0 (For test) = %lf\n",V_0);
    printf(" @ r_0 (For test) = %lf\n @\n",r_0);
    printf(" @ calc sctt len  = %s\n\n",Argv[13]);
    fflush(stdout);
//=======================================================================//
    
    PHASE_SHIFT *phase = new PHASE_SHIFT;
    char scatt_length_filename[1024];
    
    if( test_flg ){
        
        phase->set_test( r_0, V_0, mass, delta, max_r );
        phase->calc_phase_shift( E_min, E_max );
        phase->output_phase_shift_err( outfile );
        
        if( calc_scatt_len_flg ){
            phase->calc_scatt_len();
            phase->output_scatt_len_err("scattering_length");
        }
    }
    else{
        
        phase->input_param( infile, endian_flg );
        phase->set_env( lattice_space, mass, ang_mom, delta, max_r);
        phase->calc_phase_shift( E_min, E_max );
        phase->output_phase_shift_err( outfile );
        
        if( calc_scatt_len_flg ){
            snprintf(scatt_length_filename, sizeof(scatt_length_filename)
                     ,"%s_scattering_length",outfile);
            phase->calc_scatt_len();
            phase->output_scatt_len_err(scatt_length_filename);
        }
    }
    delete phase;
    
    time( &end_time );
    printf("\n @ JOB END : ELAPSED TIME [s] = %d\n\n"
           ,(int)difftime(end_time,start_time) );
    return 0;
}
