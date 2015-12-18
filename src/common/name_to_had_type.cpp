//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Function for convert the hadron name and hadron type number
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <analysis.h>

int name_to_hadron_type( char* hadron_name ){
    
    int hadron_type;
    
    if( strcmp( hadron_name,"pion" )==0 )
        hadron_type = PION;
    else if( strcmp( hadron_name,"phi" )==0 )
        hadron_type = PHI;
    else if( strcmp( hadron_name,"etaSS" )==0 )
        hadron_type = ETASS;
    else if( strcmp( hadron_name,"proton" )==0 )
        hadron_type = PROTON;
    else if( strcmp( hadron_name,"Lambda" )==0 )
        hadron_type = LAMBDA;
    else if( strcmp( hadron_name,"Sigma" )==0 )
        hadron_type = SIGMA;
    else
        hadron_type = -1;
    
    return hadron_type;
}

string had_type_to_name( int had_type ){
    
    string hadron_name;
    
    if( had_type == PION )
        hadron_name = "pion";
    else if( had_type == PHI )
        hadron_name = "phi";
    else if( had_type == ETASS )
        hadron_name = "etaSS";
    else if( had_type == PROTON )
        hadron_name = "proton";
    else if( had_type == LAMBDA )
        hadron_name = "Lambda";
    else if( had_type == SIGMA )
        hadron_name = "Sigma";
    else
        hadron_name = "UNKNOWN";
    
    return hadron_name;
}
