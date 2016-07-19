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
   
   cdouble A[16], B[16], C[16];
   A[0] = cdouble(1423,143); A[1] = cdouble(12,143); A[2] = cdouble(123,1423); A[3] = cdouble(1623,3435);
   A[4] = cdouble(123,1413); A[5] = cdouble(6533,1343); A[6] = cdouble(1543,163); A[7] = cdouble(3423,2143);
   A[8] = cdouble(142,1423); A[9] = cdouble(1163,3143); A[10] = cdouble(733,1645); A[11] = cdouble(623,6343);
   A[12] = cdouble(423,13); A[13] = cdouble(1542,142); A[14] = cdouble(8243,2643); A[15] = cdouble(93,1343);
   
//   double A[16], B[16], C[16];
//   A[0] = 1; A[1] = 2; A[2] = 3; A[3] = 4;
//   A[4] = 4; A[5] = 3; A[6] = 2; A[7] = 1;
//   A[8] = 3; A[9] = 4; A[10] = 2; A[11] = 1;
//   A[12] = 2; A[13] = 4; A[14] = 3; A[15] = 1;
   
   print_mat(A, 4);
   
   inv_mat_gauss(A, B, 4);
   
   print_mat(B, 4);
   
   for (int i=0; i<4; i++) for (int j=0; j<4; j++) {
      C[j+4*i] = COMP_ZERO;
      for (int k=0; k<4; k++) C[j+4*i] += A[k+4*i] * B[j+4*k];
   }
   print_mat(C, 4);
   
//   int num_dim = 16*16*16;
//   
//   double eigen;
//   
//   double *alpha = new double[num_dim];
//   double *beta  = new double[num_dim];
//   double *pot   = new double[num_dim];
//   
//   for (int i=0; i<num_dim; i++) pot[i] = 0.0;
//   
//   to_tridiag_Lanczos_for_schrodinger(alpha, beta, pot, 16, 0.5, 0.1);
//   
//   eigenvalue_bi_section(alpha, beta, &eigen, num_dim, 1, 0.000000001);
//   
//   double *tmp_v = new double[num_dim];
//   
//   eigenvector_inverse_iter(alpha, beta, eigen, tmp_v, num_dim, 0.000000001);
//   
//   for (int x=0; x<16; x++) for (int y=0; y<16; y++) for (int z=0; z<16; z++)
//   {
//      if (x>8 || y>x || z>y) continue;
//      
//      printf("%lf %e\n", sqrt( x*x + y*y + z*z ), tmp_v[x+16*(y+16*z)]);
//   }
//   
//   delete [] tmp_v;
//   delete [] alpha;
//   delete [] beta;
//   delete [] pot;
   
   return 0;
}
