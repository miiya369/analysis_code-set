//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Function for convert the fit function number and function name
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <analysis.h>

string N_func_to_name( int N_func ){
    
    string func_name;
    
    if( N_func == TEST )
        func_name = "test";
    else if( N_func == CONSTANT )
        func_name = "const";
    else if( N_func == ONE_EXPONENTIAL )
        func_name = "1EXP";
    else if( N_func == ONE_GAUSSIAN )
        func_name = "1G";
    else if( N_func == TWO_GAUSSIAN )
        func_name = "2G";
    else if( N_func == THREE_GAUSSIAN )
        func_name = "3G";
    else if( N_func == ONE_SHIFTED_GAUSSIAN )
        func_name = "1SG";
    else if( N_func == TWO_SHIFTED_GAUSSIAN )
        func_name = "2SG";
    else
        func_name = "UNKNOWN";
    
    return func_name;
}

int name_to_N_func( char* func_name ){
    
    int N_func;
    
    if( strcmp(func_name,"test" )==0 )
        N_func = TEST;
    else if( strcmp( func_name,"const" )==0 )
            N_func = CONSTANT;
    else if( strcmp( func_name,"1EXP" )==0 )
            N_func = ONE_EXPONENTIAL;
    else if( strcmp( func_name,"1G" )==0 )
            N_func = ONE_GAUSSIAN;
    else if( strcmp( func_name,"2G" )==0 )
            N_func = TWO_GAUSSIAN;
    else if( strcmp( func_name,"3G" )==0 )
            N_func = THREE_GAUSSIAN;
    else if( strcmp( func_name,"1SG" )==0 )
            N_func = ONE_SHIFTED_GAUSSIAN;
    else if( strcmp( func_name,"2SG" )==0 )
            N_func = TWO_SHIFTED_GAUSSIAN;
    else
        N_func = -1;
    
    return N_func;
}
