//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup gauge
 * @brief   Main part for the coulomb gfix using gauge fix matrix
 * @author  Takaya Miyamoto
 * @since   Fri Nov 13 05:58:58 JST 2015
 */
//--------------------------------------------------------------------------

#include <common/common_inc.h>
#include <gauge/gauge_link.h>

int main(int argc, char **argv)
{
   if (argc!=5) {
      printf(" usage : %s [space size] [time size] [conf] [Gmat]\n", argv[0]);
      printf(" usage : The gauge configuration is must be binary ildg format.\n");
      return -1;
   }
   int    ssize = atoi(argv[1]);
   int    tsize = atoi(argv[2]);
   size_t conf_size = ssize * ssize * ssize * tsize * 4 * 3 * 3 * 2;
   size_t gmat_size = ssize * ssize * ssize * tsize * 3 * 3 * 2;
   
   ifstream ifs1(argv[3], ios::in | ios::binary);
   ifstream ifs2(argv[4], ios::in | ios::binary);
   if (!ifs1) {
      printf(" ERROR : The file \"%s\" is  not found.\n", argv[3]);
      return -1;
   }
   if (!ifs2) {
      printf(" ERROR : The file \"%s\" is  not found.\n", argv[4]);
      return -1;
   }
   double *conf = new double[conf_size];
   double *gmat = new double[gmat_size];
   ifs1.read((char*)&conf[0], sizeof(double)*conf_size);   ifs1.close();
   ifs2.read((char*)&gmat[0], sizeof(double)*gmat_size);   ifs2.close();

   printf(" @ before plaq = %lf\n", plaquette(conf, ssize, tsize));
   
   gfix_by_gmat(conf, gmat, ssize, tsize);
   
   printf(" @ after plaq  = %lf\n", plaquette(conf, ssize, tsize));
   
   delete [] conf;
   delete [] gmat;
   
   return 0;
}
