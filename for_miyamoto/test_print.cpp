
#include <stdio.h>
#include <stdarg.h>
#include <string>

int main( int argc, char **argv ){
 
   std::string test[2] = {"PS", "SS"};
   
   printf("%s %s\n", test[0].c_str(), test[1].c_str());
   
   return 0;
}