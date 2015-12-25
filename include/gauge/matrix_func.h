//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup gauge
 * @brief   Header file for some function of vector & matrix
 * @author  Takaya Miyamoto
 * @since   Sat Dec 19 14:44:07 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef VECTOR_AND_MATRIX_FUNC_H
#define VECTOR_AND_MATRIX_FUNC_H

#include <complex>
#include <cstddef>

#ifdef _OPENMP
#include <omp.h>
#endif

#ifndef COMP_IMAG
#define COMP_IMAG std::complex<double>(0.0,1.0)
#endif
#ifndef COMP_ZERO
#define COMP_ZERO std::complex<double>(0.0,0.0)
#endif

#define idx(i,j) ((j) + Ndim * (i))

//--------------------------------------------------------------------------
/**
 * @brief The function for initialize for matrix to all-zero elements
 */
//--------------------------------------------------------------------------
void init_mat_zero(double *mat, const size_t Ndim)
{
   for (size_t i=0; i<Ndim*Ndim; i++) mat[i] = 0.0;
}

//--------------------------------------------------------------------------
/**
 * @brief The function for initialize for matrix to unit-matrix
 */
//--------------------------------------------------------------------------
void init_mat_unit(double *mat, const size_t Ndim)
{
   for (size_t i=0; i<Ndim; i++) for (size_t j=0; j<Ndim; j++)
      mat[idx(i,j)] = (i==j ? 1.0 : 0.0);
}

//--------------------------------------------------------------------------
/**
 * @brief The function for print the elements of matrix
 */
//--------------------------------------------------------------------------
void print_mat(const double *mat, const size_t Ndim)
{
   for (size_t i=0; i<Ndim; i++)
   {
      for (size_t j=0; j<Ndim; j++) printf(" %1.1lf",mat[idx(i,j)]);
      printf("\n");
   }
   printf("\n");
}

void print_mat(const std::complex<double> *mat, const size_t Ndim)
{
   for (size_t i=0; i<Ndim; i++)
   {
      for (size_t j=0; j<Ndim; j++)
         printf(" (%1.1lf,%1.1lf)", mat[idx(i,j)].real(), mat[idx(i,j)].imag());
      printf("\n");
   }
   printf("\n");
}

//--------------------------------------------------------------------------
/**
 * @brief The function for calculate the norm of vector
 */
//--------------------------------------------------------------------------
double trace_mat(const double *mat, const size_t Ndim)
{
   double ret = 0.0;
   for (size_t i=0; i<Ndim; i++) ret += mat[idx(i,i)];
   
   return ret;
}

//--------------------------------------------------------------------------
/**
 * @brief The function for check the symmetry of the matrix
 */
//--------------------------------------------------------------------------
double check_mat_symmetry(const double *mat, const size_t Ndim)
{
   double ret = 0.0;
   for (size_t i=0; i<Ndim; i++) for (size_t j=0; j<Ndim; j++)
      ret += pow(mat[idx(i,j)] - mat[idx(j,i)], 2);
   
   return sqrt(ret);
}

//--------------------------------------------------------------------------
/**
 * @brief The function for print the elements of vector
 */
//--------------------------------------------------------------------------
void print_vec(const double *vec, const size_t Ndim)
{
   for (size_t i=0; i<Ndim; i++) printf(" %lf",vec[i]);
   printf("\n");
}

//--------------------------------------------------------------------------
/**
 * @brief The function for calculate the norm of vector
 */
//--------------------------------------------------------------------------
double norm_vec(const double *vec, const size_t Ndim)
{
   double ret = 0.0;
   for (size_t i=0; i<Ndim; i++) ret += pow(vec[i], 2);
   
   return sqrt(ret);
}

//--------------------------------------------------------------------------
/**
 * @brief The function for calculate AX=B using Gauss–Jordan elimination
 */
