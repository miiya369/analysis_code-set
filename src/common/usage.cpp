//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Function for usage
 * @author  Takaya Miyamoto
 * @since   Fri Dec 18 08:00:33 JST 2015
 */
//--------------------------------------------------------------------------

#include <common/analysis.h>

void analysis::usage( int usage_type ) {
   
   if (usage_type == EFFECTIVEMASS) {
      printf("\n ====== Calculation & Output effective mass usage   ======\n");
      printf("\n");
      printf("   -f       [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -idir    [ Path of the input directory ]\n");
      printf("   -odir    [ Path of the output directory ]\n");
      printf("   -calc_R    Calculate R-correlator effective mass\n");
      printf("   -fit       Do fitting effective mass\n");
      printf("   -t_min   [ Minimum range of fitting [t] ] : For effective mass fitting\n");
      printf("   -t_max   [ Maximum range of fitting [t] ] : For effective mass fitting\n");
      printf("   -hadron  [ hadron  name ] [ ] [ ] ... [ ] @ <- Need \"@\" at the end\n");
      printf("   -channel [ channel name ] [ ] [ ] ... [ ] @ <- Need \"@\" at the end\n");
      printf("   -src_t   [ time slice of src point ]\n");
      printf("   -check     Check the arguments\n\n");
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
      printf("   -idir      [ Path of the input directory ]\n");
      printf("   -odir      [ Path of the output directory ]\n");
      printf("   -t_min     [ Minimum value of time slice ]\n");
      printf("   -t_max     [ Maximum value of time slice ]\n");
      printf("   -channel   [ Channel name to calculate ]\n");
      printf("   -mass_had1 [ Mass of hadron 1 ]\n");
      printf("   -mass_had2 [ Mass of hadron 2 ]\n");
      printf("   -src_t     [ time slice of src point ]\n");
      printf("   -comp        Read compress NBS wave\n");
      printf("   -check       Check the arguments\n\n");
   }
   else if (usage_type == CALC_TENSOR_FORCE) {
      printf("\n ====== Calculation & Output tensor potential usage ======\n");
      printf("\n");
      printf("   -f         [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -idir      [ Path of the input directory ]\n");
      printf("   -odir      [ Path of the output directory ]\n");
      printf("   -t_min     [ Minimum value of time slice ]\n");
      printf("   -t_max     [ Maximum value of time slice ]\n");
      printf("   -channel   [ Channel name to calculate ]\n");
      printf("   -mass_had1 [ Mass of hadron 1 ]\n");
      printf("   -mass_had2 [ Mass of hadron 2 ]\n");
      printf("   -src_t     [ time slice of src point ]\n");
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
   else if (usage_type == EIGEN_ENERGY) {
      printf("\n ====== Calculation eigen energy for finite box usage ======\n");
      printf("\n");
      printf("   -f    [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -time [ Value of time slice for calculate ]\n");
      printf("   -check  Check the arguments\n\n");
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
      printf("   -ifile  [ Path of the input file ]\n");
      printf("   -ofile  [ Path of the output file ]\n");
      printf("   -hadron [ hadron name ] [ ] [ ] ... [ ] @ <- Need \"@\" at the end\n");
      printf("   -check    Check the arguments\n\n");
   }
   else if (usage_type == AVERAGE_COMPRESS) {
      printf("\n ====== Calculation of average (Compressed) NBS wave function usage ======\n");
      printf("\n");
      printf("   -f       [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -wdir    [ Path of the working directory ]\n");
      printf("   -t_shift [ time shift 1   ] [2] [3] ... [n] @ <- Need \"@\" at the end\n");
      printf("   -hadron  [ hadron name 1  ] [2] [3] ... [n] @ <- Need \"@\" at the end\n");
      printf("   -channel [ channel name 1 ] [2] [3] ... [n] @ <- Need \"@\" at the end\n");
      printf("   -check    Check the arguments\n\n");
   }
   else if (usage_type == JACK_KNIFE_BIN_CHECK) {
      printf("\n ====== Check the bin-size dependence of Jack Knife samples usage ======\n");
      printf("\n");
      printf("   -f       [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -idir    [ Path of the input directory ]\n");
      printf("   -ofile   [ Path of the output file ]\n");
      printf("   -hadron  [ hadron name ]\n");
      printf("   -t_min   [ Minimum value of time slice ]\n");
      printf("   -t_max   [ Maximum value of time slice ]\n");
      printf("   -src_t   [ time slice of src point ]\n");
      printf("   -check    Check the arguments\n\n");
   }
   else if (usage_type == MAKE_JACK_KNIFE_SAMPLES) {
      printf("\n ====== Make Jack-Knife samples usage ======\n");
      printf("\n");
      printf("   -f       [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -idir    [ Path of the input directory ]\n");
      printf("   -odir    [ Path of the output directory ]\n");
      printf("   -obase   [ The base name of output configurations ]\n");
      printf("   -src_t   [ time slice of src point ]\n");
      printf("   -hadron  [ hadron name 1  ] [2] [3] ... [n] @ <- Need \"@\" at the end\n");
      printf("   -channel [ channel name 1 ] [2] [3] ... [n] @ <- Need \"@\" at the end\n");
      printf("   -check    Check the arguments\n\n");
   }
}
