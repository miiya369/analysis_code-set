//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Miscellaneous functions
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <analysis.h>

void route( string class_name, string func_name, int io ){
    
//    return;   // for no debug.
    
    time_t timer;
    time(&timer);
         if( io == 1 ) printf(" @@@ Enter into  %s.%s || %s", class_name.c_str(),
                         func_name.c_str(), ctime(&timer));
    else if( io == 0 ) printf(" @@@ Go out from %s.%s || %s", class_name.c_str(),
                         func_name.c_str(), ctime(&timer));
    fflush(stdout);
}

void error( int error_type, const char* error_comments ){
    
    if( error_type == 0 ) exit(1);
    
    if( error_type == 1 ) printf("\n\n @@@@@@ WARNING : %s\n\n", error_comments);
    
    if( error_type == 2 ){
        printf("\n\n @@@@@@ FILE OPEN ERROR : The file \"%s\" is not exist.\n\n",
               error_comments);
        exit(1);
    }
    if( error_type == 3 ){
        printf("\n\n @@@@@@ ERROR : %s\n\n", error_comments);
        exit(1);
    }
}

void endian_convert( cdouble *DATA, int DATA_size ){
    
    char dummy_R[8], dummy_I[8];
    time_t start_t, end_t;
    time( &start_t );
    printf(" @ Convert endian ... ");
    fflush(stdout);
    
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for(int k=0; k<DATA_size; k++ ){
        
        for(int j=0; j<8; j++){
            dummy_R[j] = ((char*)&DATA[k])[j];
            dummy_I[j] = ((char*)&DATA[k])[j+8];
        }
        for(int j=0; j<8; j++){
            ((char*)&DATA[k])[j]   = dummy_R[7-j];
            ((char*)&DATA[k])[j+8] = dummy_I[7-j];
        }
    }
    time( &end_t );
    printf("Done |       ( %d second )\n",(int)difftime(end_t,start_t));
}

void endian_convert( double *DATA, int DATA_size ){
    
    char dummy[8];
    time_t start_t, end_t;
    time( &start_t );
    printf(" @ Convert endian ... ");
    fflush(stdout);
    
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for(int k=0; k<DATA_size; k++ ){
        for(int j=0; j<8; j++) dummy[j] = ((char*)&DATA[k])[j];
        for(int j=0; j<8; j++) ((char*)&DATA[k])[j] = dummy[7-j];
    }
    time( &end_t );
    printf("Done |       ( %d second )\n",(int)difftime(end_t,start_t));
}

void endian_convert( int* DATA ){
    
    char dummy[4];

    for(int j=0; j<4; j++) dummy[j] = ((char*)&DATA[0])[j];
    for(int j=0; j<4; j++) ((char*)&DATA[0])[j] = dummy[3-j];
}