//--------------------------------------------------------------------------
void solve_mat_gauss(  const double *Amat, double *Xmat, const double *Bmat
                     , const size_t Ndim )
{
   double tmp;
   double *mat = new double[Ndim*Ndim];
   for (size_t i=0; i<Ndim*Ndim; i++)
   {
      mat [i] = Amat[i];
      Xmat[i] = Bmat[i];
   }
   for (size_t i=0; i<Ndim; i++) // A -> U
   {
      tmp = mat[idx(i,i)];
      for (size_t j=0; j<Ndim; j++)
      {
         Xmat[idx(i,j)] /= tmp;
         mat [idx(i,j)] /= tmp;
      }
      for (size_t ii=i+1; ii<Ndim; ii++)
      {
         tmp = mat[idx(ii,i)];
         for (size_t jj=0; jj<Ndim; jj++)
         {
            Xmat[idx(ii,jj)] -= Xmat[idx(i,jj)] * tmp;
            mat [idx(ii,jj)] -= mat [idx(i,jj)] * tmp;
         }
      }
   }
   for (   size_t  i=Ndim-1;  i!=0;  i--) // U -> E
      for (size_t ii=i;      ii!=0; ii--)
      {
         tmp = mat[idx(ii,i)];
         for (size_t jj=0; jj<Ndim; jj++)
         {
            Xmat[idx(ii,jj)] -= Xmat[idx(i,jj)] * tmp;
            mat[ idx(ii,jj)] -= mat[ idx(i,jj)] * tmp;
         }
      }
   
//   print_mat(mat, Ndim);   // For Debug
   
   delete [] mat;
}

void solve_mat_gauss(  const std::complex<double> *Amat, std::complex<double> *Xmat
                     , const std::complex<double> *Bmat, const size_t Ndim )
{
   std::complex<double> tmp;
   std::complex<double> *mat = new std::complex<double>[Ndim*Ndim];
   for (size_t i=0; i<Ndim*Ndim; i++)
   {
      mat [i] = Amat[i];
      Xmat[i] = Bmat[i];
   }
   for (size_t i=0; i<Ndim; i++) // A -> U
   {
      tmp = mat[idx(i,i)];
      for (size_t j=0; j<Ndim; j++)
      {
         Xmat[idx(i,j)] /= tmp;
         mat [idx(i,j)] /= tmp;
      }
      for (size_t ii=i+1; ii<Ndim; ii++)
      {
         tmp = mat[idx(ii,i)];
         for (size_t jj=0;   jj<Ndim; jj++)
         {
            Xmat[idx(ii,jj)] -= Xmat[idx(i,jj)] * tmp;
            mat [idx(ii,jj)] -= mat [idx(i,jj)] * tmp;
         }
      }
   }
   for (   size_t  i=Ndim-1;  i>0;   i--) // U -> E
      for (size_t ii=i-1;    ii>=0; ii--)
      {
         tmp = mat[idx(ii,i)];
         for (size_t jj=0; jj<Ndim; jj++)
         {
            Xmat[idx(ii,jj)] -= Xmat[idx(i,jj)] * tmp;
            mat [idx(ii,jj)] -= mat [idx(i,jj)] * tmp;
         }
      }

//   print_mat(mat, Ndim);   // For Debug

   delete [] mat;
}

//--------------------------------------------------------------------------
/**
 * @brief The function for inverse matrix using Gauss–Jordan elimination
 */
//--------------------------------------------------------------------------
void inv_mat_gauss(const double *imat, double *inv_mat, const size_t Ndim)
{
   double *mat = new double[Ndim*Ndim];
   for (size_t i=0; i<Ndim; i++) for (size_t j=0; j<Ndim; j++)
      i==j ? mat[idx(i,j)] = 1.0 : mat[idx(i,j)] = 0.0;
   
   solve_mat_gauss(imat, inv_mat, mat, Ndim);
   
   delete [] mat;
}

void inv_mat_gauss(  const std::complex<double> *imat
                   ,       std::complex<double> *inv_mat, const size_t Ndim )
{
   std::complex<double> *mat = new std::complex<double>[Ndim*Ndim];
   for (size_t i=0; i<Ndim; i++) for (size_t j=0; j<Ndim; j++)
      i==j ? mat[idx(i,j)] = 1.0 : mat[idx(i,j)] = COMP_ZERO;
   
   solve_mat_gauss(imat, inv_mat, mat, Ndim);
   
   delete [] mat;
}

//--------------------------------------------------------------------------
/**
 * @brief The function for calculate highest eigenvalue using power method
 */
