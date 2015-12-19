
#include <stdio.h>
#include <fstream>
using namespace std;

#define SIZE 32

int main( int argc, char **argv ){
   
   int count = 0;
   int red_count = 0;
//   ofstream ofs("test.txt",ios::out);
   
   for(      int z=0;z<SIZE;z++)
      for(   int y=0;y<SIZE;y++)
         for(int x=0;x<SIZE;x++) {
            count++;
            if (x>SIZE/2 || y>x || z>y) continue;
            
//            if (x<=SIZE/2 && y<=x && z<=y ){ red_count++;
//               ofs << x << " " << y << " " << z << endl;
//            }
            red_count++;
         }
   printf("count = %d\nred_count = %d\n",count, red_count);
//   ofs.close();
   
   return 0;
}
