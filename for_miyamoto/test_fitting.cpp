
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <fstream>

using namespace std;

double func( double x, double *a ){
   
   return a[0]*exp(-pow(x/a[1],2))
         +a[2]*exp(-pow(x/a[3],2))
         +a[4]*exp(-pow(x/a[5],2));
}

int main( int argc, char **argv ){
   
   int fit_type, Nconf, Ndata;
#define idx(conf,data) conf+Nconf*data
   ifstream ifs(argv[1],ios::in|ios::binary);
   if (!ifs) return 1;
   
   ifs.read((char*)&fit_type, sizeof(int));
   ifs.read((char*)&Nconf   , sizeof(int));   // read file header
   ifs.read((char*)&Ndata   , sizeof(int));
   
   double *cood = new double[Ndata];
   double *data = new double[Ndata*Nconf];
   double *err  = new double[Ndata];
   
   for (int r=0; r<Ndata; r++) {
      ifs.read((char*)&cood[r], sizeof(double));
      ifs.read((char*)&err[r] , sizeof(double));
      for (int i=0; i<Nconf; i++)
         ifs.read((char*)&data[idx(i,r)], sizeof(double));
   }
   ifs.close();
   srand((unsigned int)time(NULL));
   
   double param[6], param_tmp[6];
   double chisq_tmp, chisq=10000;
   int    end_loop=-1, ccc=50;
   for (int loop=0; loop<10000000; loop++){
      
      for (int a=0; a<6; a++) param_tmp[a] = ((rand()/(double)RAND_MAX)-0.1)*30;
      chisq_tmp = 0;
      for (int r=0; r<Ndata; r++)
         chisq_tmp += pow(data[idx(ccc,r)]-func(cood[r],param_tmp),2)/pow(err[r],2);
      
      if (chisq_tmp<chisq){
         for (int a=0; a<6; a++) param[a] = param_tmp[a];
         chisq=chisq_tmp;
      }
      if (chisq/(Ndata-6)<1){
         end_loop = loop;
         break;
      }
   }
   printf(  "loop=%d, chisq=%lf\n"
          , end_loop, chisq/(Ndata-6));
   printf(  "%lf*exp(-(x/(%lf))**2)"
          "+(%lf)*exp(-(x/(%lf))**2)"
          "+(%lf)*exp(-(x/(%lf))**2)\n"
          , param[0], param[1], param[2], param[3], param[4], param[5]);
#undef idx
   return 0;
}