//--------------------------------------------------------------------------
double eigenvalue_power(  const double *imat
                        , const size_t Ndim, const double stop_cnd )
{
   double *tmp_v0 = new double[Ndim];
   double *tmp_v1 = new double[Ndim];
   double  tmp1, tmp2, tmp_eigen0 = 999999, tmp_eigen1 = 999;
   
   tmp_v0[0] = 1.0;   // initialize v = (1,0,0,0,...,0)
   for (size_t i=1; i<Ndim; i++) tmp_v0[i] = 0.0;
   
//   int count = 0;   // For Debug
   while (fabs(tmp_eigen0-tmp_eigen1) > stop_cnd)
   {
      tmp_eigen0 = tmp_eigen1;
      
      tmp1 = 0.0;   tmp2 = 0.0;
      for (size_t i=0; i<Ndim; i++)
      {
         tmp_v1[i] = 0.0;
         for (size_t j=0; j<Ndim; j++) tmp_v1[i] += imat[idx(i,j)] * tmp_v0[j];
         tmp1 += tmp_v1[i] * tmp_v1[i];
      }
      for (size_t i=0; i<Ndim; i++)
      {
         tmp_v1[i] /= tmp1;
         tmp2      += tmp_v1[i] * tmp_v0[i];
         tmp_v0[i]  = tmp_v1[i];
      }
      tmp_eigen1 = 1.0/tmp2;
      
//      printf("%d %e %e\n", ++count, tmp_eigen1, fabs(tmp_eigen0-tmp_eigen1));
   }
//   for (int i=0; i<Ndim; i++)
//      printf("%d %lf\n", i, tmp_v1[i]);   // For Debug
   
   delete [] tmp_v0;
   delete [] tmp_v1;
   
   return tmp_eigen1;
}

//--------------------------------------------------------------------------
/**
 * @brief The matrix for Householder transformation
 */
//--------------------------------------------------------------------------
inline double Householder_matrix(  const double *v_x, const double *v_y
                                 , const double v_norm2
                                 , const size_t iii, const size_t jjj )
{
   return (  (iii==jjj ? 1.0 : 0.0)
           - 2.0 * (v_x[iii]-v_y[iii]) * (v_x[jjj]-v_y[jjj]) / v_norm2 );
}

inline double Householder_matrix(  const double *v, const double v_norm2
                                 , const size_t iii, const size_t jjj )
{
   return (iii==jjj ? 1.0 : 0.0) - 2.0 * v[iii] * v[jjj] / v_norm2;
}

//--------------------------------------------------------------------------
/**
 * @brief The matrix of Hamiltonian for Schrodinger equation
 */
//--------------------------------------------------------------------------
#define xyz(x,y,z) ((x)+SIZE*((y)+SIZE*(z)))
inline double Hamiltonian_Schrodinger(  const double *V, const double mass
                                      , const double lat_a, const int SIZE
                                      , const int xR, const int yR, const int zR
                                      , const int xC, const int yC, const int zC )
{
   if (xyz(xC,yC,zC) == xyz(xR,yR,zR))   // V is dimensionless
   {
      return 591.981159/(mass*lat_a) + V[xyz(xR,yR,zR)];
   }
   else if(xyz(xC,yC,zC) == xyz((xR+1)%SIZE,  yR, zR)      ||
           xyz(xC,yC,zC) == xyz( xR, (yR+1)%SIZE, zR)      ||
           xyz(xC,yC,zC) == xyz( xR, yR, (zR+1)%SIZE)      ||
           xyz(xC,yC,zC) == xyz((xR-1+SIZE)%SIZE, yR,  zR) ||
           xyz(xC,yC,zC) == xyz( xR, (yR-1+SIZE)%SIZE, zR) ||
           xyz(xC,yC,zC) == xyz( xR, yR, (zR-1+SIZE)%SIZE)  )
   {
      return -98.6635265/(mass*lat_a);
   }
   else
   {
      return 0.0;
   }
}

