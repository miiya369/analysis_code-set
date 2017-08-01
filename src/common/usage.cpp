//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Function for usage
 * @author  Takaya Miyamoto
 * @since   Mon Jan 30 08:58:23 JST 2017
 */
//--------------------------------------------------------------------------

#include <common/analysis.h>

void analysis::usage(int usage_type)
{
   if (usage_type == EFFECTIVEMASS)
   {
      printf("\n ====== Calculation & Output effective mass usage   ======\n");
      printf("\n");
      printf("   -f         [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -conf_list [ Gauge configuration list ]\n");
      printf("   -idir      [ Path of the input directory ]\n");
      printf("   -odir      [ Path of the output directory ]\n");
      printf("   -take_JK     Take Jack-Knife average after input the data\n");
      printf("   -calc_R      Calculate R-correlator effective mass\n");
      printf("   -fit         Do fitting effective mass\n");
      printf("   -in_fit      Input correlator-fit data\n");
      printf("   -t_min     [ Minimum range of fitting [t] ] : For effective mass fitting\n");
      printf("   -t_max     [ Maximum range of fitting [t] ] : For effective mass fitting\n");
      printf("   -hadron    [ Hadron  name ] [ ] [ ] ... [ ] @ <- Need \"@\" at the end\n");
      printf("   -channel   [ Channel name ] [ ] [ ] ... [ ] @ <- Need \"@\" at the end\n");
      printf("   -mom       [ Momentum of (Hadron) correlator ]\n");
      printf("   -spin      [ Spin type of (NBS) correlator ]\n");
      printf("   -src_t     [ Time slice of src point ]\n");
      printf("   -dir_type  [ Directory type (PS or SS) to read ]\n");
      printf("   -check       Check the arguments\n\n");
   }
   else if (usage_type == CALC_WAVE_FUNCTION)
   {
      printf("\n ====== Calculation & Output wave function usage ======\n");
      printf("\n");
      printf("   -f         [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -conf_list [ Gauge configuration list ]\n");
      printf("   -t_min     [ Minimum value of time slice ]\n");
      printf("   -t_max     [ Maximum value of time slice ]\n");
      printf("   -channel   [ Channel name to calculate ]\n");
      printf("   -spin      [ Spin type to calculate ]\n");
      printf("   -take_JK     Take Jack-Knife average after input the data\n");
      printf("   -check       Check the arguments\n\n");
   }
   else if (usage_type == CALC_POTENTIAL)
   {
      printf("\n ====== Calculation & Output potential usage ======\n");
      printf("\n");
      printf("   -f         [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -conf_list [ Gauge configuration list ]\n");
      printf("   -idir      [ Path of the input directory ]\n");
      printf("   -odir      [ Path of the output directory ]\n");
      printf("   -t_min     [ Minimum value of time slice ]\n");
      printf("   -t_max     [ Maximum value of time slice ]\n");
      printf("   -channel   [ Channel name to calculate ]\n");
      printf("   -spin      [ Spin type to calculate ]\n");
      printf("   -mass_had1 [ Mass of hadron 1 ]\n");
      printf("   -mass_had2 [ Mass of hadron 2 ]\n");
      printf("   -src_t     [ time slice of src point ]\n");
      printf("   -src_rela  [ Relativistic of src ]\n");
      printf("   -snk_rela  [ Relativistic of snk ]\n");
      printf("   -take_JK     Take Jack-Knife average after input the data\n");
      printf("   -check       Check the arguments\n\n");
   }
   else if (usage_type == CALC_TENSOR_FORCE)
   {
      printf("\n ====== Calculation & Output tensor potential usage ======\n");
      printf("\n");
      printf("   -f         [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -conf_list [ Gauge configuration list ]\n");
      printf("   -idir      [ Path of the input directory ]\n");
      printf("   -odir      [ Path of the output directory ]\n");
      printf("   -time      [ The value of time slice ]\n");
      printf("   -channel   [ Channel name to calculate ]\n");
      printf("   -mass_had1 [ Mass of hadron 1 ]\n");
      printf("   -mass_had2 [ Mass of hadron 2 ]\n");
      printf("   -src_t     [ time slice of src point ]\n");
      printf("   -take_JK     Take Jack-Knife average after input the data\n");
      printf("   -check       Check the arguments\n\n");
   }
   else if (usage_type == CALC_TENSOR_FORCE_2x2)
   {
      printf("\n ====== Calculation & Output 2x2 coupled channel tensor potential usage ======\n");
      printf("\n");
      printf("   -f         [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -conf_list [ Gauge configuration list ]\n");
      printf("   -idir      [ Path of the input directory ]\n");
      printf("   -odir      [ Path of the output directory ]\n");
      printf("   -time      [ The value of time slice ]\n");
      printf("   -src_t     [ time slice of src point ]\n");
      printf("   -check       Check the arguments\n\n");
   }
   else if (usage_type == FITTING_DATA)
   {
      printf("\n ======   Data fitting usage   ======\n");
      printf("\n");
      printf("   -f         [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -ifile     [ Path of the input fit data file ]\n");
      printf("   -ofile     [ Path of the output parameter data file ]\n");
      printf("   -r_min     [ Minimum range of fitting ]\n");
      printf("   -r_max     [ Maximum range of fitting ]\n");
      printf("   -fit_func  [ Fit function type ]\n");
      printf("   -param     [ Parameter ] [ ] [ ] ... [ ] @ <- Need \"@\" at the end\n");
      printf("   -check       Check the arguments\n\n");
   }
   else if (usage_type == CALC_PHASE_SHIFT)
   {
      printf("\n ======   Phase shift calculation usage   ======\n");
      printf("\n");
      printf("   -f         [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -ifile     [ Path of the input parameter file ]\n");
      printf("   -ofile     [ Path of the output file ]\n");
      printf("   -E_min     [ Minimum Enargy (MeV) for output ]\n");
      printf("   -E_max     [ Maximum Enargy (MeV) for output ]\n");
      printf("   -E_div     [ Enargy devision (MeV) for output ]\n");
      printf("   -mass      [ Reduced mass [Mev/c2] ]\n\n");
      printf("   -test        Calculate test potential ( Square wall )\n");
      printf("   -V0        [ Energy of Square wall potential ( For test ) ]\n");
      printf("   -r0        [ Range  of Square wall potential ( For test ) ]\n");
      printf("   -check       Check the arguments\n\n");
   }
   else if (usage_type == CALC_T_MATRIX_2x2)
   {
      printf("\n ======   2x2 coupled channel T matrix calculation usage   ======\n");
      printf("\n");
      printf("   -f         [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -E_min     [ Minimum Enargy (MeV) for output ]\n");
      printf("   -E_max     [ Maximum Enargy (MeV) for output ]\n");
      printf("   -E_div     [ Enargy devision (MeV) for output ]\n\n");
      printf("   -check       Check the arguments\n\n");
   }
   else if (usage_type == EIGEN_ENERGY)
   {
      printf("\n ====== Calculation eigen energy for finite box usage ======\n");
      printf("\n");
      printf("   -f         [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -idir      [ Path of the input directory ]\n");
      printf("   -conf_list [ Gauge configuration list ]\n");
      printf("   -time      [ Value of time slice for calculate ]\n");
      printf("   -spin      [ Spin type to calculate ]\n");
      printf("   -mass_had1 [ Mass of hadron 1 ]\n");
      printf("   -mass_had2 [ Mass of hadron 2 ]\n");
      printf("   -take_JK     Take Jack-Knife average after input the data\n");
      printf("   -check       Check the arguments\n\n");
   }
   else if (usage_type == EIGEN_ARPACK)
   {
      printf("\n ====== Calculation eigenenergy & eigenvector for finite box by using ARPACK ======\n");
      printf("\n");
      printf("   -f         [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -idir      [ Path of the input directory ]\n");
      printf("   -conf_list [ Gauge configuration list ]\n");
      printf("   -time      [ Value of time slice for calculate ]\n");
      printf("   -spin      [ Spin type to calculate ]\n");
      printf("   -mass_had1 [ Mass of hadron 1 ]\n");
      printf("   -mass_had2 [ Mass of hadron 2 ]\n");
      printf("   -take_JK     Take Jack-Knife average after input the data\n");
      printf("   -check       Check the arguments\n\n");
   }
   if (usage_type == DISPERSION_RELATION)
   {
      printf("\n ====== Calculation & Output dispersion relation usage   ======\n");
      printf("\n");
      printf("   -f         [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -conf_list [ Gauge configuration list ]\n");
      printf("   -idir      [ Path of the input directory ]\n");
      printf("   -odir      [ Path of the output directory ]\n");
      printf("   -take_JK     Take Jack-Knife average after input the data\n");
      printf("   -t_min     [ Minimum range of fitting [t] ] : For effective mass fitting\n");
      printf("   -t_max     [ Maximum range of fitting [t] ] : For effective mass fitting\n");
      printf("   -hadron    [ hadron  name ] [ ] [ ] ... [ ] @ <- Need \"@\" at the end\n");
      printf("   -src_t     [ time slice of src point ]\n");
      printf("   -check       Check the arguments\n\n");
   }
   else if (usage_type == COUPLED_CHANNEL_POT_2x2)
   {
      printf("\n ====== Calculation & Output 2x2 coupled channel potential usage ======\n");
      printf("\n");
      printf("   -f         [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -conf_list [ Gauge configuration list ]\n");
      printf("   -idir      [ Path of the input directory ]\n");
      printf("   -odir      [ Path of the output directory ]\n");
      printf("   -time      [ Value of time slice for calculate ]\n");
      printf("   -spin      [ Spin type to calculate ]\n");
      printf("   -channel00 [ Channel name of (0, 0) component ]\n");
      printf("   -channel01 [ Channel name of (0, 1) component ]\n");
      printf("   -channel10 [ Channel name of (1, 0) component ]\n");
      printf("   -channel11 [ Channel name of (1, 1) component ]\n");
      printf("   -mass0_0   [ 1st-hadron mass of 1st-channel (Lattice Unit) ]\n");
      printf("   -mass0_1   [ 2nd-hadron mass of 1st-channel (Lattice Unit) ]\n");
      printf("   -mass1_0   [ 1st-hadron mass of 2nd-channel (Lattice Unit) ]\n");
      printf("   -mass1_1   [ 2nd-hadron mass of 2nd-channel (Lattice Unit) ]\n");
      printf("   -Zfactor01 [ Sqrt ratio of point Z-factor (=sqrt[Z_1/Z_2]) of (0, 1) component ]\n");
      printf("   -src_t     [ time slice of src point ]\n");
      printf("   -check       Check the arguments\n\n");
   }
   else if (usage_type == COUPLED_CHANNEL_POT_3x3)
   {
      printf("\n ====== Calculation & Output 3x3 coupled channel potential usage ======\n");
      printf("\n");
      printf("   -f         [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -conf_list [ Gauge configuration list ]\n");
      printf("   -idir      [ Path of the input directory ]\n");
      printf("   -odir      [ Path of the output directory ]\n");
      printf("   -time      [ Value of time slice for calculate ]\n");
      printf("   -spin      [ Spin type to calculate ]\n");
      printf("   -channel00 [ Channel name of (0, 0) component ]\n");
      printf("   -channel01 [ Channel name of (0, 1) component ]\n");
      printf("   -channel02 [ Channel name of (0, 2) component ]\n");
      printf("   -channel10 [ Channel name of (1, 0) component ]\n");
      printf("   -channel11 [ Channel name of (1, 1) component ]\n");
      printf("   -channel12 [ Channel name of (1, 2) component ]\n");
      printf("   -channel20 [ Channel name of (2, 0) component ]\n");
      printf("   -channel21 [ Channel name of (2, 1) component ]\n");
      printf("   -channel22 [ Channel name of (2, 2) component ]\n");
      printf("   -mass0_0   [ 1st-hadron mass of 1st-channel (Lattice Unit) ]\n");
      printf("   -mass0_1   [ 2nd-hadron mass of 1st-channel (Lattice Unit) ]\n");
      printf("   -mass1_0   [ 1st-hadron mass of 2nd-channel (Lattice Unit) ]\n");
      printf("   -mass1_1   [ 2nd-hadron mass of 2nd-channel (Lattice Unit) ]\n");
      printf("   -mass2_0   [ 1st-hadron mass of 3rd-channel (Lattice Unit) ]\n");
      printf("   -mass2_1   [ 2nd-hadron mass of 3rd-channel (Lattice Unit) ]\n");
      printf("   -Zfactor01 [ Sqrt ratio of point Z-factor (=sqrt[Z_1/Z_2]) of (0, 1) component ]\n");
      printf("   -Zfactor02 [ Sqrt ratio of point Z-factor (=sqrt[Z_1/Z_2]) of (0, 2) component ]\n");
      printf("   -Zfactor12 [ Sqrt ratio of point Z-factor (=sqrt[Z_1/Z_2]) of (1, 2) component ]\n");
      printf("   -src_t     [ time slice of src point ]\n");
      printf("   -check       Check the arguments\n\n");
   }
   else if (usage_type == JACK_KNIFE_BIN_CHECK)
   {
      printf("\n ====== Check the bin-size dependence of Jack Knife samples usage ======\n");
      printf("\n");
      printf("   -f         [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -conf_list [ Gauge configuration list ]\n");
      printf("   -idir      [ Path of the input directory ]\n");
      printf("   -ofile     [ Path of the output file ]\n");
      printf("   -hadron    [ hadron name ]\n");
      printf("   -t_min     [ Minimum value of time slice ]\n");
      printf("   -t_max     [ Maximum value of time slice ]\n");
      printf("   -src_t     [ time slice of src point ]\n");
      printf("   -check       Check the arguments\n\n");
   }
   else if (usage_type == WAVE_DECOMPRESSION)
   {
      printf("\n ====== De-compression of wave function usage ======\n");
      printf("\n");
      printf("   -f       [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -ipath   [ Path of the input directory ]\n");
      printf("   -opath   [ Path of the output directory ]\n");
      printf("   -channel [ channel name 1 ] [2] [3] ... [n] @ <- Need \"@\" at the end\n");
      printf("   -check     Check the arguments\n\n");
   }
   else if (usage_type == FOLDING_POTENTIAL_GAUSS)
   {
      printf("\n ======   Calculation of the folding potential (Gaussian-type) usage   ======\n");
      printf("\n");
      printf("   -f         [ Path of the input arguments file ] <- Must be required !\n");
      printf("   -ifile     [ Path of the input fit data file ]\n");
      printf("   -ofile     [ Path of the output data file ]\n");
      printf("   -A         [ #.nucleus ]\n");
      printf("   -DensName  [ The name of density-type ]\n");
      printf("   -Nplot     [ #.plot for output potential ]\n");
      printf("   -max_r     [ Max range for output potential ]\n\n");
      printf("   -phase       Calculate the phase shift\n");
      printf("   -E_min     [ Minimum Enargy (MeV) for output ]  <- For phase shift\n");
      printf("   -E_max     [ Maximum Enargy (MeV) for output ]  <- For phase shift\n");
      printf("   -E_div     [ Enargy devision (MeV) for output ] <- For phase shift\n");
      printf("   -mass      [ Reduced mass [Mev/c2] ]            <- For phase shift\n\n");
      printf("   -check       Check the arguments\n\n");
   }
}
