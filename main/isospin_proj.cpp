//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup potential
 * @brief   Main part for isospin projection for NBS wave function
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <analysis.h>
#include <sys/stat.h>
#define PROJECT ISOSPIN_PROJECTION   // Project name

int set_dir( char*, int, int );

int main( int argc, char **argv ){
    
//================== arguments setting and error check ==================//
    if( argc == 1 ){ usage( PROJECT );   return 0; }
    char Argv[MAX_N_ARG][MAX_LEN_PATH];
    char data_list[MAX_N_DATA][MAX_LEN_PATH];
    if( set_arg( PROJECT, argc, argv, Argv, data_list ) == -1 ) return 0;
    
    time_t start_time, start_time2, end_time;
    time( &start_time );
    
    bool endian_flg  = false;
    int xyz_size          = atoi(Argv[0]);
    int time_min          = atoi(Argv[1]);
    int time_max          = atoi(Argv[2]);
    const char* conf_list = Argv[3];
    
    if(Argv[4][0] == 'y') endian_flg = true;
    
    int n_conf = set_data_list( conf_list,  data_list );
    
    printf("\n @ Arguments set :\n");
    printf(" @ #. conf    = %d\n",n_conf);
    printf(" @ xyz_size   = %d\n",xyz_size);
    printf(" @ t shift    = %s\n",data_list[N_T_SHIFT]);
    printf(" @ x shift    = %s\n",data_list[N_X_SHIFT]);
    printf(" @ y shift    = %s\n",data_list[N_Y_SHIFT]);
    printf(" @ z shift    = %s\n",data_list[N_Z_SHIFT]);
    printf(" @ snk rela   = %s\n",data_list[SNK_RELA]);
    printf(" @ src rela   = %s\n",data_list[SRC_RELA]);
    printf(" @ time_min   = %d\n",time_min);
    printf(" @ time_max   = %d\n",time_max);
    printf(" @ endian cnv = %s\n",Argv[4]);
    printf(" @ conf list  = %s\n",conf_list);
    printf(" @ data file  = %s\n\n",data_list[MAIN_PATH]);
    fflush(stdout);
    if( n_conf == 0 ){
        printf(" @@@@@@ No gauge configurations !\n\n");
        return 0;
    }
//=======================================================================//
    
    int n_size = xyz_size*xyz_size*xyz_size*4*4;
    string infile_name, outfile_name;
    int t1, t2, t3;
    
    int conf_type = 2;
    if( set_dir( data_list[MAIN_PATH], n_conf, conf_type ) == -1 ) return 0;
    
#define idx(n,c) n+ n_size* c
    for( int conf=0; conf<n_conf; conf++ ){
        t1 = 0;   t2 = 0;   t3 = 0;
        
        for( int it = time_min; it <= time_max; it++ ){
            cdouble *NBS_in     = new cdouble[ n_size * 9 ];
            cdouble *NBS_proj   = new cdouble[ n_size * 7 ];
            snprintf(data_list[N_TIME], sizeof(data_list[N_TIME]), "%d", it);
            
            time( &start_time2 );
            snprintf(data_list[NBS_DIRECTORY],
                     sizeof(data_list[NBS_DIRECTORY]), "BBwave.dir");
            for( int ch = 0; ch < 9; ch++ ){
                snprintf(data_list[N_CHANNEL], sizeof(data_list[N_CHANNEL]),
                         "%d",ch);
                infile_name = set_path( data_list, conf );
                ifstream infile( infile_name.c_str(), ios::in | ios::binary );
                if( !infile ) error(2, infile_name.c_str());
                
                for( int n = 0; n < n_size; n++ )
                    infile.read((char*)&NBS_in[idx(n,ch)], sizeof(cdouble));
                infile.close();
            }
            if( endian_flg ) endian_convert( NBS_in, n_size * 9 );
            time( &end_time );
            t1 += (int)difftime(end_time,start_time2);
            
            time( &start_time2 );
            for( int n = 0; n < n_size; n++ ){
//===================== I_z = 1/2, Lambda N - Lambda N =====================//
                NBS_proj[ idx(n,0) ] = NBS_in[ idx(n,0) ];
                    
//===================== I_z = 1/2, Lambda N - Sigma  N =====================//
                NBS_proj[ idx(n,1) ] = sqrt(2.0/3.0)*NBS_in[ idx(n,2) ]
                                     - sqrt(1.0/3.0)*NBS_in[ idx(n,1) ];
                    
//===================== I_z = 1/2, Sigma  N - Lambda N =====================//
                NBS_proj[ idx(n,2) ] = sqrt(2.0/3.0)*NBS_in[ idx(n,6) ]
                                     - sqrt(1.0/3.0)*NBS_in[ idx(n,3) ];
                    
//===================== I_z = 1/2, Sigma  N - Sigma  N =====================//
                NBS_proj[ idx(n,3) ] = sqrt(2.0/3.0)*(
                                        sqrt(2.0/3.0)*NBS_in[ idx(n,8) ]
                                      - sqrt(1.0/3.0)*NBS_in[ idx(n,5) ] )
                                     - sqrt(1.0/3.0)*(
                                        sqrt(2.0/3.0)*NBS_in[ idx(n,7) ]
                                      - sqrt(1.0/3.0)*NBS_in[ idx(n,4) ] );
                    
//===================== I_z = 3/2, Lambda N - Sigma  N =====================//
                NBS_proj[ idx(n,4) ] = sqrt(1.0/3.0)*NBS_in[ idx(n,2) ]
                                     + sqrt(2.0/3.0)*NBS_in[ idx(n,1) ];
                    
//===================== I_z = 3/2, Sigma  N - Lambda N =====================//
                NBS_proj[ idx(n,5) ] = sqrt(1.0/3.0)*NBS_in[ idx(n,6) ]
                                     + sqrt(2.0/3.0)*NBS_in[ idx(n,3) ];
                    
//===================== I_z = 1/2, Sigma  N - Sigma  N =====================//
                NBS_proj[ idx(n,6) ] = sqrt(1.0/3.0)*(
                                        sqrt(1.0/3.0)*NBS_in[ idx(n,8) ]
                                      + sqrt(2.0/3.0)*NBS_in[ idx(n,5) ] )
                                     + sqrt(2.0/3.0)*(
                                        sqrt(1.0/3.0)*NBS_in[ idx(n,7) ]
                                      + sqrt(2.0/3.0)*NBS_in[ idx(n,4) ] );
            }
            time( &end_time );
            t2 += (int)difftime(end_time,start_time2);
            
            time( &start_time2 );
            snprintf(data_list[NBS_DIRECTORY],
                     sizeof(data_list[NBS_DIRECTORY]), "BBwave.prj");
            for( int ch = 0; ch < 7; ch++ ){
                snprintf(data_list[N_CHANNEL], sizeof(data_list[N_CHANNEL]),
                         "%d",ch);
                outfile_name = set_path( data_list, conf );
                ofstream outfile( outfile_name.c_str(), ios::out | ios::binary );
                if( !outfile ) error(2, infile_name.c_str());
                
                for( int n = 0; n < n_size; n++ )
                    outfile.write((char*)&NBS_proj[idx(n,ch)],sizeof(cdouble));
                outfile.close();
            }
            time( &end_time );
            t3 += (int)difftime(end_time,start_time2);

            delete [] NBS_in;
            delete [] NBS_proj;
        } // it
        printf(
    " @ conf = %3d || INPUT = %5d [s] | CALCULATE = %5d [s] | OUTPUT = %5d [s]\n"
               , conf+1, t1, t2, t3 );
        fflush(stdout);
    } // conf
#undef idx
    time( &end_time );
    printf("\n @ JOB END : ELAPSED TIME [s] = %d\n\n"
           ,(int)difftime(end_time,start_time) );
    return 0;
}