void Hamiltonian_Schrodinger(  double *mat, const double *V, const double mass
                             , const double lat_a, const int SIZE )
{
   size_t Ndim = SIZE*SIZE*SIZE;

#ifdef _OPENMP
#pragma omp parallel for
#endif
   for (      int xR=0; xR<SIZE; xR++)
      for (   int yR=0; yR<SIZE; yR++)
         for (int zR=0; zR<SIZE; zR++)
            for (      int xC=0; xC<SIZE; xC++)
               for (   int yC=0; yC<SIZE; yC++)
                  for (int zC=0; zC<SIZE; zC++)
                  {
                     size_t R = xyz(xR,yR,zR);
                     size_t C = xyz(xC,yC,zC);
                     mat[idx(R,C)] = Hamiltonian_Schrodinger( V,mass,lat_a,SIZE
                                                             ,xR,yR,zR,xC,yC,zC);
                  }
#undef xyz
}

//--------------------------------------------------------------------------
/**
 * @brief The function for transformation to tridiagonal matrix
 * @brief using Householder transformation (for symmetric matrix)
 */
//--------------------------------------------------------------------------
void to_tridiag_Householder(double *imat, const size_t Ndim)
{
   double *tmp_v   = new double[Ndim];
   double *tmp_mat = new double[Ndim*Ndim];
   double  tmp;
   
   for (size_t loop=0; loop<Ndim-2; loop++)
   {
      tmp         = 0.0;
      tmp_v[loop] = 0.0;
      for (size_t i=loop+1; i<Ndim; i++)
      {
         tmp_v[i] =     imat[idx(i,loop)];
         tmp     += pow(imat[idx(i,loop)], 2);
      }
      tmp_v[loop+1] = imat[idx(loop+1,loop)] - sqrt(tmp);
      tmp          -= imat[idx(loop+1,loop)] * sqrt(tmp);
      tmp          *= 2.0;
      
#ifdef _OPENMP
#pragma omp parallel for
#endif
      for (size_t i=loop; i<Ndim; i++) for (size_t j=loop; j<Ndim; j++)
      {
         tmp_mat[idx(i,j)] = 0.0;
         for (size_t k=loop; k<Ndim; k++) for (size_t l=loop; l<Ndim; l++)
            tmp_mat[idx(i,j)] += (  Householder_matrix(tmp_v,tmp,i,k)
                                  * imat[idx(k,l)]
                                  * Householder_matrix(tmp_v,tmp,l,j) );
      }
      for (size_t i=loop; i<Ndim; i++) for (size_t j=loop; j<Ndim; j++)
         imat[idx(i,j)] = tmp_mat[idx(i,j)];
      
//      printf("%d\n",loop);   // For Debug
      
   } // loop
   
   delete [] tmp_v;
   delete [] tmp_mat;
}

//--------------------------------------------------------------------------
/**
 * @brief The function for transformation to tridiagonal matrix
 * @brief using Lanczos algorithm
 */
