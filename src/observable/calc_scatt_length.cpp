//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup scattering length
 * @brief   Function for calculate scattering length
 * @author  Takaya Miyamoto
 * @since   Thu Jul 23 18:01:17 JST 2015
 */
//--------------------------------------------------------------------------

#include <observable/scattering_length.h>

void SCATTERING_LENGTH::calc_scatt_len() {
   
   func_name = "calc_scatt_len________";
   analysis::route(class_name, func_name, 1);
   
   if (phase_shift == NULL) {
      analysis::error(1,"Phase shift has not been input yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   if (scatt_length != NULL) delete [] scatt_length;
   scatt_length = new double[analysis::Nconf * (E_max-E_min+1)];
   
   for (int iE=E_min; iE<=E_max; iE++)
      for (int i=0; i<analysis::Nconf; i++)
         scatt_length[idx(i,iE-E_min)]
            = -tan(phase_shift[idx(i,iE-E_min)] * PI/180.0)
             * hbar_c / sqrt(2.0 * mass * iE);

   analysis::route(class_name, func_name, 0);
}
