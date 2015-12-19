
#include <common/analysis.h>
#include <potential/potential.h>

int analysis::Nconf=10;
int analysis::xSIZE=16;
int analysis::ySIZE=16;
int analysis::zSIZE=16;
int analysis::tSIZE=16;

int main(int argc, char **argv) {
   
//   analysis::Nconf = analysis::set_data_list(
//     "/Users/miiya/Desktop/work_kyoto/test_data/cp-pacs+JLQCD_ens3_193conf"
//   , "0", "0", "0", "0", "CG05", "CG05"
//   , "./arg/conf_list/conf_list.cp-pacs+JLQCD_ens3_190confs" );
//   
//   CHANNEL_TYPE ch("Pro-Lam_Pro-Lam");
//   POTENTIAL pot;
//   pot.set_pot(ch, 8, true, false, 0, 0, 0, 0.5);
//   pot.calc_laplacian();
//   
//   int min_Ndata = 999999;
//   int tmp_Ndata;
//   int data_count = 0;
//   bool *data_checker = new bool[NBSwave::xyzSIZE];
//   for (size_t i=0; i<NBSwave::xyzSIZE; i++)
//      data_checker[i] = true;
//   
//   //============== If data reduction is wrong, chenge this one =============//
//   double reduction_precision = 0.000000000001;
//   //========================================================================//
//   
//   printf(" @ Data reduction          |   0%%");
//   for (size_t i=0; i<analysis::Nconf; i++) {
//      tmp_Ndata = NBSwave::xyzSIZE;
//      
//      for (size_t j=0; j<NBSwave::xyzSIZE-1; j++) {
//         if (!data_checker[j]) continue;
//         
//         for (size_t k=j+1; k<NBSwave::xyzSIZE; k++) {
//            if(   pot(j+NBSwave::xyzSIZE*i).real() + reduction_precision
//               >= pot(k+NBSwave::xyzSIZE*i).real()
//               &&
//                  pot(j+NBSwave::xyzSIZE*i).real() - reduction_precision
//               <= pot(k+NBSwave::xyzSIZE*i).real() ) {
//               
//               data_checker[k] = false;
//               tmp_Ndata--;
//            }
//         }
//      }
//      if (       min_Ndata >  tmp_Ndata) {
//         min_Ndata = tmp_Ndata;
//         data_count = 1;
//      } else if (min_Ndata == tmp_Ndata) {
//         data_count++;
//      }
//      printf("\b\b\b\b%3.0lf%%",double(i+1)/double(analysis::Nconf)*100);
//      fflush(stdout);
//   }
//   printf("\n @@@ #.data     = %d       ### FOR DEBUG\n", min_Ndata);
//   printf(" @@@ Data count = %d / %d ### FOR DEBUG\n"
//          , data_count, analysis::Nconf);
//   data_count = 0;
//   ofstream ofs1("./results/reduction_point", ios::out);
//   ofstream ofs2("./results/all_point", ios::out);
//   for (size_t j=0; j<NBSwave::xyzSIZE; j++){
//      int x, y, z;
//      analysis::convert_xyz(&x, &y, &z, j);
//      ofs2 << x << " " << y << " " << z << endl;
//      if(data_checker[j]){
//         ofs1 << x << " " << y << " " << z << endl;
//         //         printf("%ld, ",j);
//         data_count++;
//      }
//   }
//   ofs1.close();
//   ofs2.close();
//   printf("\n count = %d\n",data_count);
//   printf("reduced Ndata = %d, ",NBSwave::reduced_Ndata());
   
   return 0;
}

