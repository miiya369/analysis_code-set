//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup none
 * @brief   The code for averaging of 1/48 compressed NBSwave data
 * @author  Takaya Miyamoto
 * @since   Tue Jun 21 21:39:44 JST 2016
 */
//--------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

static string *ipath = NULL;
static string  opath;
static int     Naverage;

static int WaveSize;
static int tmp_WaveSize;

inline int set_args(int, char**);

inline int input (double*, int);
inline int output(const double*);

//========================================================================//
int main(int argc, char **argv)
{
   if (set_args(argc, argv) == 1) return 1;
      
   double *Wave = new double[WaveSize];
   for (int i=0; i<WaveSize; i++) Wave[i] = 0.0;
   
   for (int i=0; i<Naverage; i++) if (input(Wave, i) == 1) return 1;
   
   if (output(Wave) == 1) return 1;
   
   delete [] Wave;
   if (ipath  != NULL) delete [] ipath;
   return 0;
}
//========================================================================//

inline int set_args(int argc, char** argv)
{
   if (argc < 3) {
      printf("usage: %s [ofile] [ifile1] [ifile2] ...\n", argv[0]);
      return 1;
   }
   
   Naverage = argc - 2;
   ipath    = new string [Naverage];
   
   for (int i=0; i<Naverage; i++) {
      ipath[i] = argv[i+2];
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
   
   fclose(fp);
   
   for (int i=0; i<WaveSize; i++) wave[i] += tmp_wave[i] / double(Naverage);
   
   delete [] tmp_wave;
   
   return 0;
}

inline int output(const double* wave)
{
   FILE* fp;
   if ((fp = fopen(opath.c_str(), "wb")) == NULL) {
      printf("The file '%s' can not be open.\n", opath.c_str());
      return 1;
   }
   
   fwrite((void*)wave, sizeof(double), WaveSize, fp);
   
   fclose(fp);
   
   return 0;
}
