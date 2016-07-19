//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBS wave function
 * @brief   Main part for averaging of compressed NBSwave data
 * @author  Takaya Miyamoto
 * @since   Tue Jun 21 21:39:44 JST 2016
 */
//--------------------------------------------------------------------------

#include <common/analysis.h>

#include <yukawa/PH1_4pt.h>
#include <yukawa/compress48.h>

static string *ipath = NULL;
static string opath;

static int Naverage;

using namespace yukawa;
typedef four_point_base::BC BC;
typedef PH1                 Core;
typedef four_point<Core>    Four_Point;
typedef compress48<Core>    Compress48;

static int  set_args(int, char**);

//========================================================================//
int main(int argc, char **argv)
{
   if (set_args(argc, argv) == 1) return 1;
   
   mapping48  map;     map.read(ipath[0].c_str());
   Compress48 comp;   comp.read(ipath[0].c_str(), map);
   Compress48 tmp;
   
   for (int i=1; i<Naverage; i++)
   {
      tmp.read(ipath[i].c_str(), map);
      comp += tmp; // NBSwave ave.
   } // Naverage
   
   comp /= double(Naverage);
   comp.write(opath.c_str());
   
   delete [] ipath;
   return 0;
}
//========================================================================//

static int set_args(int argc, char** argv)
{
   if (argc < 3)
   {
      printf("usage: %s [ofile] [ifile1] [ifile2] ...\n", argv[0]);
      return 1;
   }
   
   Naverage = argc - 2;
   ipath    = new string [Naverage];
   
   for (int i=0; i<Naverage; i++)
   {
      ipath[i] = argv[i+2];
   }
   opath = argv[1];
   
//   printf("Nave = %d\n", Naverage); // For Debug
//   return 1;
   
   return 0;
}
