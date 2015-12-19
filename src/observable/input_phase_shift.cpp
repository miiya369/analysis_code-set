//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup scattering length
 * @brief   Function for input phase shift data
 * @author  Takaya Miyamoto
 * @since   Thu Jul 23 18:01:17 JST 2015
 */
//--------------------------------------------------------------------------

#include <observable/scattering_length.h>

void SCATTERING_LENGTH::input_phase_shift( const char* in_file_name ) {
   
   func_name = "input_phase_shift_file";
   analysis::route(class_name, func_name, 1);
   
   ifstream ifs(in_file_name, ios::in | ios::binary);
   if(!ifs) analysis::error(2, in_file_name);
   
   int    tmp_Nconf, tmp_E_min, tmp_E_max;
   double tmp_mass;
   
   ifs.read((char*)&tmp_Nconf, sizeof(int));
   ifs.read((char*)&tmp_E_min, sizeof(int));   // read file header
   ifs.read((char*)&tmp_E_max, sizeof(int));
   ifs.read((char*)&tmp_mass,  sizeof(double));
   
   if (!analysis::machine_is_little_endian()) {
      analysis::endian_convert(&tmp_Nconf, 1);
      analysis::endian_convert(&tmp_E_min, 1);
      analysis::endian_convert(&tmp_E_max, 1);
      analysis::endian_convert(&tmp_mass,  1);
   }
   
   analysis::Nconf = tmp_Nconf;
   E_min           = tmp_E_min;
   E_max           = tmp_E_max;
   mass            = tmp_mass;
   
   if (phase_shift != NULL) delete [] phase_shift;
   phase_shift = new double[analysis::Nconf * (E_max-E_min+1)];
   
   double tmp;
   for (size_t n=0; n<analysis::Nconf*(E_max-E_min+1); n++) {
      ifs.read((char*)&tmp, sizeof(double));
      if (!analysis::machine_is_little_endian())
         analysis::endian_convert(&tmp, 1);
      phase_shift[n] = tmp;
   }
   ifs.close();
   
   analysis::route(class_name, func_name, 0);
}

void SCATTERING_LENGTH::input_phase_shift(  double *phase_SHIFT,  double MASS
                                          , int E_MIN, int E_MAX ) {
   
   func_name = "input_phase_shift_____";
   analysis::route(class_name, func_name, 1);
   
   mass            = MASS;
   E_min           = E_MIN;
   E_max           = E_MAX;
   
   if (phase_shift != NULL) delete [] phase_shift;
   phase_shift = new double[analysis::Nconf * (E_max-E_min+1)];
   
   for (size_t n=0; n<analysis::Nconf*(E_max-E_min+1); n++)
      phase_shift[n] = phase_SHIFT[n];
   
   analysis::route(class_name, func_name, 0);
}
