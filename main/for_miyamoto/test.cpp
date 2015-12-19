//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup test
 * @brief   Main part for test or debug
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <NBSwave/NBSwave.h>

int analysis::Nconf = 10;
int analysis::xSIZE = 16;
int analysis::ySIZE = 16;
int analysis::zSIZE = 16;
int analysis::tSIZE = 32;

char analysis::data_list[MAX_N_DATA][MAX_LEN_PATH];

int main(int argc, char **argv) {
   
   analysis::Nconf = analysis::set_data_list(
   "/Users/miiya/Desktop/work_kyoto/test_data/cp-pacs+JLQCD_ens3_193conf"
   , "0", "0", "0", "0", "CG05", "CG05"
   , "./arg/conf_list/conf_list.cp-pacs+JLQCD_ens3_190confs" );

   CHANNEL_TYPE ch("Pro-Lam_Pro-Lam");
   NBS_WAVE nbs;
   nbs.set_NBS(ch,8,5,true,0,0,0,false);
   nbs.projection();
   
   for (      int z=0; z<analysis::zSIZE; z++)
      for (   int y=0; y<analysis::ySIZE; y++)
         for (int x=0; x<analysis::xSIZE; x++) {
               if (x>analysis::xSIZE/2 || y>x || z>y) continue;
            
            int x_shift = 0;
            int y_shift = 0;
            int z_shift = 0;
            if (x > analysis::xSIZE/2) x_shift = analysis::xSIZE;
            if (y > analysis::ySIZE/2) y_shift = analysis::ySIZE;
            if (z > analysis::zSIZE/2) z_shift = analysis::zSIZE;
            // make r for each coodinate with periodic boundary condition
            double R = sqrt(  (x - x_shift)*(x - x_shift)
                     + (y - y_shift)*(y - y_shift)
                     + (z - z_shift)*(z - z_shift) );
            
            printf("%lf %e\n",R,nbs(x,y,z).real());
         }
   
   return 0;
}
