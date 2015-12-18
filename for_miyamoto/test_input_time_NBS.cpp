
#include <stdio.h>
#include <fstream>
#include <complex>
#include <time.h>

using namespace std;

typedef complex<int> cint;
typedef complex<double> cdouble;

string set_path( string PATH, string NBS_dir, int channel, string HAD, int t_shift, int num_conf, int time, int conf_type );

int main(){
    
    int SIZE = 16;
    int Ndata = 190;
    int time_NBS = 8;
    int conf_type = 1;
    int buf_size=1;
    const char* INFILE_PATH = "/Users/miiya/Dropbox/analysis_HAL_code/analysis_code_set_v2015.05.14/test_data/cp-pacs_ens3_193conf";
    
    int N_loop = 100;
    
    time_t start_time, end_time;
    cdouble *NBS;   NBS = new cdouble[ 4*4*SIZE*SIZE*SIZE*Ndata ];
    
    for( int buf_loop = 1; buf_loop <= 5; buf_loop++ ){
    time( &start_time );
    for( int loop=0;loop<N_loop;loop++) for( int conf = 0; conf < Ndata; conf++ ){
        
        string NBSfile_name = set_path( INFILE_PATH, "BBwave.dir", 0, "", 0, conf+1, time_NBS, conf_type );
        ifstream NBSfile( NBSfile_name.c_str(), ios::in | ios::binary );
        if( !NBSfile ){
            printf( " @@@@@@ FILE OPEN ERROR : The file \"%s\" is not exist.\n", NBSfile_name.c_str());
            continue;
        }
        for( int file_loop = 0; file_loop < 4*4*SIZE*SIZE*SIZE; file_loop+=buf_size ){
            
            NBSfile.read( (char*)&NBS[ file_loop + 4*4*SIZE*SIZE*SIZE* conf ], buf_size*sizeof(cdouble) );
        }
        NBSfile.close();
    }
    time( &end_time );
    printf("%d %lf\n", buf_size, difftime(end_time,start_time) );
        buf_size *= 2;
    }
    
    delete [] NBS;
}

//====================================================================================//
//====================================================================================//

string set_path( string PATH, string NBS_dir, int channel, string HAD, int t_shift, int num_conf, int time, int conf_type ){
    
    char file_name[1024];   // conf_type == 1 -> "cp-pacs confs ens_3", 2 -> "pacs-cs confs ens_2"
    char conf_name[1024];
    char dir_name[30];
    char header[60];
    char footer[30];
    
    if( conf_type == 1 ){
        string conf = "RC16x32_B1830Kud013760Ks013710C1761";   // cp-pacs confs ens_3
        int num1, num2;
        if( num_conf <= 500 ){ num1 = 1;   num2 = num_conf + 40; }
        else if( num_conf <= 600 ){ num1 = 2;   num2 = num_conf - 450; }
        else{ num1 = 3;   num2 = num_conf - 560; }
        snprintf( conf_name, sizeof(conf_name), "%s-%1d-%05d", conf.c_str(), num1, num2);
        
    }else if( conf_type == 2 ){
        string conf = "RC32x64_B1900Kud01372700Ks01364000C1715";   // pacs-cs confs ens_2
        snprintf( conf_name, sizeof(conf_name), "%s-b-%05d", conf.c_str(), num_conf+130);
    }
    
    if( time == -1 ){
        snprintf( dir_name, sizeof(dir_name), "correlator.PS.dir");
        snprintf( header, sizeof(header), "%s_correlator.", HAD.c_str());
        snprintf( footer, sizeof(footer), "0");
    }
    else{
        snprintf( dir_name, sizeof(dir_name), "%s.S1.%02d", NBS_dir.c_str(), channel);
        snprintf( header, sizeof(header), "NBSwave.+%03d", time);
        snprintf( footer, sizeof(footer), "0.NUC_CG05.NUC_CG05");
    }
    
    snprintf( file_name, sizeof(file_name), "%s/%s/%s0/%s+000.000.000.%03d.%s%s",
             PATH.c_str(), dir_name, conf_name, header, t_shift, conf_name, footer );
    
    string FILE_NAME( file_name );
    return FILE_NAME;
}