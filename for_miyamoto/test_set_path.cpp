
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

string set_path( char**, int );

int main( int argc, char **argv ){
    
    const char aaa[2] = "a";
    char bbb[100];
    snprintf(bbb,sizeof(bbb),"%c",aaa[0]);
    printf("%c %c\n",bbb[0],bbb[1]);
    
    ifstream ifs( argv[1], ios::in );
    if( !ifs ){ printf("おい！！！！！！！！！\n"); return 0; }
    char Argv[1000][1024];
    int i=1;
    snprintf(Argv[0],sizeof(Argv[0]), "%s","c");
    while( ifs.getline(Argv[i],sizeof(Argv[i])) ) i++;
    for( int j=1; j<400; j++) cout << set_path(Argv,j) << endl;
    return 0;
}

string set_path( char path_list[1000][1024], int num_conf ){
    
    char file_name[1024];
    
    //===================== For path list of all data file =====================//
    if( path_list[0][0] == 'a' )
        snprintf( file_name, sizeof(file_name), "%s", path_list[num_conf] );
    
    //====================== For path list of gauge confs ======================//
    else if( path_list[0][0] == 'b' ){
        char dir_name[30];
        char header[60];
        char footer[30];
        int  time    = atoi( path_list[2] );
        int  channel = atoi( path_list[3] );
        int  t_shift = atoi( path_list[6] );
        
        if( time == -1 ){
            snprintf( dir_name, sizeof(dir_name), "correlator.PS.dir");
            snprintf( header, sizeof(header),
                     "%s_correlator.", path_list[5]);
            snprintf( footer, sizeof(footer), "0");
        }
        else{
            snprintf( dir_name, sizeof(dir_name),
                     "%s.S1.%02d", path_list[4], channel);
            snprintf( header, sizeof(header), "NBSwave.+%03d", time);
            snprintf( footer, sizeof(footer), "0.NUC_CG05.NUC_CG05");
        }
        snprintf( file_name, sizeof(file_name),
                 "%s/%s/%s0/%s+000.000.000.%03d.%s%s",
                 path_list[1], dir_name, path_list[num_conf+6], header,
                 t_shift, path_list[num_conf+6], footer );
        
    //===================== For make path list in the code =====================//
    }else if( path_list[0][0] == 'c' ){
        char dir_name[30];
        char conf_name[1024];
        char header[60];
        char footer[30];
        int  time      = atoi( path_list[2] );
        int  channel   = atoi( path_list[3] );
        int  t_shift   = atoi( path_list[6] );
        int  conf_type = atoi( path_list[7] );
        
        if( time == -1 ){
            snprintf( dir_name, sizeof(dir_name), "correlator.PS.dir");
            snprintf( header, sizeof(header),
                     "%s_correlator.", path_list[5]);
            snprintf( footer, sizeof(footer), "0");
        }
        else{
            snprintf( dir_name, sizeof(dir_name),
                     "%s.S1.%02d", path_list[4], channel);
            snprintf( header, sizeof(header), "NBSwave.+%03d", time);
            snprintf( footer, sizeof(footer), "0.NUC_CG05.NUC_CG05");
        }
        
        if( conf_type == 1 ){   // cp-pacs confs ens_3
            char conf[] = "RC16x32_B1830Kud013760Ks013710C1761";
            int num1, num2;
            if( num_conf <= 500 ){ num1 = 1;   num2 = num_conf + 40; }
            else if( num_conf <= 600 ){ num1 = 2;   num2 = num_conf - 450; }
            else{ num1 = 3;   num2 = num_conf - 560; }
            snprintf( conf_name, sizeof(conf_name),
                     "%s-%1d-%05d", conf, num1, num2);
            
        }else if( conf_type == 2 ){   // pacs-cs confs ens_2
            char conf[] = "RC32x64_B1900Kud01372700Ks01364000C1715";
            snprintf( conf_name, sizeof(conf_name), "%s-b-%05d",
                     conf, num_conf+130);
        }
        snprintf( file_name, sizeof(file_name),
                 "%s/%s/%s0/%s+000.000.000.%03d.%s%s",
                 path_list[1], dir_name, conf_name, header,
                 t_shift, conf_name, footer );
        
    }
    string FILE_NAME( file_name );
    return FILE_NAME;
}