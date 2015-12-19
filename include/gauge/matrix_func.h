//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup gauge
 * @brief   Header file for some function of complex matrix
 * @author  Takaya Miyamoto
 * @since   Mon Oct  5 16:25:44 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef COMPLEX_MATRIX_FUNC_H
#define COMPLEX_MATRIX_FUNC_H

#include <complex>

#ifdef _OPENMP
#include <omp.h>
#endif

#ifndef COMP_IMAG
#define COMP_IMAG std::complex<double>(0.0,1.0)
#endif
#ifndef COMP_ZERO
#define COMP_ZERO std::complex<double>(0.0,0.0)
#endif

#define idx(i,j) (j + Ndim * i)

//--------------------------------------------------------------------------
/**
 * @brief The function for calculate AX=B using Gauss–Jordan elimination
 */
//--------------------------------------------------------------------------
void solve_mat_gauss( double *Amat, double *Xmat, double *Bmat, int Ndim ) {
   
   double  tmp1, tmp2;
   double *mat = new double[Ndim*Ndim];
   for (int i=0; i<Ndim*Ndim; i++) {
      mat[ i] = Amat[i];
      Xmat[i] = Bmat[i];
   }
   
   for (int i=0; i<Ndim; i++) {
      tmp1 = mat[idx(i,i)];
      for (int j=0; j<Ndim; j++) {
         Xmat[idx(i,j)] /= tmp1;
         mat[ idx(i,j)] /= tmp1;
      }
      for (   int ii=i+1; ii<Ndim; ii++) {
         tmp2 = mat[idx(ii,i)];
         for (int jj=0;   jj<Ndim; jj++) {
            Xmat[idx(ii,jj)] -= Xmat[idx(i,jj)] * tmp2;
            mat[ idx(ii,jj)] -= mat[ idx(i,jj)] * tmp2;
         }
      }
   } // M -> U
   
   for (      int i =Ndim-1; i >0;    i--) {
      for (   int ii=i-1;    ii>=0;   ii--) {
         tmp2 = mat[idx(ii,i)];
         for (int jj=0;      jj<Ndim; jj++) {
            Xmat[idx(ii,jj)] -= Xmat[idx(i,jj)] * tmp2;
            mat[ idx(ii,jj)] -= mat[ idx(i,jj)] * tmp2;
         }
      }
   } // U -> E
   
//   for (int i=0; i<Ndim; i++) {
//      for (int j=0; j<Ndim; j++) printf("%1.2lf ", mat[idx(i,j)]);
//      printf("\n");
//   }  printf("\n");   // For Debug
   
   delete [] mat;
}
void solve_mat_gauss(  std::complex<double> *Amat, std::complex<double> *Xmat
                     , std::complex<double> *Bmat, int Ndim ) {
   
   std::complex<double>  tmp1, tmp2;
   std::complex<double> *mat = new std::complex<double>[Ndim*Ndim];
   for (int i=0; i<Ndim*Ndim; i++) {
      mat[ i] = Amat[i];
      Xmat[i] = Bmat[i];
   }
   
   for (int i=0; i<Ndim; i++) {
      tmp1 = mat[idx(i,i)];
      for (int j=0; j<Ndim; j++) {
         Xmat[idx(i,j)] /= tmp1;
         mat[ idx(i,j)] /= tmp1;
      }
      for (   int ii=i+1; ii<Ndim; ii++) {
         tmp2 = mat[idx(ii,i)];
         for (int jj=0;   jj<Ndim; jj++) {
            Xmat[idx(ii,jj)] -= Xmat[idx(i,jj)] * tmp2;
            mat[ idx(ii,jj)] -= mat[ idx(i,jj)] * tmp2;
         }
      }
   } // M -> U
   
   for (      int i =Ndim-1; i >0;    i--) {
      for (   int ii=i-1;    ii>=0;   ii--) {
         tmp2 = mat[idx(ii,i)];
         for (int jj=0;      jj<Ndim; jj++) {
            Xmat[idx(ii,jj)] -= Xmat[idx(i,jj)] * tmp2;
            mat[ idx(ii,jj)] -= mat[ idx(i,jj)] * tmp2;
         }
      }
   } // U -> E
   
//      for (int i=0; i<Ndim; i++) {
//         for (int j=0; j<Ndim; j++)
//            printf("(%1.2lf,%1.2lf) "
//                   , mat[idx(i,j)].real(), mat[idx(i,j)].imag());
//         printf("\n");
//      }  printf("\n");   // For Debug
   
   delete [] mat;
}

//--------------------------------------------------------------------------
/**
 * @brief The function for inverse matrix using Gauss–Jordan elimination
 */
//--------------------------------------------------------------------------
void inv_mat_gauss( double *imat, double *inv_mat, int Ndim ) {
   
   double *mat = new double[Ndim*Ndim];
   for (int i=0; i<Ndim; i++) for (int j=0; j<Ndim; j++)
      i==j ? mat[idx(i,j)] = 1.0 : mat[idx(i,j)] = 0.0;
   
   solve_mat_gauss( imat, inv_mat, mat, Ndim );
   
   delete [] mat;
}
void inv_mat_gauss(  std::complex<double> *imat
                   , std::complex<double> *inv_mat, int Ndim ) {
   
   std::complex<double> *mat = new std::complex<double>[Ndim*Ndim];
   for (int i=0; i<Ndim; i++) for (int j=0; j<Ndim; j++)
      i==j ? mat[idx(i,j)] = 1.0 : mat[idx(i,j)] = COMP_ZERO;
   
   solve_mat_gauss( imat, inv_mat, mat, Ndim );
   
   delete [] mat;
}

//--------------------------------------------------------------------------
/**
 * @brief The function for calculate eigenvalue using power method
 */
//--------------------------------------------------------------------------
void eigenvalue_power(  double *imat, double *eigenvalue, int Ndim, int Nvalue
                      , double stop_cnd ) {
   
   double *tmp_v0 = new double[Ndim];
   double *tmp_v1 = new double[Ndim];
   double  tmp1, tmp2, tmp_eigen0 = 999999, tmp_eigen1 = 999;
   
   tmp_v0[0] = 1.0;   // initialize v = (1,0,0,0,...)
   for (int i=1; i<Ndim; i++) tmp_v0[i] = 0.0;
   
//   int count = 0;   // For Debug
   while (fabs(tmp_eigen0-tmp_eigen1) > stop_cnd) {
      tmp_eigen0 = tmp_eigen1;
      
      tmp1 = 0.0;   tmp2 = 0.0;
      for (int i=0; i<Ndim; i++) {
         tmp_v1[i] = 0.0;
         for (int j=0; j<Ndim; j++) tmp_v1[i] += imat[idx(i,j)] * tmp_v0[j];
         tmp1 += tmp_v1[i] * tmp_v1[i];
      }
      for (int i=0; i<Ndim; i++) {
         tmp_v1[i] /= tmp1;
         tmp2      += tmp_v1[i] * tmp_v0[i];
         tmp_v0[i]  = tmp_v1[i];
      }
      tmp_eigen1 = 1.0/tmp2;
      
//      count++;   // For Debug
//      printf("%d %e %e\n", count, tmp_eigen1, fabs(tmp_eigen0-tmp_eigen1));
   }
   eigenvalue[0] = tmp_eigen1;
//   for (int i=0; i<Ndim; i++) printf("%d %lf\n", i, tmp_v1[i]);   // For Debug
   
   delete [] tmp_v0;
   delete [] tmp_v1;
}

#undef idx
#endif
