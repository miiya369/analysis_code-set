//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup phase shift
 * @brief   Function for calculate phase shift
 * @author  Takaya Miyamoto
 * @since   Thu Jul 23 17:47:41 JST 2015
 */
//--------------------------------------------------------------------------

#include <observable/phase_shift.h>

void PHASE_SHIFT::calc_phase_shift(  double LATTICE_SPACE, double MASS
                                   , double ang_mom, double DEL, double R_max
                                   , int E_MIN, int E_MAX ) {
   
   func_name = "calc_phase_shift______";
   analysis::route(class_name, func_name, 1);
   
   if (param == NULL) {
      analysis::error(1,"Potential parameter has not been input yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   lattice_space = LATTICE_SPACE;
   mass          = MASS;
   ll            = ang_mom;
   del           = DEL;
   r_max         = R_max;
   E_max         = E_MAX;
   E_min         = E_MIN;
   
   if (phase_shift != NULL) delete [] phase_shift;
   phase_shift = new double[analysis::Nconf * (E_max-E_min+1)];
   
   double max_r_iter = r_max / del;
   int    counter = 0;
   
   printf(" @ phase shift calculating |   0%%");
#ifdef _OPENMP
#pragma omp parallel for
#endif
   for (int E=E_min; E<=E_max; E++) {
      double Coe, r;
      double phi[3], phi_diff=0, tmp1, tmp2;
      double k = sqrt(2*mass*E);
      
      for(int conf=0; conf<analysis::Nconf; conf++) {
         phi[0]  = 0.0;
         phi[1]  = del;   // <- Initialize of wave function
         r       = del;
         
         for (int iter=0; iter<max_r_iter; iter++) {
            r   += del;
            Coe = (2*mass)/pow(hbar_c,2)*(E-V(r,conf))-ll*(ll+1)/pow(r,2);
            phi[2]   = (2-Coe*pow(del,2))*phi[1]-phi[0];
            phi_diff = (phi[2]-phi[0])/(2.0*del);
            phi[0]   = phi[1];
            phi[1]   = phi[2];
            
//            tmp1 = atan((k*phi[0])/(hbar_c*phi_diff)) //
//            -k*r/hbar_c+ll*PI/2.0;        //
//            tmp2 = fmod( tmp1, PI );                  //
//            if(      tmp2 >=  PI/2.0 ) tmp2 -= PI;    //   For Denug !
//            else if( tmp2 <= -PI/2.0 ) tmp2 += PI;    //
//            printf("%lf %lf\n",r, tmp2* 180.0 / PI);  //
         }
         tmp1 = atan((k*phi[0])/(hbar_c*phi_diff))-k*r/hbar_c+ll*PI/2.0;
         tmp2 = fmod( tmp1, PI );
         if(      tmp2 >=  PI/2.0 ) tmp2 -= PI;
         else if( tmp2 <= -PI/2.0 ) tmp2 += PI;
         phase_shift[idx(conf,E-E_min)] = tmp2 * 180.0 / PI;
      }
      counter++;
      printf("\b\b\b\b%3.0lf%%",double(counter)/double(E_max-E_min)*100);
      fflush(stdout);
   }
   printf("\n");
   
   analysis::route(class_name, func_name, 0);
}

void PHASE_SHIFT::calc_phase_shift_test(  double r_0, double V_0
                                        , double MASS, double DEL, double R_max
                                        , int E_MIN, int E_MAX ) {
   
   func_name = "calc_phase_shift_test_";
   analysis::route(class_name, func_name, 1);
   
   analysis::Nconf = 1;
   func_type.set("test");
   lattice_space   = 1;
   ll              = 0;
   
   test_r_0 = r_0;
   test_V_0 = V_0;
   mass     = MASS;
   del      = DEL;
   r_max    = R_max;
   E_max    = E_MAX;
   E_min    = E_MIN;
   
   if (phase_shift != NULL) delete [] phase_shift;
   phase_shift = new double[analysis::Nconf * (E_max-E_min+1)];
   
   double max_r_iter = r_max / del;
   int    counter = 0;
   
   printf(" @ phase shift calculating |   0%%");
#ifdef _OPENMP
#pragma omp parallel for
#endif
   for (int E=E_min; E<=E_max; E++) {
      double Coe, r;
      double phi[3], phi_diff=0, tmp1, tmp2;
      double k = sqrt(2*mass*E);
      
      for(int conf=0; conf<1; conf++) {
         phi[0]  = 0.0;
         phi[1]  = del;   // <- Initialize of wave function
         r       = del;
         
         for (int iter=0; iter<max_r_iter; iter++) {
            r   += del;
            Coe = (2*mass)/pow(hbar_c,2)*(E-V(r,conf))-ll*(ll+1)/pow(r,2);
            phi[2]   = (2-Coe*pow(del,2))*phi[1]-phi[0];
            phi_diff = (phi[2]-phi[0])/(2.0*del);
            phi[0]   = phi[1];
            phi[1]   = phi[2];
            
            //            tmp1 = atan((k*phi[0])/(hbar_c*phi_diff)) //
            //            -k*r/hbar_c+ll*PI/2.0;        //
            //            tmp2 = fmod( tmp1, PI );                  //
            //            if(      tmp2 >=  PI/2.0 ) tmp2 -= PI;    //   For Denug !
            //            else if( tmp2 <= -PI/2.0 ) tmp2 += PI;    //
            //            printf("%lf %lf\n",r, tmp2* 180.0 / PI);  //
         }
         tmp1 = atan((k*phi[0])/(hbar_c*phi_diff))-k*r/hbar_c+ll*PI/2.0;
         tmp2 = fmod( tmp1, PI );
         if(      tmp2 >=  PI/2.0 ) tmp2 -= PI;
         else if( tmp2 <= -PI/2.0 ) tmp2 += PI;
         phase_shift[idx(conf,E-E_min)] = tmp2 * 180.0 / PI;
      }
      counter++;
      printf("\b\b\b\b%3.0lf%%",double(counter)/double(E_max-E_min)*100);
      fflush(stdout);
   }
   printf("\n");
   
   analysis::route(class_name, func_name, 0);
}
