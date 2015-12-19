//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup phase shift
 * @brief   Function for calculate phase shift
 * @author  Takaya Miyamoto
 * @since   Fri Sep  4 04:20:49 JST 2015
 */
//--------------------------------------------------------------------------

#include <observable/phase_shift.h>

void observable::calc_phase_shift(  PHASE_SHIFT &phase, double *param, int Nparam
                                  , int func_type, double lat_space, double mm
                                  , double ll, double del, double r_max ) {
   
   string class_name = "________________________________";
   string func_name  = "calc_phase_shift______";
   analysis::route(class_name, func_name, 1);

   double hbar_c = 197.327053;
   double hc_api = hbar_c / lat_space;
   
   for (size_t iE=0; iE<phase.info_data_size(); iE++) {
      double Coe, E = phase.E(iE);
      double phi[3], phi_diff=0, tmp1, tmp2;
      double k = sqrt(2*mm*E);

      phi[0]  = 0.0;
      phi[1]  = del;   // <- Initialize of wave function
         
      for (double r=del; r<=r_max; r+=del) {
         Coe = (  (2*mm) / pow(hbar_c,2)
                * (E - V(r/lat_space, param, Nparam, func_type)*hc_api)
                - ll*(ll+1) / pow(r,2) );
         
            phi[2]   = (2-Coe*pow(del,2))*phi[1]-phi[0];
            phi_diff = (phi[2]-phi[0])/(2.0*del);
            phi[0]   = phi[1];
            phi[1]   = phi[2];
      }
      tmp1 = atan((k*phi[0])/(hbar_c*phi_diff))-k*r_max/hbar_c+ll*PI/2.0;
      tmp2 = fmod( tmp1, PI );
      if(      tmp2 >=  PI/2.0 ) tmp2 -= PI;
      else if( tmp2 <= -PI/2.0 ) tmp2 += PI;
      phase(iE) = tmp2 * 180.0 / PI;
   }
   analysis::route(class_name, func_name, 0);
}

double observable::scatt_len( double phase, int energy, double mass ) {
   
   return -tan(phase * PI/180.0) * 197.327053 / sqrt(2.0 * mass * energy);
}