//////////////////////////////////////////////////////////////////////////

int set_dir( char* PATH, int N_conf, int conf_type ){

    char dir_name[2048];
    char conf_name[2048];
    char conf_dir_name[2048];
    int num1, num2;
    string conf;
    
    if(conf_type==1)
        conf = "RC16x32_B1830Kud013760Ks013710C1761";
    else if(conf_type==2)
        conf = "RC32x64_B1900Kud01372700Ks01364000C1715";
    else{
        printf(" @@@@@@ invalid conf type !\n\n");
        return -1;
    }

    printf(" @ output directory setting... |     ");
    for( int channel = 0; channel < 7; channel++ ){
        snprintf( dir_name, sizeof(dir_name),
                 "%s/BBwave.prj.S1.%02d", PATH, channel );
        mkdir( dir_name, 0755 );

        for( int conf_loop = 1; conf_loop <= N_conf; conf_loop++ ){
        
            if(conf_type==1){
                if( conf_loop <= 500 ){ num1 = 1;   num2 = conf_loop + 40; }
                else if( conf_loop <= 600 ){ num1 = 2;   num2 = conf_loop - 450; }
                else{ num1 = 3;   num2 = conf_loop - 560; }
                snprintf( conf_name, sizeof(conf_name),
                         "%s-%1d-%05d", conf.c_str(), num1, num2);
            }else if(conf_type==2){
                snprintf( conf_name, sizeof(conf_name),
                         "%s-b-%05d", conf.c_str(), conf_loop+130);
            }
            snprintf( conf_dir_name, sizeof(conf_dir_name),
                     "%s/%s0", dir_name, conf_name);
            mkdir( conf_dir_name, 0755 );
        }
        printf("\b\b\b\b%3.0lf%%",double(channel+1)/double(7)*100);
        fflush(stdout);
    }
    printf("\n");
    return 0;
}
