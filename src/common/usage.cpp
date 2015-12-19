//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Function for usage
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <analysis.h>

void usage( int usage_type ){
    
    if( usage_type == EXTRACT_Z_FACTOR ){
        printf("\n ====== Calculation & Output Z-factor usage ======\n");
        printf("\n");
        printf("   -f [ Path of the input arguments file ] <- Must be required !\n\n");
    }
    if( usage_type == COUPLED_CHANNEL_POT ){
        printf("\n ====== Calculation & Output coupled channel potential usage ======\n");
        printf("\n");
        printf("   -f [ Path of the input arguments file ] <- Must be required !\n");
        printf("   -t [ Value of time slice for calculate ]\n\n");
    }
    if( usage_type == EFFECTIVEMASS ){
        printf("\n ====== Calculation & Output effective mass usage   ======\n");
        printf("\n");
        printf("   -f [ Path of the input arguments file ] <- Must be required !\n");
        printf("   -i [ Path of the input directory ]\n");
        printf("   -o [ Path of the output directory ]\n");
        printf("   -m  Do fitting effective mass\n");
        printf("   -t [ Minimum range of fitting [t] ] : For effective mass fitting\n");
        printf("   -T [ Maximum range of fitting [t] ] : For effective mass fitting\n\n");
    }
    if( usage_type == CALC_POTENTIAL ){
        printf("\n ====== Calculation & Output potential usage ======\n");
        printf("\n");
        printf("   -f [ Path of the input arguments file ] <- Must be required !\n");
        printf("   -t [ Minimum value of time slice ]\n");
        printf("   -T [ Maximum value of time slice ]\n\n");
    }
    if( usage_type == FITTING_DATA ){
        printf("\n ======   Data fitting usage   ======\n");
        printf("\n");
        printf("   -f [ Path of the input arguments file ] <- Must be required !\n");
        printf("   -i [ Path of the input fit data file ]\n");
        printf("   -o [ The output file name ]\n");
        printf("   -r [ Minimum range of fitting [t] ] : For correlator\n");
        printf("   -R [ Maximum range of fitting [t] ] : For correlator\n");
        printf("   -t [ Fit function type ]\n");
        printf("   -p [ Parameter ] [ ] [ ] ... [ ] @ <- Need \"@\" at the end\n\n");
    }
    if( usage_type == CALC_OBSERVABLE ){
        printf("\n ======   Observable calculation usage   ======\n");
        printf("\n");
        printf("   -f [ Path of the input arguments file ] <- Must be required !\n");
        printf("   -i [ Path of the input parameter file ]\n");
        printf("   -o [ The output file name ]\n");
        printf("   -m [ Reduced mass [Mev/c2] ]\n\n");
        printf("   -t  Calculate test potential ( Square wall )\n");
        printf("   -e [ Energy of Square wall potential ( For test ) ]\n");
        printf("   -r [ Range  of Square wall potential ( For test ) ]\n\n");
    }
    if( usage_type == ISOSPIN_PROJECTION ){
        printf("\n ======   Isospin projection usage   ======\n");
        printf("\n");
        printf("   -f [ Path of the input arguments file ] <- Must be required !\n\n");
    }
}
