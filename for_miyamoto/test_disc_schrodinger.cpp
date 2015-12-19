#include <stdio.h>
#include <complex>

using namespace std;

int main( int argc, char **argv ){
    
   double E   = 10.0;
   double V   = 0.0;
   double mu  = 100.0;
   double eps = 0.0001;
   
   double a = 1 - mu * pow(eps, 2) * (E - V);
   
   complex<double> xp = a + sqrt(complex<double>(pow(a, 2) - 1));
   complex<double> xm = a - sqrt(complex<double>(pow(a, 2) - 1));
   
   complex<double> psi_1 = complex<double>(eps, 0.0);
   
   complex<double> Coe, psi;
   for (int n=0; n<3000; n++) {
      Coe = (pow(xp, n) - pow(xm, n))/(xp - xm);
      psi = Coe * psi_1;
      printf("%d %lf %lf\n", n, psi.real(), psi.imag());
   }
}
