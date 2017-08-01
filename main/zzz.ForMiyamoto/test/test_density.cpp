//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup test
 * @brief   Test for density
 * @author  Takaya Miyamoto
 * @since   Sat Oct  8 01:24:08 JST 2016
 */
//--------------------------------------------------------------------------

#include <observable/density_type.h>

//========================================================================//
int main(int argc, char **argv) {
   
   DENSITY_GAUSSIAN    gs(60, 1.2);
   DENSITY_WOODS_SAXON ws(60, 1.2, 0.6);
   
   for (double r=0.0; r<30; r+=0.1) {
      printf("%lf %e %e\n", r, gs(r), ws(r));
   }
   
   double tmp_gs = 0.0, tmp_ws = 0.0;
   for (double r=0.0; r<30; r+=0.1) {
      tmp_gs += gs(r) * r * r * 0.1;
      tmp_ws += ws(r) * r * r * 0.1;
   }
   printf("# %lf %lf\n", tmp_gs * 4*PI, tmp_ws * 4*PI);
   printf("# %lf %lf\n", gs.rho0(), ws.rho0());
   
   return 0;
}
//========================================================================//
