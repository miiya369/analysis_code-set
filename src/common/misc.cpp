//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Miscellaneous functions of analysis namespace
 * @author  Takaya Miyamoto
 * @since   Mon Nov  7 16:14:42 JST 2016
 */
//--------------------------------------------------------------------------

#include <common/analysis.h>

string analysis::bool_to_str(bool flg) {
   if (flg) return "yes";
   else     return "no";
}

bool analysis::str_to_bool(string str) {
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
void analysis::convert_xyz(int *ix, int *iy, int *iz, int xyz) {
   int tmp = xyz;
   
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
void analysis::convert_xyzn(int *ix, int *iy, int *iz, int *conf, int xyzn) {
   int tmp = xyzn;
   
   (*ix)    = tmp % analysis::xSIZE;
   tmp      = (tmp-(*ix)) / analysis::xSIZE;
   (*iy)    = tmp % analysis::ySIZE;
   tmp      = (tmp-(*iy)) / analysis::ySIZE;
   (*iz)    = tmp % analysis::zSIZE;
   (*conf)  = (tmp-(*iz)) / analysis::zSIZE;
}

//--------------------------------------------------------------------------
/**
 * @brief Convert 0,1,...,L -> -L/2+1,...,L/2
 */
//--------------------------------------------------------------------------
void analysis::convert_origin(int ix, int iy, int iz, int *iX, int *iY, int *iZ) {
   if (ix > analysis::xSIZE/2) (*iX) = ix - analysis::xSIZE;
   else                        (*iX) = ix;
   
   if (iy > analysis::ySIZE/2) (*iY) = iy - analysis::ySIZE;
   else                        (*iY) = iy;
   
   if (iz > analysis::zSIZE/2) (*iZ) = iz - analysis::zSIZE;
   else                        (*iZ) = iz;
}

void analysis::endian_convert(cdouble *DATA, int DATA_size) {
   char dummy_R[8], dummy_I[8];
   
   for (int k=0; k<DATA_size; k++) {
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

void analysis::endian_convert(double *DATA, int DATA_size) {
   char dummy[8];
   
   for (int k=0; k<DATA_size; k++) {
      for (int j=0; j<8; j++) dummy[j] = ((char*)&DATA[k])[j];
      for (int j=0; j<8; j++) ((char*)&DATA[k])[j] = dummy[7-j];
   }
}

void analysis::endian_convert(int* DATA, int DATA_size) {
   char dummy[4];
   
   for (int k=0; k<DATA_size; k++ ) {
      for(int j=0; j<4; j++) dummy[j] = ((char*)&DATA[k])[j];
      for(int j=0; j<4; j++) ((char*)&DATA[k])[j] = dummy[3-j];
   }
}
