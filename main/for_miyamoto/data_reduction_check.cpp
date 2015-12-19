
#include <common/analysis.h>
#include <potential/potential.h>

int analysis::Nconf=10;
int analysis::xSIZE=16;
int analysis::ySIZE=16;
int analysis::zSIZE=16;
int analysis::tSIZE=16;

size_t NBSwave::xyzSIZE;
size_t NBSwave::NBSSIZE;
size_t NBSwave::xyznSIZE;
size_t NBSwave::NBSnSIZE;

char analysis::data_list[MAX_N_DATA][MAX_LEN_PATH];

int main(int argc, char **argv) {
   
   analysis::Nconf = analysis::set_data_list(
     "/Users/miiya/Desktop/work_kyoto/test_data/cp-pacs+JLQCD_ens3_193conf"
   , "0", "0", "0", "0", "CG05", "CG05"
   , "./arg/conf_list/conf_list.cp-pacs+JLQCD_ens3_190confs" );
   
   CHANNEL_TYPE ch("Pro-Lam_Pro-Lam");
   POTENTIAL pot;
   pot.set_pot(ch, 8, true, false, 0, 0, 0.5);
   pot.calc_laplacian();
   
   int min_Ndata = 999999;
   int tmp_Ndata;
   int data_count = 0;
   bool *data_checker = new bool[NBSwave::xyzSIZE];
   for (size_t i=0; i<NBSwave::xyzSIZE; i++)
      data_checker[i] = true;
   
   //============== If data reduction is wrong, chenge this one =============//
   double reduction_precision = 0.000000000001;
   //========================================================================//
   
   printf(" @ Data reduction          |   0%%");
   for (size_t i=0; i<analysis::Nconf; i++) {
      tmp_Ndata = NBSwave::xyzSIZE;
      
      for (size_t j=0; j<NBSwave::xyzSIZE-1; j++) {
         if (!data_checker[j]) continue;
         
         for (size_t k=j+1; k<NBSwave::xyzSIZE; k++) {
            if(   pot(j+NBSwave::xyzSIZE*i).real() + reduction_precision
               >= pot(k+NBSwave::xyzSIZE*i).real()
               &&
                  pot(j+NBSwave::xyzSIZE*i).real() - reduction_precision
               <= pot(k+NBSwave::xyzSIZE*i).real() ) {
               
               data_checker[k] = false;
               tmp_Ndata--;
            }
         }
      }
      if (       min_Ndata >  tmp_Ndata) {
         min_Ndata = tmp_Ndata;
         data_count = 1;
      } else if (min_Ndata == tmp_Ndata) {
         data_count++;
      }
      printf("\b\b\b\b%3.0lf%%",double(i+1)/double(analysis::Nconf)*100);
      fflush(stdout);
   }
   printf("\n @@@ #.data     = %d       ### FOR DEBUG\n", min_Ndata);
   printf(" @@@ Data count = %d / %d ### FOR DEBUG\n"
          , data_count, analysis::Nconf);
   data_count = 0;
   for (size_t j=0; j<NBSwave::xyzSIZE; j++)
      if(data_checker[j]){
         printf("%ld, ",j);
         data_count++;
      }
   printf("\n count = %d\n",data_count);
   data_count = 0;
   for (size_t j=0; j<NBSwave::data_reduction(-1); j++)
      printf("%ld, ",NBSwave::data_reduction(j));
   
   return 0;
}

