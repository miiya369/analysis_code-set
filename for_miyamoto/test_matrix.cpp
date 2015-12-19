#include <stdio.h>
#include <stdlib.h>
#include <complex>
#include <time.h>
#include <fstream>

//#include <omp.h>

#include <gauge/matrix_class.h>
#include <gauge/matrix_func.h>
#include <gauge/gauge_link.h>

using namespace std;

typedef complex<double> cdouble;

int main( int argc, char **argv ){
   
   double val[1];
   
//   int size = 4;
////   double test_Amat[9] = {2,3,3,1,4,1,2,1,4};
//   double test_Amat[16] = {3,1,1,2,5,1,3,4,2,0,1,0,1,3,2,1};
////   double test_Bmat[9] = {1,0,0,0,1,0,0,0,1};
////   double test_Bmat[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
////   double test_Bmat[9] = {24,20,25,15,19,15,24,14,26};
//   double test_Bmat[16] = {43,50,57,64,89,102,115,128,11,14,17,20,47,54,61,68};
//   double test_Xmat[16];
//   solve_mat_gauss(test_Amat, test_Xmat, test_Bmat, size);
////   inv_mat_gauss(test_Amat, test_Xmat, size);
//   for (int i=0; i<size; i++) {
//      for (int j=0; j<size; j++) printf("%1.2lf ", test_Amat[j+size*i]);
//      printf("\n");
//   }  printf("\n");
//   for (int i=0; i<size; i++) {
//      for (int j=0; j<size; j++) printf("%1.2lf ", test_Xmat[j+size*i]);
//      printf("\n");
//   }  printf("\n");
//   for (int i=0; i<size; i++) {
//      for (int j=0; j<size; j++) printf("%1.2lf ", test_Bmat[j+size*i]);
//      printf("\n");
//   }  printf("\n");
//   return 0;
   
   
//   int size = 100;
//   double mass = 1000*pow(0.0907/197,2);
//   double *mat     = new double[size*size];
//   double *mat_inv = new double[size*size];
//   
//   for (int i=0; i<size; i++) for (int j=0; j<size; j++)
//      mat[j+size*i] = (i==j           ?  1.0/mass :
//                       i==j+1||i==j-1 ? -0.5/mass : 0.0);
//   
//   inv_mat_gauss(mat, mat_inv, size);
//   eigenvalue_power(mat_inv, val, size, 1, 10e-12);
//   
//   return 0;
   
   
   
//   int size = 80;
//   for (double aa=0.1; aa>0.01; aa-=0.01) {
//      double *mat     = new double[size*size];
//      double *mat_inv = new double[size*size];
//      double mass = 1000*pow(aa/197,2);
//      
//      for (int i=0; i<size; i++) for (int j=0; j<size; j++)
//         mat[j+size*i] = (i==j           ?  1.0/mass :
//                          i==j+1||i==j-1 ? -0.5/mass : 0.0);
//      
////      inv_mat_gauss(mat, mat_inv, size);
//      eigenvalue_power(mat, val, size, 1, 10e-12);
//      
//      printf("%lf %e\n", aa, val[0]);
//      delete [] mat;
//      delete [] mat_inv;
//   }
   
   
   
//   double mass = 1000*pow(0.0907/197,2);
////   printf("%lf\n",1.0/mass); return 0;
//   for (int size=10; size<100; size+=10) {
//      double *mat     = new double[size*size];
//      double *mat_inv = new double[size*size];
//      
//      for (int i=0; i<size; i++) for (int j=0; j<size; j++) {
////         mat[j+size*i] = (i==j                              ?  1.0/mass :
////                          i==(j+1)%size||i==(j-1+size)%size ? -0.5/mass : 0.0);
//         mat[j+size*i] = (i==j           ?  1.0/mass :
//                          i==j+1||i==j-1 ? -0.5/mass : 0.0);
//      }
////      inv_mat_gauss(mat, mat_inv, size);
//      eigenvalue_power(mat, val, size, 1, 10e-12);
//      
//      printf("%d %e\n", size, val[0]);
//      delete [] mat;
//      delete [] mat_inv;
//   }

   
   
   
//#define idx(xx,yy,zz) ((xx)+size*((yy)+size*(zz)))
//#define idx_mat(x1,y1,z1,x2,y2,z2) (idx(x2,y2,z2)+SIZE*idx(x1,y1,z1))
//   double mass = 1000*pow(0.0907/197,2);
//   for (int size=3; size<9; size++) {
//   int SIZE = size*size*size;
//   double *mat     = new double[SIZE*SIZE];
//   double *mat_inv = new double[SIZE*SIZE];
//   for (      int iz=0; iz<size; iz++)
//      for (   int iy=0; iy<size; iy++)
//         for (int ix=0; ix<size; ix++)
//            for (      int jz=0; jz<size; jz++)
//               for (   int jy=0; jy<size; jy++)
//                  for (int jx=0; jx<size; jx++) {
//                     mat[idx_mat(ix,iy,iz,jx,jy,jz)]
//                     = idx(jx,jy,jz)==idx(ix,iy,iz) ? 3.0/mass :
//                     idx(jx,jy,jz)==idx((ix+1)%size,iy,iz) ||
//                     idx(jx,jy,jz)==idx(ix,(iy+1)%size,iz) ||
//                     idx(jx,jy,jz)==idx(ix,iy,(iz+1)%size) ||
//                     idx(jx,jy,jz)==idx((ix-1+size)%size,iy,iz) ||
//                     idx(jx,jy,jz)==idx(ix,(iy-1+size)%size,iz) ||
//                     idx(jx,jy,jz)==idx(ix,iy,(iz-1+size)%size) ? -0.5/mass
//                     : 0.0;
//                  }
////   for (int i=0; i<SIZE; i++) {
////      for (int j=0; j<SIZE; j++) printf(" %lf",mat[j+SIZE*i]);
////      printf("\n");
////   }
////   for (int i=0; i<SIZE; i++) {
////      int count=0;
////      for (int j=0; j<SIZE; j++) if (mat[i+SIZE*j]==8) count++;
////      printf("%d\n",count);
////   } return 0;
//   inv_mat_gauss(mat, mat_inv, SIZE);
//   eigenvalue_power(mat, val, SIZE, 1, 10e-6);
//   delete [] mat;
//   delete [] mat_inv;
//   printf("%d %lf\n", size, 1.0/val[0]);
//   } return 0;
//#undef idx
//#undef idx_mat
   

   
   
   
   
//   size_t mat_size = 16*16*16*32*3*3*2;
//   double *fgmat   = new double[mat_size];
//   double *fconf   = new double[mat_size*4];
//   
//   ifstream ifs1(argv[1],ios::in|ios::binary);
//   ifstream ifs2(argv[2],ios::in|ios::binary);
//   ifs1.read((char*)fgmat,sizeof(double)*mat_size);
//   ifs2.read((char*)fconf,sizeof(double)*mat_size*4);
//   ifs1.close();
//   ifs2.close();
//   
//   MATRIX *gmat = (MATRIX*)fgmat;
//   MATRIX *conf = (MATRIX*)fconf;
//   MATRIX tmp1, tmp2;
//   
//   printf("%e\n", plaquette(fconf, 16, 32));
//   gfix_by_gmat(fconf, fgmat, 16, 32);
//   printf("%e\n", plaquette(fconf, 16, 32));
   
//#define idx(mu,x,y,z,t) (mu+4*(x+16*(y+16*(z+16*t))))
//   
//   tmp1 = ((  gmat[idx(0,1,2,3,4)/4]
//            * conf[idx(0,1,2,3,4)]
//            * gmat[idx(0,2,2,3,4)/4].dagger()
//            )
//           *
//           (  gmat[idx(0,2,2,3,4)/4]
//            * conf[idx(1,2,2,3,4)]
//            * gmat[idx(0,2,3,3,4)/4].dagger()
//            )
//           *
//           (  gmat[idx(0,1,3,3,4)/4]
//            * conf[idx(0,1,3,3,4)]
//            * gmat[idx(0,2,3,3,4)/4].dagger()
//            ).dagger()
//           *
//           (  gmat[idx(0,1,2,3,4)/4]
//            * conf[idx(1,1,2,3,4)]
//            * gmat[idx(0,1,3,3,4)/4].dagger()
//            ).dagger()
//           );
//   
//   tmp2 = (  conf[idx(0,1,2,3,4)]
//           * conf[idx(1,2,2,3,4)]
//           * conf[idx(0,1,3,3,4)].dagger()
//           * conf[idx(1,1,2,3,4)].dagger()
//           );
//   
//   tmp1.print();
//   tmp2.print();
//   (tmp1-tmp2).print();
//   
//   printf("%e %e %e\n",tmp1.trace().real(),tmp2.trace().real(),(tmp1-tmp2).trace().real());
//   printf("%e %e %e\n",tmp1.trace().imag(),tmp2.trace().imag(),(tmp1-tmp2).trace().imag());
//   
//#undef idx
   
//   std::complex<double> tmp;
//   for (size_t n=0; n<16*16*16*32; n++) {
//      tmp1 += gmat[n] * gmat[n].dagger();
//      tmp  += gmat[n].det();
//   }
//   tmp1.print();
//   printf("%lf %lf\n", tmp.real(), tmp.imag());
//   
//   tmp1.init();
//   tmp = std::complex<double>(0.0,0.0);
//   for (size_t n=0; n<16*16*16*32*4; n++) {
//      tmp1 += conf[n] * conf[n].dagger();
//      tmp  += conf[n].det();
//   }
//   tmp1.print();
//   printf("%lf %lf\n", tmp.real(), tmp.imag());
   
   return 0;
}
