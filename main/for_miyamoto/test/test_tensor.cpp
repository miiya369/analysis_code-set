//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup test or debug
 * @author  Takaya Miyamoto
 * @brief   Main part for test 
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>

int main(int argc, char **argv) {
   
   analysis::set_data(32,32,32,64,20);
   analysis::set_data_list(  "../data/pacs-cs_ens2_64src_bin20"
                           , "A64", "000", "000", "000", "CG05", "CG05"
                           , "arg/conf_list/conf_list.pacs-cs_ens2_bin020" );
   
   NBSwave::rot_matrix_init();
   
   NBS_WAVE_ORG     Wave_org;
   NBS_WAVE_SRC_PRJ Wave_sprj, S12Wave_sprj;
   CORRELATOR Corr1;
   CORRELATOR Corr2;
   
   R_CORRELATOR S_Rcorr, S_S12_Rcorr, S_K_Rcorr, D_Rcorr, D_S12_Rcorr, D_K_Rcorr;
   
   R_CORRELATOR_SRC_PRJ Rcorr_sprj[3], S12_Rcorr_sprj, K_Rcorr_sprj;
   
   CONFIG<POTENTIAL> V_C;
   CONFIG<POTENTIAL> V_T;
   
   char   outfile_name[1024];
   double reduced_mass = 0.5;
   
   CHANNEL_TYPE channel("Pro-Lam_Pro-Lam");
   SPIN_TYPE    spin   ("S1_Sz0");
   
   int time = 9;
   
   for (int conf=0; conf<analysis::Nconf; conf++)
   {
      Corr1.set(channel.hadron1, conf, 0, "PS");
      Corr2.set(channel.hadron2, conf, 0, "PS");
      
      for (int it=time-1; it<=time+1; it++)
      {
         Wave_org.set( channel, it, conf, true, true );
         
         NBSwave::spin_projection(Wave_org, Wave_sprj, spin);
         
         if (it==time)
         {
            NBSwave::S12_psi(Wave_sprj, S12Wave_sprj);
            S12_Rcorr_sprj.set(S12Wave_sprj, Corr1, Corr2, it);
         }
         
         Rcorr_sprj[it-time+1].set(Wave_sprj, Corr1, Corr2, it);
      }
      
      potential::kernel(   K_Rcorr_sprj, Rcorr_sprj[0]
                        , Rcorr_sprj[1], Rcorr_sprj[2], reduced_mass);
      
      Rcorrelator::Swave_division (Rcorr_sprj[1], Rcorr_sprj[0]);
      Rcorrelator::spin_projection(Rcorr_sprj[1], S_Rcorr, spin);
      Rcorrelator::mult_YDstar    (Rcorr_sprj[0], spin);
      Rcorrelator::spin_projection(Rcorr_sprj[0], D_Rcorr, SPIN_1_p1);
      
      Rcorrelator::Swave_division (S12_Rcorr_sprj,Rcorr_sprj[0]);
      Rcorrelator::spin_projection(S12_Rcorr_sprj,S_S12_Rcorr, spin);
      Rcorrelator::mult_YDstar    (Rcorr_sprj[0], spin);
      Rcorrelator::spin_projection(Rcorr_sprj[0], D_S12_Rcorr, SPIN_1_p1);
      
      Rcorrelator::Swave_division (K_Rcorr_sprj,  Rcorr_sprj[0]);
      Rcorrelator::spin_projection(K_Rcorr_sprj,  S_K_Rcorr, spin);
      Rcorrelator::mult_YDstar    (Rcorr_sprj[0], spin);
      Rcorrelator::spin_projection(Rcorr_sprj[0], D_K_Rcorr, SPIN_1_p1);
      
      potential::tensor_potential(  V_C(conf), V_T(conf)
                                  , S_Rcorr, S_S12_Rcorr, S_K_Rcorr
                                  , D_Rcorr, D_S12_Rcorr, D_K_Rcorr );
   }
   
   int output_num = 1;
   
   snprintf(  outfile_name, sizeof(outfile_name), "results/test_tensor/V_C%d", output_num );
   analysis::output_data_err( V_C, outfile_name, true );

   snprintf(  outfile_name, sizeof(outfile_name), "results/test_tensor/V_T%d", output_num );
   analysis::output_data_err( V_T, outfile_name, true );
   
   return 0;
}
