//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBS wave function
 * @brief   Main part for difference chack for NBSwave of results of hf-code
 * @author  Takaya Miyamoto
 * @since   Thu Jun  2 00:10:23 JST 2016
 */
//--------------------------------------------------------------------------

#include <NBSwave/NBSwave.h>

static string     path1, path2;
static SPIN_TYPE  spin;

static int  set_args(int, char**);
//========================================================================//
int main(int argc, char** argv)
{
   if (set_args(argc, argv) == 1) return 1;
   
   NBS_WAVE_ORG Wave_org1(path1.c_str());
   NBS_WAVE_ORG Wave_org2(path2.c_str());
   NBS_WAVE Wave1, Wave2;
   
   NBSwave::spin_projection(Wave_org1, Wave1, spin);
   NBSwave::spin_projection(Wave_org2, Wave2, spin);
   
   double comp_eps = 1e-30;
   
   size_t count_z       = 0;
   size_t count_CompEps = 0;
   size_t count_m[1000];
   size_t count_p[1000];
   for (int i=0; i<1000; i++)
   {
      count_m[i] = 0;
      count_p[i] = 0;
   }
   double  tmp_f;
   size_t n_size    = Wave1.data_size();
   size_t tot_count = 0;
   
   for (size_t i=0; i<n_size; i++)
   {
      if      (abs(Wave1(i).real() + Wave2(i).real()) == 0.0     )
         count_z++;
      else if (abs(Wave1(i).real() + Wave2(i).real()) <  comp_eps)
         count_CompEps++;
      else
      {
         tmp_f = 2.0 * abs(  (Wave1(i).real() - Wave2(i).real())
                           / (Wave1(i).real() + Wave2(i).real()) );
         
         if      (tmp_f == 0.0    ) count_z++;
         else if (log10(tmp_f) > 0) count_p[ int(log10(tmp_f))]++;
         else                       count_m[-int(log10(tmp_f))]++;
      }
      tot_count++;
      
      if      (abs(Wave1(i).imag() + Wave2(i).imag()) == 0.0     )
         count_z++;
      else if (abs(Wave1(i).imag() + Wave2(i).imag()) <  comp_eps)
         count_CompEps++;
      else
      {
         tmp_f = 2.0 * abs(  (Wave1(i).imag() - Wave2(i).imag())
                           / (Wave1(i).imag() + Wave2(i).imag()) );
         
         if      (tmp_f == 0.0    ) count_z++;
         else if (log10(tmp_f) > 0) count_p[ int(log10(tmp_f))]++;
         else                       count_m[-int(log10(tmp_f))]++;
      }
      tot_count++;
   }
   
   size_t count_sum = count_z + count_CompEps;
   printf("\n======================== CHACK RESULTS ========================\n");
   printf("   #.ZERO DIFFERENCE\t=\t%lu\n\n", count_z);
   printf("   #.CompEps DIFFERENCE\t=\t%lu\n\n", count_CompEps);
   for (int i=999; i>=0; i--)
      if (count_p[i] != 0)
      {
         printf("   #.1e%d DIFFERENCE\t=\t%lu\n", i, count_p[i]);
         count_sum += count_p[i];
      }
   for (int i=0; i<1000; i++)
      if (count_m[i] != 0)
      {
         printf("   #.1e%d DIFFERENCE\t=\t%lu\n", -i, count_m[i]);
         count_sum += count_m[i];
      }
   printf("\n   TOTAL SUM\t\t=\t%lu", count_sum);
   printf("\n   TOTAL CHACK\t\t=\t%lu\n\n", tot_count);
   printf("===============================================================\n\n");
   
   return 0;
}
//========================================================================//

static int set_args(int argc, char** argv)
{
   if (argc != 5)
   {
      printf("usage: %s [path1] [path2] [spin] [space size]\n", argv[0]);
      return 1;
   }
   path1 = argv[1];
   path2 = argv[2];
   
   spin.set(argv[3]);
   
   analysis::xSIZE = atoi(argv[4]);
   analysis::ySIZE = atoi(argv[4]);
   analysis::zSIZE = atoi(argv[4]);
   
   return 0;
}
