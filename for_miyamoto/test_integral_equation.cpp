#include <stdio.h>
#include <math.h>

int main( int argc, char **argv ){
   
   double pi = 4.0*atan(1);
   double p, s;
   double del = 0.01;
   double cut = 100;
   
//   for (double x=-1.; x<=1.; x+=del) {
//      
//      p = cut * tan(pi/4.0*(x+1));
//      s = cut * pi/4.0 * (del/pow(cos(pi/4.0*(x+1)),2));
//      
//      printf("%lf %lf %lf\n", x, p, s );
//   }
   
   double q1, q2, m1 = 1234, m2 = 4321, W = m1-m2-1;
   
   q1 = sqrt(pow((W*W + m1*m1 - m2*m2)/(2*W), 2) - m1*m1);
   q2 = sqrt(pow((W*W - m1*m1 + m2*m2)/(2*W), 2) - m2*m2);
   
   printf("%lf\n%lf\n",q1, q2);
   
   return 0;
}
