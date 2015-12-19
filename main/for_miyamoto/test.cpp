//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup test or debug
 * @author  Takaya Miyamoto
 * @brief   Main part for test 
 */
//--------------------------------------------------------------------------

#include <observable/phase_shift.h>

int main(int argc, char **argv) {
   
   analysis::Nconf=1;
   analysis::xSIZE=32;
   analysis::ySIZE=32;
   analysis::zSIZE=32;
   analysis::tSIZE=64;
   
//   int Nparam, func_type;
//   
//   observable::input_param( "../other/param1"
//                           , analysis::Nconf, Nparam, func_type );
//   
//   double *param0 = new double[analysis::Nconf * Nparam];
//   double *param1 = new double[analysis::Nconf * Nparam];
//   double *param2 = new double[analysis::Nconf * Nparam];
//   double *param3 = new double[analysis::Nconf * Nparam];
//   
//   observable::input_param( "../other/param1", param0 );
//   observable::input_param(
//      "../other/Pro-Lam_Pro-Lam_potential_S0Sz0_fitparam_3G_t9", param0 );
//   observable::input_param( "../other/param3", param1 );
//   observable::input_param( "../other/param3", param2 );
//   observable::input_param( "../other/param4", param3 );
   
   CONFIG<PHASE_SHIFT> phase1(analysis::Nconf);
//   CONFIG<PHASE_SHIFT> phase2(analysis::Nconf);
   
//   double mass[2] = { 721, 726 };
//   double mass[2] = { 500, 600 };
   double mass[1] = { 200 };
//   double mass[2] = { 1394.690075, 1493.555631 };
//   double mass_threshold[2] = { 2888.245706, 2911.42267 };
   double mass_threshold[2] = { 0, 50 };
//   double mass_threshold[1] = { 2888.245706 };
//   double paraparam[2] = {1,300};
//   double paraparam[8] = {1,10,1,15,1,15,1,60};
   double paraparam[6] = {2,1,3,8,-3.5,5};
   
   
//   cdouble Smat;
//   double masmas = 500;
//   for (   double Re_k=-0.1; Re_k<=0.1; Re_k+=0.005)
//      for (double Im_k=-0.1; Im_k<=0.1; Im_k+=0.005) {
//         Smat = observable::Smat_Cplane(  Re_k, Im_k, paraparam
//                                        , masmas, 6, 5, 0.0907, 0.001, 6);
////         Smat = log(Smat)/(2.0*COMP_IMAG) * 180.0/PI;
//         printf("%lf %lf %lf %lf\n", Re_k, Im_k, Smat.real(), Smat.imag());
//      }
//   return 0;
   
   
   int counter = 0;
   printf(" @ phase shift calculating       |   0%%");
#ifdef _OPENMP
#pragma omp parallel for
#endif
   for (int conf=0; conf<analysis::Nconf; conf++) {
      
      PHASE_SHIFT phase[2];
      phase[0].mem_alloc( 0, 1000, 1 );
//      phase[1].mem_alloc( -100, 100, 1 );
      
      phase1(conf).mem_alloc( 0, 1000, 1 );
//      phase2(conf).mem_alloc( -100, 100, 1 );
      
//      double *tmp_param = new double[2*2*Nparam];
//      for (int n=0; n<Nparam; n++) {
//         tmp_param[n+Nparam*0] = param0[conf+analysis::Nconf*n];
//         tmp_param[n+Nparam*1] = param1[conf+analysis::Nconf*n];
//         tmp_param[n+Nparam*2] = param2[conf+analysis::Nconf*n];
//         tmp_param[n+Nparam*3] = param3[conf+analysis::Nconf*n];
//      }
      int Nchannel = 1;
      
      observable::calc_phase_shift_dif(  phase, paraparam, mass, mass_threshold
                                       , Nchannel, 6, 5, 0.0907, 0.001, 10);

//      observable::calc_phase_shift_dif(  phase, tmp_param, mass
//                                       , mass_threshold, Nchannel, Nparam
//                                       , func_type, 0.0907, 0.0001, 6);

//      observable::calc_phase_shift_int(  phase, tmp_param, mass
//                                       , mass_threshold, Nchannel, Nparam
//                                       , func_type, 0.0907, 30);
      
      for (size_t iE=0; iE<phase[0].info_data_size(); iE++) {
         phase1(conf)(iE) = phase[0](iE);
//         phase2(conf)(iE) = phase[1](iE);
      }
      
//      delete [] tmp_param;
      counter++;
      printf("\b\b\b\b%3.0lf%%",double(counter)/double(analysis::Nconf)*100);
      fflush(stdout);
   } // conf
   printf("\n");

   observable::output_phase_shift_err( "./tmp/test1", phase1 );
//   observable::output_phase_shift_err( "./tmp/test2", phase2 );
   
//   delete [] param0;
//   delete [] param1;
//   delete [] param2;
//   delete [] param3;
   
   return 0;
}