//--------------------------------------------------------------------------
void to_tridiag_Lanczos( double *imat, const size_t Ndim )
{
   double *v1    = new double[Ndim];
   double *v2    = new double[Ndim];
   double *v3    = new double[Ndim];
   double *alpha = new double[Ndim];
   double *beta  = new double[Ndim];
   double *tmp_v = new double[Ndim];
   double *tmp_p;
   double  tmp   = 0.0;
   
   v1[0] = 1.0;   // initialize v1 = (1,0,0,0,...,0)
   for (size_t i=1; i<Ndim; i++) v1[i] = 0.0;
   
   alpha[0] = imat[idx(0,0)];   // initial value for alpha & beta
   for (size_t i=1; i<Ndim; i++) tmp += pow(imat[idx(i,0)], 2);
   beta[0]  = sqrt(tmp);
   
   v2[0] = 0.0;   // initialize v2 = (0, a21/beta, ..., aN1/beta)
   for (size_t i=1; i<Ndim; i++) v2[i] = imat[idx(i,0)] / beta[0];
   
   for (size_t loop=1; loop<Ndim; loop++)   // Lanczos algorithm
   {
      alpha[loop] = 0.0;
      for (size_t i=0; i<Ndim; i++)
      {
         tmp_v[i]     = 0.0;
         for (size_t j=0; j<Ndim; j++) tmp_v[i] += imat[idx(i,j)] * v2[j];
         alpha[loop] += v2[i] * tmp_v[i];
      }
      tmp = 0.0;
      for (size_t i=0; i<Ndim; i++)
         tmp += pow(tmp_v[i] - alpha[loop]*v2[i] - beta[loop-1]*v1[i], 2);
      beta[loop] = sqrt(tmp);

//      printf("%lf %lf\n",alpha[loop], beta[loop]);   // For Debug
      
      for (size_t i=0; i<Ndim; i++)
         v3[i] = (tmp_v[i]-alpha[loop]*v2[i]-beta[loop-1]*v1[i]) / beta[loop];
      
//      double gosa1 = 0.0, gosa2 = 0.0;
//      for (int k=0; k<Ndim; k++) {
//         gosa1 += v1[k]*v2[k];   gosa2 += v1[k]*v1[k];
//         gosa1 += v2[k]*v3[k];   gosa2 += v2[k]*v2[k];   // For Debug
//         gosa1 += v3[k]*v1[k];   gosa2 += v3[k]*v3[k];
//      }  printf(" loop = %d, gosa1 = %e, gosa2 = %e\n"
//                , loop, gosa1/3.0, gosa2/3.0);
      
      tmp_p = v1;   v1 = v2;   v2 = v3;   v3 = tmp_p;
      
//      printf("%d\n",loop);   // For Debug
      
   }  // loop
   
   for (size_t i=0; i<Ndim; i++) for (size_t j=0; j<Ndim; j++)
      if      (i==j)   imat[idx(i,j)] = alpha[i];
      else if (i+1==j) imat[idx(i,j)] = beta[i];
      else if (j+1==i) imat[idx(i,j)] = beta[j];
      else             imat[idx(i,j)] = 0.0;
   
   delete [] v1;
   delete [] v2;
   delete [] v3;
   delete [] alpha;
   delete [] beta;
   delete [] tmp_v;
}

//--------------------------------------------------------------------------
/**
 * @brief The function for transformation to tridiagonal matrix
 * @brief using Lanczos algorithm (For schrodinger equation)
 */
