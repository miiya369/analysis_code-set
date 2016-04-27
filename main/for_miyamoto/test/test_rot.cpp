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
   
   NBS_WAVE_ORG     *Wave_org  = new NBS_WAVE_ORG;
   NBS_WAVE_SRC_PRJ *Wave_sprj = new NBS_WAVE_SRC_PRJ;
   
   CONFIG<NBS_WAVE> *Wave     = new CONFIG<NBS_WAVE>;
   CONFIG<NBS_WAVE> *S12_Wave = new CONFIG<NBS_WAVE>;
   
   CONFIG<NBS_WAVE> *Wave_bar     = new CONFIG<NBS_WAVE>;
   CONFIG<NBS_WAVE> *S12_Wave_bar = new CONFIG<NBS_WAVE>;
   
   char outfile_name[1024];
   
   CHANNEL_TYPE channel("Pro-Lam_Pro-Lam");
   SPIN_TYPE    spin   ("S1_Sz+1");
   SPIN_TYPE    spin_src("S1_Sz0");
   
   int time  = 9;
   
   for (int conf=0; conf<analysis::Nconf; conf++)
   {
      Wave_org->set( channel, time, conf, true, true );
      
      NBSwave::spin_projection(*Wave_org, *Wave_sprj, spin_src);
      
      NBSwave::spin_projection(*Wave_sprj, (*Wave)(conf), spin);
      
      NBSwave::S12_psi(*Wave_sprj, (*S12_Wave)(conf), spin);
      
      NBSwave::Swave_division( (*Wave)(conf)    , (*Wave_bar)(conf) );
      NBSwave::Swave_division( (*S12_Wave)(conf), (*S12_Wave_bar)(conf) );
      
//      NBSwave::remove_angular((*Wave_bar)(conf),     1, 1);
//      NBSwave::remove_angular((*S12_Wave_bar)(conf), 1, 1);
   }
   
   int output_num = 10;
   
   snprintf(  outfile_name, sizeof(outfile_name), "results/test_tensor/wave%d", output_num );
   analysis::output_data_err( *Wave, outfile_name, true );
   
   snprintf(  outfile_name, sizeof(outfile_name), "results/test_tensor/S12wave%d", output_num );
   analysis::output_data_err( *S12_Wave, outfile_name, true );
   
   snprintf(  outfile_name, sizeof(outfile_name), "results/test_tensor/wave_bar%d", output_num );
   analysis::output_data_err( *Wave_bar, outfile_name, true );
   
   snprintf(  outfile_name, sizeof(outfile_name), "results/test_tensor/S12wave_bar%d", output_num );
   analysis::output_data_err( *S12_Wave_bar, outfile_name, true );
   
   return 0;
}
