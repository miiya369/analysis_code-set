//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Main part for fitting analysis
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <fitting.h>
#define PROJECT FITTING_DATA   // Project name

int main( int argc, char **argv ){
    
//================== arguments setting and error check ==================//
    if( argc == 1 ){ usage( PROJECT );   return 0; }
    char Argv[MAX_N_ARG][MAX_LEN_PATH];
    char data_list[MAX_N_DATA][MAX_LEN_PATH];
    if( set_arg( PROJECT, argc, argv, Argv, data_list ) == -1 ) return 0;
    
    time_t start_time, end_time;
    time( &start_time );
    
    bool endian_flg = false;
    double stp_cnd       = atof(Argv[0]);
    int    func_type     = name_to_N_func(Argv[1]);
    int    fit_range_min = atoi(Argv[2]);
    int    fit_range_max = atoi(Argv[3]);
    const char* infile   = Argv[4];
    const char* outfile  = Argv[5];
    double lattice_spacing = atof(Argv[7]);
    
    if(Argv[6][0] == 'y') endian_flg = true;
    
    int n_param = atoi(Argv[8]);
    double *param_initial = new double[n_param];
    for(int loop=0; loop<n_param; loop++)
        param_initial[loop] = atof(Argv[loop+9]);
    
    printf("\n @ Arguments set :\n");
    printf(" @ stop cond   = %lf\n",stp_cnd);
    printf(" @ func type   = %s\n",N_func_to_name(func_type).c_str());
    printf(" @ #. param    = %d\n @ param       = { ",n_param);
    for(int loop=0; loop<n_param; loop++) printf("%lf ",param_initial[loop]);
    printf("}\n @ range min   = %d\n",fit_range_min);
    printf(" @ range max   = %d\n",fit_range_max);
    printf(" @ lat spacing = %lf\n",lattice_spacing);
    printf(" @ endian cnv  = %s\n",Argv[6]);
    printf(" @ infile      = %s\n",infile);
    printf(" @ outfile     = %s\n\n",outfile);
    fflush(stdout);
//=======================================================================//
    
    FITTING *fit = new FITTING;
    
    fit->input_data( infile, endian_flg );
    fit->set_func( func_type, param_initial );
    fit->print_func_gnu( true, lattice_spacing );
    fit->fit_data( fit_range_min, fit_range_max, stp_cnd );
    fit->print_param();
    fit->print_func_gnu( false, lattice_spacing );
    fit->output_param( outfile );

    delete [] param_initial;
    delete fit;
    
    time( &end_time );
    printf("\n @ JOB END : ELAPSED TIME [s] = %d\n\n"
           ,(int)difftime(end_time,start_time) );
    return 0;
}
