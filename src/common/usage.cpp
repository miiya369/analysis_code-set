//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Function for usage
 * @author  Takaya Miyamoto
 * @since   Mon Sep  7 02:23:05 JST 2015
 */
//--------------------------------------------------------------------------

#include <common/analysis.h>

void analysis::usage( int usage_type ) {
   
   if (usage_type == EFFECTIVEMASS) {
      printf("\n ====== Calculation & Output effective mass usage   ======\n");
      printf("\n");
      printf("   -f      [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -ifile  [ Path of the input directory ]\n");
      printf("   -ofile  [ Path of the output directory ]\n");
      printf("   -fit      Do fitting effective mass\n");
      printf("   -t_min  [ Minimum range of fitting [t] ] : For effective mass fitting\n");
      printf("   -t_max  [ Maximum range of fitting [t] ] : For effective mass fitting\n");
      printf("   -hadron [ hadron name ] [ ] [ ] ... [ ] @ <- Need \"@\" at the end\n");
      printf("   -check    Check the arguments\n\n");
   }
   else if (usage_type == CALC_WAVE_FUNCTION) {
      printf("\n ====== Calculation & Output wave function usage ======\n");
      printf("\n");
      printf("   -f         [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -t_min     [ Minimum value of time slice ]\n");
      printf("   -t_max     [ Maximum value of time slice ]\n");
      printf("   -channel   [ Channel name to calculate ]\n");
      printf("   -comp        Read compress NBS wave\n");
      printf("   -check       Check the arguments\n\n");
   }
   else if (usage_type == CALC_POTENTIAL) {
      printf("\n ====== Calculation & Output potential usage ======\n");
      printf("\n");
      printf("   -f         [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -t_min     [ Minimum value of time slice ]\n");
      printf("   -t_max     [ Maximum value of time slice ]\n");
      printf("   -channel   [ Channel name to calculate ]\n");
      printf("   -mass_had1 [ Mass of hadron 1 ]\n");
      printf("   -mass_had2 [ Mass of hadron 2 ]\n");
      printf("   -comp        Read compress NBS wave\n");
      printf("   -check       Check the arguments\n\n");
   }
   else if (usage_type == FITTING_DATA) {
      printf("\n ======   Data fitting usage   ======\n");
      printf("\n");
      printf("   -f        [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -ifile    [ Path of the input fit data file ]\n");
      printf("   -ofile    [ Path of the output parameter data file ]\n");
      printf("   -t_min    [ Minimum range of fitting [t] ] : For correlator\n");
      printf("   -t_max    [ Maximum range of fitting [t] ] : For correlator\n");
      printf("   -fit_func [ Fit function type ]\n");
      printf("   -param    [ Parameter ] [ ] [ ] ... [ ] @ <- Need \"@\" at the end\n");
      printf("   -check      Check the arguments\n\n");
   }
   else if (usage_type == CALC_OBSERVABLE) {
      printf("\n ======   Observable calculation usage   ======\n");
      printf("\n");
      printf("   -f     [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -ifile [ Path of the input parameter file ]\n");
      printf("   -ofile [ Path of the output file]\n");
      printf("   -mass  [ Reduced mass [Mev/c2] ]\n\n");
      printf("   -test    Calculate test potential ( Square wall )\n");
      printf("   -V0    [ Energy of Square wall potential ( For test ) ]\n");
      printf("   -r0    [ Range  of Square wall potential ( For test ) ]\n");
      printf("   -check   Check the arguments\n\n");
   }
   else if (usage_type == COUPLED_CHANNEL_POT) {
      printf("\n ====== Calculation & Output coupled channel potential usage ======\n");
      printf("\n");
      printf("   -f    [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -time [ Value of time slice for calculate ]\n");
      printf("   -check  Check the arguments\n\n");
   }
   else if (usage_type == EXTRACT_Z_FACTOR) {
      printf("\n ====== Calculation & Output Z-factor usage ======\n");
      printf("\n");
      printf("   -f      [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -ifile  [ Path of the input directory ]\n");
      printf("   -ofile  [ Path of the output directory ]\n");
      printf("   -hadron [ hadron name ] [ ] [ ] ... [ ] @ <- Need \"@\" at the end\n");
      printf("   -check    Check the arguments\n\n");
   }
   else if (usage_type == AVERAGE_COMPRESS) {
      printf("\n ====== Calculation & Output Z-factor usage ======\n");
      printf("\n");
      printf("   -f       [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -wdir    [ Path of the working directory ]\n");
      printf("   -t_shift [ time shift 1   ] [2] [3] ... [n] @ <- Need \"@\" at the end\n");
      printf("   -hadron  [ hadron name 1  ] [2] [3] ... [n] @ <- Need \"@\" at the end\n");
      printf("   -hadron  [ channel name 1 ] [2] [3] ... [n] @ <- Need \"@\" at the end\n");
      printf("   -check    Check the arguments\n\n");
   }
}
