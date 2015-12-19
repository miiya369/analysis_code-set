//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup gauge
 * @brief   Header file for complex matrix class
 * @author  Takaya Miyamoto
 * @since   Sun Oct 18 04:26:59 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef COMPLEX_MATRIX_H
#define COMPLEX_MATRIX_H

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

#define MAT_DIM 3

//--------------------------------------------------------------------------
/**
 * @brief The class for complex matrix
 */
//--------------------------------------------------------------------------
class MATRIX {
    
private:
   std::complex<double> matrix[MAT_DIM*MAT_DIM];
    
protected:
    
    
public:
//================== For inner index ==================//
   inline int ij(const int i, const int j) {
      return j + MAT_DIM * i;
   }
   inline const int ij(const int i, const int j) const {
      return j + MAT_DIM * i;
   }
//================== For writing ==================//
   inline std::complex<double>& operator()(const int i, const int j) {
      return matrix[ij(i,j)];
   }
   inline std::complex<double>& operator()(const int i) {
      return matrix[i];
   }
//================== For reading ==================//
   inline const std::complex<double>& operator()(const int i, const int j) const {
      return matrix[ij(i,j)];
   }
   inline const std::complex<double>& operator()(const int i) const {
      return matrix[i];
   }
//================== Constructor ==================//
   MATRIX() {}
   MATRIX(const MATRIX &obj) {
      for (int i=0; i<MAT_DIM*MAT_DIM; i++)
         (*this)(i) = obj(i);
   }
   MATRIX(const std::complex<double> &valu) {   // construct to c_valu * Unit matrix
      for (int i=0; i<MAT_DIM; i++)
         (*this)(i,i) = valu;
   }
   ~MATRIX() {}   // Destructor
//================== For initialize ==================//
   void init() {
      for (int i=0; i<MAT_DIM*MAT_DIM; i++)
         (*this)(i) = COMP_ZERO;   // initialize to zero matrix
   }
   void init(const MATRIX &obj) {
      for (int i=0; i<MAT_DIM*MAT_DIM; i++)
         (*this)(i) = obj(i);
   }
   void init(const std::complex<double> &valu) {   // initialize to c_valu * Unit matrix
      for (int i=0; i<MAT_DIM; i++)
         (*this)(i,i) = valu;
   }
//================== Operator define ==================//
   MATRIX& operator=(const MATRIX &rhs) {
      for (int i=0; i<MAT_DIM*MAT_DIM; i++)
         (*this)(i) = rhs(i);
      return *this;
   }
   MATRIX& operator=(const std::complex<double> &rhs) {   // rhs = c_valu * Unit matrix
      for (   int i=0; i<MAT_DIM; i++)
         for (int j=0; j<MAT_DIM; j++)
               if ( i==j ) (*this)(i,j) = rhs;
               else        (*this)(i,j) = COMP_ZERO;
      return *this;
   }
   MATRIX& operator+=(const MATRIX &rhs) {
      for (int i=0; i<MAT_DIM*MAT_DIM; i++)
         (*this)(i) += rhs(i);
      return *this;
   }
   MATRIX& operator+=(const std::complex<double> &rhs) {   // rhs = c_valu * Unit matrix
      for (int i=0; i<MAT_DIM; i++)
         (*this)(i,i) += rhs;
      return *this;
   }
   MATRIX& operator-=(const MATRIX &rhs) {
      for (int i=0; i<MAT_DIM*MAT_DIM; i++)
         (*this)(i) -= rhs(i);
      return *this;
   }
   MATRIX& operator-=(const std::complex<double> &rhs) {   // rhs = c_valu * Unit matrix
      for (int i=0; i<MAT_DIM; i++)
         (*this)(i,i) -= rhs;
      return *this;
   }
   MATRIX& operator*=(const MATRIX &rhs) {
      MATRIX tmp;
      for (         int i=0; i<MAT_DIM; i++)
         for (      int j=0; j<MAT_DIM; j++)
               for (int k=0; k<MAT_DIM; k++)
                  tmp(i,j) += (*this)(i,k) * rhs(k,j);
      return *this = tmp;
   }
   MATRIX& operator*=(const std::complex<double> &rhs) {
      for (int i=0; i<MAT_DIM*MAT_DIM; i++)
         (*this)(i) *= rhs;
      return *this;
   }
   MATRIX& operator/=(const std::complex<double> &rhs) {
      for (int i=0; i<MAT_DIM*MAT_DIM; i++)
         (*this)(i) /= rhs;
      return *this;
   }
//================== Operator helper ==================//
   inline friend MATRIX operator+(MATRIX lhs, const MATRIX &rhs) {
      return lhs += rhs;
   }
   inline friend MATRIX operator+(MATRIX lhs, const std::complex<double> &rhs) {
      return lhs += rhs;
   }
   inline friend MATRIX operator+(const std::complex<double> &lhs, MATRIX rhs) {
      return rhs += lhs;
   }
   inline friend MATRIX operator-(MATRIX lhs, const MATRIX &rhs) {
      return lhs -= rhs;
   }
   inline friend MATRIX operator-(MATRIX lhs, const std::complex<double> &rhs) {
      return lhs -= rhs;
   }
   inline friend MATRIX operator-(const std::complex<double> &lhs, MATRIX rhs) {
      rhs *= (-1);
      return rhs += lhs;
   }
   inline friend MATRIX operator*(MATRIX lhs, const MATRIX &rhs) {
      return lhs *= rhs;
   }
   inline friend MATRIX operator*(MATRIX lhs, const std::complex<double> &rhs) {
      return lhs *= rhs;
   }
   inline friend MATRIX operator*(const std::complex<double> &lhs, MATRIX rhs) {
      return rhs *= lhs;
   }
   inline friend MATRIX operator/(MATRIX lhs, const std::complex<double> &rhs) {
      return lhs /= rhs;
   }
//================== Several function ==================//
   static MATRIX unit() {
      MATRIX ret;
      for (int i=0; i<MAT_DIM; i++)
         ret(i,i) = std::complex<double>(1.0,0.0);
      return ret;
   }
   MATRIX dagger() {
      MATRIX tmp;
      for (    int i=0; i<MAT_DIM; i++)
         for (int j=0; j<MAT_DIM; j++)
               tmp(j,i) = conj((*this)(i,j));
      return tmp;
   }
   std::complex<double> trace() {
      std::complex<double> tmp;
      for (int i=0; i<MAT_DIM; i++)
         tmp += (*this)(i,i);
      return tmp;
   }
   std::complex<double> sum_all_elements() {
      std::complex<double> tmp;
      for (int i=0; i<MAT_DIM*MAT_DIM; i++)
         tmp += (*this)(i);
      return tmp;
   }
   std::complex<double> sum_all_elements2() {
      std::complex<double> tmp;
      for (int i=0; i<MAT_DIM*MAT_DIM; i++)
         tmp += (*this)(i)*(*this)(i);
      return tmp;
   }
   MATRIX pow_mat(int index) {
      MATRIX ret( (*this) );
      for (int n=1; n<index; n++)
         ret *= (*this);
      return ret;
   }
   MATRIX exp_mat( int N_cutoff ) {
      MATRIX ret = 1.0 + (*this)/(std::complex<double>)N_cutoff;
      for (int loop=N_cutoff-1; loop>0; loop--)
         ret = 1.0 + (*this)*ret/(std::complex<double>)loop;
      return ret;
   }
#if MAT_DIM == 3
   std::complex<double> det() {
      return  (*this)(0,0)*(*this)(1,1)*(*this)(2,2)
            + (*this)(1,0)*(*this)(2,1)*(*this)(0,2)
            + (*this)(2,0)*(*this)(0,1)*(*this)(1,2)
            - (*this)(0,2)*(*this)(1,1)*(*this)(2,0)
            - (*this)(0,1)*(*this)(1,0)*(*this)(2,2)
            - (*this)(0,0)*(*this)(2,1)*(*this)(1,2);
   }
   MATRIX inverce() {
      MATRIX tmp;
      tmp(0,0) = (*this)(1,1)*(*this)(2,2)-(*this)(1,2)*(*this)(2,1);
      tmp(0,1) = (*this)(0,2)*(*this)(2,1)-(*this)(0,1)*(*this)(2,2);
      tmp(0,2) = (*this)(0,1)*(*this)(1,2)-(*this)(0,2)*(*this)(1,1);
      tmp(1,0) = (*this)(1,2)*(*this)(2,0)-(*this)(1,0)*(*this)(2,2);
      tmp(1,1) = (*this)(0,0)*(*this)(2,2)-(*this)(0,2)*(*this)(2,0);
      tmp(1,2) = (*this)(0,2)*(*this)(1,0)-(*this)(0,0)*(*this)(1,2);
      tmp(2,0) = (*this)(1,0)*(*this)(2,1)-(*this)(1,1)*(*this)(2,0);
      tmp(2,1) = (*this)(0,1)*(*this)(2,0)-(*this)(0,0)*(*this)(2,1);
      tmp(2,2) = (*this)(0,0)*(*this)(1,1)-(*this)(0,1)*(*this)(1,0);
      return tmp/(*this).det();
   }
   MATRIX exp_su3() {   // SU(3) = exp( i * su(3) ),   su(3) -> SU(3)
      double c0 = (*this).det().real();
      double c1 = ( ((*this)*(*this)).trace().real() ) / 2.0;
      double c0_max = 2.0 * pow(c1/3.0, 1.5);
      double theta = acos(c0/c0_max);
      double www = sqrt(c1) * sin(theta/3.0);
      double uuu = sqrt(c1/3.0) * cos(theta/3.0);
      double q1 = 2.0 * uuu;
      double q2 = -uuu + www;
      double q3 = -uuu - www;
      double qdet = q2*q3*q3+q3*q1*q1+q1*q2*q2-q2*q1*q1-q3*q2*q2-q1*q3*q3;
      std::complex<double> f0 = (  (q2*q3*q3-q3*q2*q2)*exp(COMP_IMAG*q1)
                    + (q1*q1*q3-q1*q3*q3)*exp(COMP_IMAG*q2)
                    + (q1*q2*q2-q2*q1*q1)*exp(COMP_IMAG*q3) ) / qdet;
      std::complex<double> f1 = (  (q2*q2   -q3*q3   )*exp(COMP_IMAG*q1)
                    + (q3*q3   -q1*q1   )*exp(COMP_IMAG*q2)
                    + (q1*q1   -q2*q2   )*exp(COMP_IMAG*q3) ) / qdet;
      std::complex<double> f2 = (  (q3      -q2      )*exp(COMP_IMAG*q1)
                    + (q1      -q3      )*exp(COMP_IMAG*q2)
                    + (q2      -q1      )*exp(COMP_IMAG*q3) ) / qdet;
      
      return f0 + (*this)*f1 + ((*this)*(*this))*f2;
   }
#else
#warning [COMMENTS by Miyamoto] Setting matrix dimension != 3
#warning   Cannot use member funcitons det(), inverce() and SU3().
#endif
   void print() {
      for (   int i=0; i<MAT_DIM; i++) {
         for (int j=0; j<MAT_DIM; j++)
            printf(" (%lf,%lf)"
                  , (*this)(i,j).real(), (*this)(i,j).imag());
         printf("\n");
      }
      printf("\n");
   }
};

#endif
