//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Non
 * @brief   Check machine's endian
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <stdio.h>

int main(){
    
    int endianTEST = 1;
    if( *(char*)&endianTEST )
        printf("\n @ This machine is Little endian.\n\n");
    else
        printf("\n @ This machine is Big endian.\n\n");
    
    return 0;
}