//--------------------------------------------------------------------------
#define xyz(x,y,z) ((x)+SIZE*((y)+SIZE*(z)))
void to_tridiag_Lanczos_for_schrodinger(  double *alpha, double *beta
                                        , const double *V, const int SIZE
                                        , const double mass, const double lat_a )
{
   size_t Ndim = SIZE*SIZE*SIZE;
   
   double *v1     = new double[Ndim];
   double *v2     = new double[Ndim];
   double *v3     = new double[Ndim];
   double *tmp_v  = new double[Ndim];
   double *tmp_p;
   double  tmp    = 0.0;
   
   double  offdiag_elem = -98.6635265 / (mass*lat_a);   // Dimensionless
   double  diag_elem_0  = -6.0 * offdiag_elem;

   v1[0] = 1.0;   // initialize v1 = (1,0,0,0,...,0)
   for (size_t i=1; i<Ndim; i++) v1[i] = 0.0;
   
   alpha[0] = diag_elem_0 + V[0];          // initial value for alpha & beta
   beta [0] = -sqrt(6.0) * offdiag_elem;   // require to Ndim > 2
   
   for (      int xC=0; xC<SIZE; xC++)     // initialize
      for (   int yC=0; yC<SIZE; yC++)     // v2 = (0, a21/beta, ..., aN1/beta)
         for (int zC=0; zC<SIZE; zC++)
         {
            if(xyz(xC,yC,zC)==xyz(1, 0, 0)      ||
               xyz(xC,yC,zC)==xyz(0, 1, 0)      ||
               xyz(xC,yC,zC)==xyz(0, 0, 1)      ||
               xyz(xC,yC,zC)==xyz(SIZE-1, 0, 0) ||
               xyz(xC,yC,zC)==xyz(0, SIZE-1, 0) ||
               xyz(xC,yC,zC)==xyz(0, 0, SIZE-1)  )
            {
               v2[xyz(xC,yC,zC)] = -1.0 / sqrt(6.0);
            }
            else
            {
               v2[xyz(xC,yC,zC)] = 0.0;
            }
         }
   
   for (size_t loop=1; loop<Ndim; loop++)   // Lanczos algorithm
   {
      alpha[loop] = 0.0;
      for (      int xC=0; xC<SIZE; xC++)
         for (   int yC=0; yC<SIZE; yC++)
            for (int zC=0; zC<SIZE; zC++)
            {
               tmp_v[xyz(xC,yC,zC)] =
               (diag_elem_0 + V[xyz(xC,yC,zC)]) * v2[xyz(xC,yC,zC)]
               + (  v2[xyz((xC+1)%SIZE, yC, zC)]
                  + v2[xyz(xC, (yC+1)%SIZE, zC)]
                  + v2[xyz(xC, yC, (zC+1)%SIZE)]
                  + v2[xyz((xC-1+SIZE)%SIZE, yC, zC)]
                  + v2[xyz(xC, (yC-1+SIZE)%SIZE, zC)]
                  + v2[xyz(xC, yC, (zC-1+SIZE)%SIZE)] ) * offdiag_elem;
               
               alpha[loop] += v2[xyz(xC,yC,zC)] * tmp_v[xyz(xC,yC,zC)];
            }
      
      tmp = 0.0;
      for (size_t i=0; i<Ndim; i++)
         tmp += pow(tmp_v[i] - alpha[loop]*v2[i] - beta[loop-1]*v1[i], 2);
      
      beta[loop] = sqrt(tmp);
      
      for (size_t i=0; i<Ndim; i++)
         v3[i] = (tmp_v[i]-alpha[loop]*v2[i]-beta[loop-1]*v1[i]) / beta[loop];
      
//      double gosa1 = 0.0, gosa2 = 0.0;
//      for (int k=0; k<Ndim; k++) {
//         gosa1 += v1[k]*v2[k];   gosa2 += v1[k]*v1[k];
//         gosa1 += v2[k]*v3[k];   gosa2 += v2[k]*v2[k];   // For Debug
//         gosa1 += v3[k]*v1[k];   gosa2 += v3[k]*v3[k];
//      }  printf(" loop = %d, gosa1 = %e, gosa2 = %e\n"
//                , loop, gosa1/3.0, gosa2/3.0);
      
      tmp_p = v1;   v1 = v2;   v2 = v3;   v3 = tmp_p;
      
   }  // loop

   delete [] v1;
   delete [] v2;
   delete [] v3;
   delete [] tmp_v;
}
#undef xyz
//--------------------------------------------------------------------------
/**
 * @brief The function for calculate the number of eigen value
 * @brief using sturm's theorem (It can be used for only tridiagonal matrix)
 */
//--------------------------------------------------------------------------
int Neigen_sturm(  const double *tri_diag_mat
                 , const size_t Ndim, const double test_eigen )
{
   int    Neigen = 0;
   double q1, q2 = test_eigen - tri_diag_mat[0];
   
   for (size_t i=1; i<Ndim; i++)
   {
      if (q2 < 0.0) Neigen++;
      
      q1 = q2;
      q2 = ( (test_eigen - tri_diag_mat[idx(i,i)])
            - pow(tri_diag_mat[idx(i,i-1)], 2) / q1 );
      
//      printf("%e\n",q1);   // For Debug
   }
   if (q2 < 0.0) Neigen++;
   
   return Neigen;
}

int Neigen_sturm(  const double *alpha, const double *beta
                 , const size_t Ndim, const double test_eigen )
{
   int    Neigen = 0;
   double q1, q2 = test_eigen - alpha[0];
   
   for (size_t i=1; i<Ndim; i++)
   {
      if (q2 < 0.0) Neigen++;
      
      q1 = q2;
      q2 = (test_eigen - alpha[i]) - pow(beta[i-1], 2) / q1;
      
//      printf("%e\n",q1);   // For Debug
   }
   if (q2 < 0.0) Neigen++;
   
   return Neigen;
}

//--------------------------------------------------------------------------
/**
 * @brief The function for calculate eigenvalue
 * @brief using Bi-section method (It can be used for only tridiagonal matrix)
 */
