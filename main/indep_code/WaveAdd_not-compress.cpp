//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup none
 * @brief   The code for averaging of 1/48 Not-compressed NBSwave data
 * @author  Takaya Miyamoto
 * @since   Tue Jun 21 21:39:44 JST 2016
 */
//--------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

static string  opath;
static string *ipath = NULL;
static double *fac   = NULL;
static int     Nadd;

static int WaveSize;
static int tmp_WaveSize;

static bool endian_flip;

inline int set_args(int, char**);

inline int input (double*, int);
inline int output(double*);

inline bool machine_is_little_endian();
inline void endian_convert(double*, size_t);

//========================================================================//
int main(int argc, char **argv)
{
   if (set_args(argc, argv) == 1) return 1;
      
   double *Wave = new double[WaveSize];
   for (int i=0; i<WaveSize; i++) Wave[i] = 0.0;
   
   for (int i=0; i<Nadd; i++) if (input(Wave, i) == 1) return 1;
   
   if (output(Wave) == 1) return 1;
   
   delete [] Wave;
   if (ipath != NULL) delete [] ipath;
   if (fac   != NULL) delete [] fac;
   return 0;
}
//========================================================================//

inline int set_args(int argc, char** argv)
{
   if (argc < 4) {
      printf("usage: %s [ofile] [factor1] [ifile1] [factor2] [ifile2] ...\n", argv[0]);
      return 1;
   }
   
   if ((argc - 2) % 2 != 0) {
      printf("Arguments error: Should be put each '[factor] [ifile]'\n");
      return 1;
   }
   
   Nadd  = (argc - 2) / 2;
   ipath = new string [Nadd];
   fac   = new double [Nadd];
   
   for (int i=0; i<Nadd; i++) {
      fac  [i] = atof(argv[2 + 0 + 2*i]);
      ipath[i] =      argv[2 + 1 + 2*i];
   }
   opath = argv[1];
   
   FILE* fp;
   if ((fp = fopen(ipath[0].c_str(), "rb")) == NULL) {
      printf("The file '%s' is not found.\n", ipath[0].c_str());
      return 1;
   }

   fseek (fp, 0, SEEK_END);
   WaveSize = ftell(fp);
   fseek (fp, 0, SEEK_SET);
   fclose(fp);

   if (WaveSize % 8 != 0) {
      printf("Invalid file size: fsize %% sizeof(double) != 0\n");
   }
   WaveSize /= 8;
   
   endian_flip = machine_is_little_endian();
   
//   printf("Nave = %d\n", Naverage); // For Debug
//   return 1;
   
   return 0;
}

inline int input(double* wave, int data_num)
{ 
   FILE* fp;
   if ((fp = fopen(ipath[data_num].c_str(), "rb")) == NULL) {
      printf("The file '%s' is not found.\n", ipath[data_num].c_str());
      return 1;
   }

   fseek  (fp, 0, SEEK_END);
   tmp_WaveSize = ftell(fp);
   fseek  (fp, 0, SEEK_SET);

   if (tmp_WaveSize != WaveSize * 8) {
      printf("ERROR: Defferent file size, %s", ipath[data_num].c_str());
      return 1;
   }
   
   double *tmp_wave = new double[WaveSize];
   fread((void*)tmp_wave, sizeof(double), WaveSize, fp);
   if (endian_flip) endian_convert(tmp_wave, WaveSize);
   
   fclose(fp);
   
   for (int i=0; i<WaveSize; i++) wave[i] += (fac[data_num] * tmp_wave[i]);
   
   delete [] tmp_wave;
   
   return 0;
}

inline int output(double* wave)
{
   FILE* fp;
   if ((fp = fopen(opath.c_str(), "wb")) == NULL) {
      printf("The file '%s' can not be open.\n", opath.c_str());
      return 1;
   }
   
   if (endian_flip) endian_convert(wave, WaveSize);

   fwrite((void*)wave, sizeof(double), WaveSize, fp);
   
   fclose(fp);
   
   return 0;
}

bool machine_is_little_endian()
{
  int endianTEST = 1;
  if (*(char*)&endianTEST) return true;
  else                     return false;
}

void endian_convert(double *DATA, size_t DATA_size)
{
  char dummy[8];

  for (size_t k=0; k<DATA_size; k++) {
    for (int j=0; j<8; j++) dummy[j] = ((char*)&DATA[k])[j];
    for (int j=0; j<8; j++) ((char*)&DATA[k])[j] = dummy[7-j];
  }
}
