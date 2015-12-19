//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup test
 * @brief   Main part for test or debug
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <common/analysis.h>
#include <potential/potential.h>
#include <common/matrix.h>

int analysis::Nconf = 10;
int analysis::xSIZE = 16;
int analysis::ySIZE = 16;
int analysis::zSIZE = 16;
int analysis::tSIZE = 32;

size_t NBSwave::xyzSIZE;
size_t NBSwave::xyznSIZE;

char analysis::data_list[MAX_N_DATA][MAX_LEN_PATH];

int main(int argc, char **argv) {

   char tes[320];
   
   printf(" %d \n",sizeof(CORRELATOR));
   printf(" %d \n",sizeof(NBS_WAVE));
   printf(" %d \n",sizeof(R_CORRELATOR));
   printf(" %d \n",sizeof(POTENTIAL));
   printf(" %d \n",sizeof(MATRIX));
   printf(" %d \n",sizeof(CHANNEL_TYPE));
   printf(" %d \n",sizeof(HADRON_TYPE));
   printf(" %d \n",sizeof(string));
   printf(" %d \n",sizeof(tes));
   
   return 0;
}