//--------------------------------------------------------------------------
void eigenvalue_bi_section(  const double *imat, double *eigenvalue
                           , const size_t Ndim, const int Nvalue
                           , const double stop_cnd )
{
   int    num_eigen;
   double aaa, bbb, ccc;
   double eigen_max = 0.0;
   
   double tmp = fabs(imat[idx(0,0)]) + fabs(imat[idx(0,1)]);
   if(tmp > eigen_max) eigen_max = tmp;   // find the maximum eigen-value
   
   for (size_t i=1; i<Ndim-1; i++)
   {
      tmp = fabs(imat[idx(i,i-1)])+fabs(imat[idx(i,i)])+fabs(imat[idx(i,i+1)]);
      if(tmp > eigen_max) eigen_max = tmp;
   }
   tmp = fabs(imat[idx(Ndim-1,Ndim-2)]) + fabs(imat[idx(Ndim-1,Ndim-1)]);
   if(tmp > eigen_max) eigen_max = tmp;
   
//   printf(" eigen max = %lf\n",eigen_max);   // For Debug
   
   for (int loop=0; loop<Nvalue; loop++)
   {
      if (loop != 0) aaa = eigenvalue[loop-1] + stop_cnd / 2.0;
      else           aaa = -eigen_max;
      
      bbb       = eigen_max;
      num_eigen = Neigen_sturm(imat,Ndim,aaa) - Neigen_sturm(imat,Ndim,bbb);
      
//      printf("%d\n",Neigen_sturm(imat, Ndim, aaa));   // For Debug
//      printf("%d\n",Neigen_sturm(imat, Ndim, 0.0));   // For Debug
      
      while ((bbb-aaa) > stop_cnd)   // Bi-section method
      {
         ccc = (aaa+bbb) / 2.0;
         num_eigen = Neigen_sturm(imat,Ndim,aaa) - Neigen_sturm(imat,Ndim,ccc);
         
//         printf(" num eigen = %d\n", num_eigen);       // For Debug
//         printf(" abc = %e %e %e\n", aaa, ccc, bbb);   // For Debug
         
         if (num_eigen != 0) bbb = ccc;
         else                aaa = ccc;
      }
      eigenvalue[loop] = (aaa+bbb) / 2.0;
      
   } // loop
}

void eigenvalue_bi_section(  const double *alpha, const double *beta
                           , double *eigenvalue, const size_t Ndim
                           , const int Nvalue , const double stop_cnd )
{
   int    num_eigen;
   double eigen_max = 0.0;
   double aaa, bbb, ccc;
   
   double tmp = fabs(alpha[0]) + fabs(beta[0]);
   if(tmp > eigen_max) eigen_max = tmp;   // find the maximum eigen-value
   
   for (size_t i=1; i<Ndim-1; i++)
   {
      tmp = fabs(beta[i-1]) + fabs(alpha[i]) + fabs(beta[i]);
      if(tmp > eigen_max) eigen_max = tmp;
   }
   tmp = fabs(alpha[Ndim-1]) + fabs(beta[Ndim-2]);
   if(tmp > eigen_max) eigen_max = tmp;
   
//      printf(" eigen max = %lf\n",eigen_max);   // For Debug
   
   for (int loop=0; loop<Nvalue; loop++)
   {
      if (loop != 0) aaa = eigenvalue[loop-1] + stop_cnd / 2.0;
      else           aaa = -eigen_max;
      
      bbb       = eigen_max;
      num_eigen = (  Neigen_sturm(alpha, beta, Ndim, aaa)
                   - Neigen_sturm(alpha, beta, Ndim, bbb) );
      
//            printf("%d\n",Neigen_sturm(imat, Ndim, aaa));   // For Debug
//            printf("%d\n",Neigen_sturm(imat, Ndim, 0.0));   // For Debug
      
      while ((bbb-aaa) > stop_cnd)   // Bi-section method
      {
         ccc = (aaa+bbb) / 2.0;
         num_eigen = (  Neigen_sturm(alpha,beta,Ndim,aaa)
                      - Neigen_sturm(alpha,beta,Ndim,ccc) );
         
//                  printf(" num eigen = %d\n", num_eigen);       // For Debug
//                  printf(" abc = %e %e %e\n", aaa, ccc, bbb);   // For Debug
         
         if (num_eigen != 0) bbb = ccc;
         else                aaa = ccc;
      }
      eigenvalue[loop] = (aaa+bbb) / 2.0;
      
   } // loop
}

#undef idx
#endif
