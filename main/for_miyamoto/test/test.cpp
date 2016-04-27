//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup test or debug
 * @author  Takaya Miyamoto
 * @brief   Main part for test 
 */
//--------------------------------------------------------------------------

#include <common/common_inc.h>
#include <gauge/matrix_func.h>

int main(int argc, char **argv) {
   
   int num_dim = 16*16*16;
   
   double eigen;
   
   double *alpha = new double[num_dim];
   double *beta  = new double[num_dim];
   double *pot   = new double[num_dim];
   
   for (int i=0; i<num_dim; i++) pot[i] = 0.0;
   
   to_tridiag_Lanczos_for_schrodinger(alpha, beta, pot, 16, 0.5, 0.1);
   
   eigenvalue_bi_section(alpha, beta, &eigen, num_dim, 1, 0.000000001);
   
   double *tmp_v = new double[num_dim];
   
   eigenvector_inverse_iter(alpha, beta, eigen, tmp_v, num_dim, 0.000000001);
   
   for (int x=0; x<16; x++) for (int y=0; y<16; y++) for (int z=0; z<16; z++)
   {
      if (x>8 || y>x || z>y) continue;
      
      printf("%lf %e\n", sqrt( x*x + y*y + z*z ), tmp_v[x+16*(y+16*z)]);
   }
   
   delete [] tmp_v;
   delete [] alpha;
   delete [] beta;
   delete [] pot;
   
   return 0;
}
