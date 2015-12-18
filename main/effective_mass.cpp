//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup potential
 * @brief   Main part for effective mass calculation
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <correlator.h>
#include <fitting.h>
#define PROJECT EFFECTIVEMASS    // Project name

int main( int argc, char **argv ){
    
//================== arguments setting and error check ==================//
    if( argc == 1 ){ usage( PROJECT );   return 0; }
    char Argv[MAX_N_ARG][MAX_LEN_PATH];
    char data_list[MAX_N_DATA][MAX_LEN_PATH];
    if( set_arg( PROJECT, argc, argv, Argv, data_list ) == -1 ) return 0;
    
    time_t start_time, end_time;
    time( &start_time );
    
    bool  calc_flg_fit = false;
    int   t_size             = atoi(Argv[0]);
    const char* conf_list    = Argv[1];
    const char* outfile_path = Argv[2];
    int   fit_range_min      = atoi(Argv[3]);
    int   fit_range_max      = atoi(Argv[4]);
    
    if(Argv[5][0] == 'y') calc_flg_fit = true;
    
    int  n_had       =  atoi(Argv[13]);
    int *hadron_type = new int[n_had];
    for(int loop=0; loop<n_had; loop++)
        hadron_type[loop] = name_to_hadron_type(Argv[loop+14]);
    
    int n_conf = set_data_list( conf_list,  data_list );
    
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
        printf("%s, ",had_type_to_name(hadron_type[loop]).c_str());
    printf("}\n @ conf list  = %s\n",conf_list);
    printf(" @ infile     = %s\n",data_list[MAIN_PATH]);
    printf(" @ outfile    = %s\n @\n",outfile_path);
    printf(" @ calc fit   = %s\n",Argv[5]);
    printf(" @ range min  = %d\n",fit_range_min);
    printf(" @ range max  = %d\n\n",fit_range_max);
    fflush(stdout);
    if( n_conf == 0 ){
        printf(" @@@@@@ No gauge configurations !\n\n");
        return 0;
    }
//=======================================================================//

    int xyz_size = 999;
    CORRELATOR *hadron = new CORRELATOR;
    FITTING *fit = new FITTING;
    string *relativ_type = new string[ n_had ];
    
    double param_ini[2] = { 0.001, 1.0 };
    char fit_data_name[MAX_LEN_PATH];
    double *eff_mass = new double[n_had*2];
    
    hadron->set_env( xyz_size, t_size, n_conf, data_list );
    
    for( int had_loop = 0; had_loop < n_had; had_loop++ ){
        if( hadron_type[had_loop] < MESON_BARYON_BOUNDARY ){
            relativ_type[had_loop] = "";
        }
        if( hadron_type[had_loop] > MESON_BARYON_BOUNDARY ){
            relativ_type[had_loop] = "_";
            relativ_type[had_loop] += data_list[SNK_RELA];
            relativ_type[had_loop] += "_";
            relativ_type[had_loop] += data_list[SRC_RELA];
        }
    }
    
    for( int had_loop = 0; had_loop < n_had; had_loop++ ){
        
        hadron->set_corr( hadron_type[had_loop] );
        hadron->make_JK_sample_corr(1);
        hadron->output_effmass_err( outfile_path );
        
        if( calc_flg_fit ){
            hadron->output_corr_fit("./tmp");
            snprintf( fit_data_name, sizeof(fit_data_name)
                     , "./tmp/%s%s_corr_fitdata"
                     , had_type_to_name(hadron_type[had_loop]).c_str()
                     , relativ_type[had_loop].c_str() );
            fit->input_data( fit_data_name, false );
            fit->set_func( ONE_EXPONENTIAL, param_ini );
            fit->fit_data( fit_range_min, fit_range_max, 0.000001 );
            eff_mass[0+2*had_loop] = fit->param_mean[1];
            eff_mass[1+2*had_loop] = fit->param_err[1];
            fit->reset_func();
            fit->delete_data();
        }
        hadron->delete_corr();
    }
    delete fit;
    delete hadron;
    delete [] relativ_type;
    
    if( calc_flg_fit ){
        printf("\n @ ============================================= @\n");
        printf(" @ EFFECTIVE MASS FITTING RESULTS (Lattice Unit) @\n");
        printf(" @ ============================================= @\n\n");
        for( int had_loop = 0; had_loop < n_had; had_loop++ )
            printf("     %s : %lf +/- %lf \n"
                   , had_type_to_name(hadron_type[had_loop]).c_str(),
                   eff_mass[0+2*had_loop], eff_mass[1+2*had_loop]);
        printf("\n @ ============================================= @\n");
    }
    delete [] hadron_type;
    delete [] eff_mass;
    
    time( &end_time );
    printf("\n @ JOB END : ELAPSED TIME [s] = %d\n\n"
           ,(int)difftime(end_time,start_time) );
    return 0;
}
