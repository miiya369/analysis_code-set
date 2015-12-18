
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

using namespace std;

int set_arg( int argc, char **argv, char argv_o[1024][1024] ){
    
    int count1 = 0;
    int count2 = 0;
    char tmp_str[100][1024],tmp_c1[1024],tmp_c2[1024];
    
    ifstream ifs(argv[1]);
    if(!ifs) printf("えあああああああああ！！！！");
    
    while(ifs.getline(tmp_str[count2],sizeof(tmp_str[count2]))) count2++;
    for( int loop1=0; loop1<count2; loop1++ ){
        if(sscanf(tmp_str[loop1]," %s = %s ",tmp_c1,tmp_c2) != 2 )
            continue;
        if( strcmp(tmp_c1,"FIT_Parameter") == 0 ){
            char *tmp_tok;
            tmp_tok = strtok(tmp_str[loop1]," \t");
            for(int iii=0;iii<3;iii++) tmp_tok = strtok(NULL," \t");
            while( strcmp(tmp_tok,"}") != 0){
                snprintf(argv_o[count1],sizeof(argv_o[count1]),"%s",tmp_tok);
                count1++;
                tmp_tok = strtok(NULL," \t");
            }
        }
    }
    
    return count1;
}

int main( int argc, char **argv ){
 
    char Argv[1024][1024];
    
    int n_arg = set_arg(argc,argv,Argv);
    
    for( int n=0; n<n_arg; n++ ) printf("%s\n",Argv[n]);
    
    return 0;
}