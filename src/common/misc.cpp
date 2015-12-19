//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Miscellaneous functions of analysis namespace
 * @author  Takaya Miyamoto
 * @since   Fri Dec 18 02:23:57 JST 2015
 */
//--------------------------------------------------------------------------

#include <common/analysis.h>

bool analysis::do_debug = false;

void analysis::route( string class_name, string func_name, int io ) {
   
   if(!do_debug) return;   // for no debug.
   
   time_t timer;
   time(&timer);
   if      (io == 1) printf(" @@@ Enter into  %s.%s || %s"
                            , class_name.c_str()
                            , func_name.c_str(), ctime(&timer));
   else if (io == 0) printf(" @@@ Go out from %s.%s || %s"
                            , class_name.c_str()
                            , func_name.c_str(), ctime(&timer));
   fflush(stdout);
}

void analysis::error( int error_type, const char* error_comments ) {
   
   if (error_type == 0) exit(1);
   
   if (error_type == 1) printf("\n\n @@@@@@ WARNING : %s\n\n", error_comments);
   
   if (error_type == 2) {
      printf("\n\n @@@@@@ FILE OPEN ERROR : The file \"%s\" is not exist.\n\n",
             error_comments);
      exit(1);
   }
   if (error_type == 3) {
      printf("\n\n @@@@@@ ERROR : %s\n\n", error_comments);
      exit(1);
   }
}

string analysis::bool_to_str( bool flg ){
   
   if (flg) return "yes";
   else     return "no";
}

bool analysis::str_to_bool( string str ){
   
   if (str == "yes") return true;
   else              return false;
}

bool analysis::machine_is_little_endian() {
   
   int endianTEST = 1;
   if (*(char*)&endianTEST) return true;
   else                     return false;
}

//--------------------------------------------------------------------------
/**
 * @brief Convert xyz = x+xSIZE*(y+ySIZE*z) -> x, y, z
 */
//--------------------------------------------------------------------------
void analysis::convert_xyz(int *ix, int *iy, int *iz, size_t xyz){
   
   size_t tmp = xyz;
   (*ix) = tmp % analysis::xSIZE;
   tmp   = (tmp-(*ix)) / analysis::xSIZE;
   (*iy) = tmp % analysis::ySIZE;
   (*iz) = (tmp-(*iy)) / analysis::ySIZE;
}

//--------------------------------------------------------------------------
/**
 * @brief Convert xyzn = x+xSIZE*(y+ySIZE*(z*zSIZE*conf) -> x, y, z, conf
 */
//--------------------------------------------------------------------------
void analysis::convert_xyzn(int *ix, int *iy, int *iz, int *conf, size_t xyzn){
   
   size_t tmp = xyzn;
   (*ix)    = tmp % analysis::xSIZE;
   tmp      = (tmp-(*ix)) / analysis::xSIZE;
   (*iy)    = tmp % analysis::ySIZE;
   tmp      = (tmp-(*iy)) / analysis::ySIZE;
   (*iz)    = tmp % analysis::zSIZE;
   (*conf)  = (tmp-(*iz)) / analysis::zSIZE;
}

void analysis::endian_convert( cdouble *DATA, size_t DATA_size ) {
   
   char dummy_R[8], dummy_I[8];
   
   for (size_t k=0; k<DATA_size; k++) {
      for (int j=0; j<8; j++) {
         dummy_R[j] = ((char*)&DATA[k])[j];
         dummy_I[j] = ((char*)&DATA[k])[j+8];
      }
      for (int j=0; j<8; j++) {
         ((char*)&DATA[k])[j]   = dummy_R[7-j];
         ((char*)&DATA[k])[j+8] = dummy_I[7-j];
      }
   }
}

void analysis::endian_convert( double *DATA, size_t DATA_size ) {
   
   char dummy[8];
   
   for (size_t k=0; k<DATA_size; k++) {
      for (int j=0; j<8; j++) dummy[j] = ((char*)&DATA[k])[j];
      for (int j=0; j<8; j++) ((char*)&DATA[k])[j] = dummy[7-j];
   }
}

void analysis::endian_convert( int* DATA, size_t DATA_size ) {
   
   char dummy[4];
   
   for (size_t k=0; k<DATA_size; k++ ) {
      for(int j=0; j<4; j++) dummy[j] = ((char*)&DATA[k])[j];
      for(int j=0; j<4; j++) ((char*)&DATA[k])[j] = dummy[3-j];
   }
}
