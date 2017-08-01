//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup test or debug
 * @author  Takaya Miyamoto
 * @brief   Main part for test 
 */
//--------------------------------------------------------------------------

#include <NBSwave/NBSwave.h>

int main(int argc, char **argv) {
   
   analysis::set_data(32,32,32,64,20);
   analysis::set_data_list(  "../data/pacs-cs_ens2_64src_bin20"
                           , "A64", "000", "000", "000", "CG05", "CG05"
                           , "arg/conf_list/conf_list.pacs-cs_ens2_bin020" );
   
   NBSwave::rot_matrix_init();
   
   NBS_WAVE_ORG     Wave_org;
   NBS_WAVE_SRC_PRJ Wave_prj;
   NBS_WAVE_SRC_PRJ S12Wave_prj;
   NBS_WAVE_SRC_PRJ Wave_prj_bar;
   
   CONFIG<NBS_WAVE> Wave[4];
   
   char outfile_name[1024];
   
   CHANNEL_TYPE channel("Pro-Lam_Pro-Lam");
   SPIN_TYPE    spin   ("S1_Sz0");
   
   int time = 9;
   
   for (int conf=0; conf<analysis::Nconf; conf++)
   {
      Wave_org.set( channel, time, conf, true, true );
      
      NBSwave::spin_projection(Wave_org, Wave_prj, spin);
      
      NBSwave::S12_psi(Wave_prj, S12Wave_prj);
      
      NBSwave::Swave_division(S12Wave_prj, Wave_prj_bar);
      
      NBSwave::parity_projection(Wave_prj_bar);
      
      NBSwave::mult_YDstar(Wave_prj_bar, spin);
      
      for (int ab=0; ab<4; ab++)
      {
         Wave[ab](conf).mem_alloc();
         for (int n=0; n<analysis::xSIZE*analysis::ySIZE*analysis::zSIZE; n++)
            Wave[ab](conf)(n) = Wave_prj_bar(n,ab);
      }
   }
   
   int output_num = 8;
   
   snprintf( outfile_name, sizeof(outfile_name), "results/test_tensor/wave0_%d", output_num );
   analysis::output_data_err( Wave[0], outfile_name, true );
   
   snprintf( outfile_name, sizeof(outfile_name), "results/test_tensor/wave1_%d", output_num );
   analysis::output_data_err( Wave[1], outfile_name, true );
   
   snprintf( outfile_name, sizeof(outfile_name), "results/test_tensor/wave2_%d", output_num );
   analysis::output_data_err( Wave[2], outfile_name, true );
   
   snprintf( outfile_name, sizeof(outfile_name), "results/test_tensor/wave3_%d", output_num );
   analysis::output_data_err( Wave[3], outfile_name, true );
   
   return 0;
}
