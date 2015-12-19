//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup test
 * @brief   Main part for test or debug
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <common/analysis.h>
#include <NBSwave/NBSwave.h>

int analysis::Nconf = 10;
int analysis::xSIZE = 16;
int analysis::ySIZE = 16;
int analysis::zSIZE = 16;
int analysis::tSIZE = 32;

size_t NBSwave::xyzSIZE;
size_t NBSwave::NBSSIZE;
size_t NBSwave::xyznSIZE;
size_t NBSwave::NBSnSIZE;

char analysis::data_list[MAX_N_DATA][MAX_LEN_PATH];

int main(int argc, char **argv) {

   analysis::Nconf = analysis::set_data_list
   (  "/Users/miiya/Desktop/work_kyoto/test_data/cp-pacs+JLQCD_ens3_193conf"
    , "0", "0", "0", "0", "CG05", "CG05", argv[1] );
   
   printf(" conf = %d\n", analysis::Nconf);
   
   if (analysis::machine_is_little_endian()) printf("LITTLE\n");
   else printf("BIG\n");
   
//   CHANNEL_TYPE channel("Pro-Lam_Pro-Lam");
//   NBS_WAVE lambdaN;
//   lambdaN.set_NBS(channel, 8, true, false);
//   lambdaN.projection(0,0);
//   lambdaN.make_JK_sample_projNBS(1);
//   lambdaN.output_NBS_err("./results");
   
//   HADRON_TYPE hadron("pion");
//   CORRELATOR corr;
//   corr.set_corr(hadron);
//   corr.output_effmass_err("./results");
   
//   R_CORRELATOR Rcorr;
//   Rcorr.set_Rcorr(channel, 8, true, 0, 0, false);
//   Rcorr.output_Rcorr_err("./results");
   
   return 0;
}
