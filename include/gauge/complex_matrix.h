//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup gauge
 * @brief   Header file for complex matrix class (arbitrary rank)
 * @author  Takaya Miyamoto
 * @since   Sun Oct 18 04:25:48 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef COMPLEX_MATRIX_ARBITRARY_RANK_H
#define COMPLEX_MATRIX_ARBITRARY_RANK_H

#include <complex>

#ifndef COMP_IMAG
#define COMP_IMAG std::complex<double>(0.0,1.0)
#endif
#ifndef COMP_ZERO
#define COMP_ZERO std::complex<double>(0.0,0.0)
#endif

//--------------------------------------------------------------------------
/**
 * @brief The class for complex matrix (arbitrary rank)
 */
//--------------------------------------------------------------------------
class COMP_MATRIX {
    
private:
   std::complex<double> *matrix = NULL;
   int rank;
    
protected:
   
public:
//================== For inner index ==================//
   inline int ij(const int i, const int j) {
      return j + rank * i;
   }
   inline const int ij(const int i, const int j) const {
      return j + rank * i;
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
   COMP_MATRIX() {}
   COMP_MATRIX(int RANK) {
      rank = RANK;
      matrix = new std::complex<double>[rank*rank];
   }
   COMP_MATRIX(const MATRIX &obj) {
      rank = obj.info_rank();
      if (matrix!=NULL) delete [] matrix;
      matrix = new std::complex<double>[rank*rank];
      for (int i=0; i<rank*rank; i++)
         (*this)(i) = obj(i);
   }
   COMP_MATRIX() { if (matrix!=NULL) delete [] matrix; }   // Destructor
//================== For initialize ==================//
   void init(int RANK) {
      rank = RANK;
      matrix = new std::complex<double>[rank*rank];
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
   int info_rank() { return rank; }
   
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
