//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup gauge
 * @brief   Main part for the coulomb gfix check
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <common/common_inc.h>
#include <gauge/gauge_link.h>

int main(int argc, char **argv) {

   if (argc==1) {
      printf(" usage : %s [space size] [time size] [file]\n", argv[0]);
      exit(1);
   }
   else if (argc!=4) {
      printf(" ERROR : invalid number of arguments.\n");
      printf(" usage : %s [space size] [time size] [file]\n", argv[0]);
      exit(1);
   }
   int    ssize = atoi(argv[1]);
   int    tsize = atoi(argv[2]);
   size_t csize = ssize * ssize * ssize * tsize * 4 * 3 * 3 * 2;
   
   ifstream ifs(argv[3], ios::in | ios::binary);
   if (!ifs) {
      printf(" ERROR : The file \"%s\" is  not found.\n", argv[3]);
      exit(1);
   }
   ifs.seekg(236);
   double *conf = new double[csize];
   ifs.read((char*)&conf[0], sizeof(double)*csize);
   ifs.close();
   
   printf(" plaq = %lf\n", plaquette(conf, ssize, tsize));
   
   cdouble ret = check_coulomb_gfix(conf, ssize, tsize);
   printf(" mean of square all elements = (%e,%e)\n", ret.real(), ret.imag());
   
   return 0;
}
