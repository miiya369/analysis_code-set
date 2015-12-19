//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup phase shift
 * @brief   Functions for output phase shift data
 * @author  Takaya Miyamoto
 * @since   Thu Jul 23 17:29:45 JST 2015
 */
//--------------------------------------------------------------------------

#include <observable/phase_shift.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for output phase shift data every gauge configurations
 */
//--------------------------------------------------------------------------
void PHASE_SHIFT::output_phase_shift_all( const char* out_file_name ) {
    
   func_name = "output_phase_shift_all";
   analysis::route(class_name, func_name, 1);
   
   if (phase_shift == NULL) {
      analysis::error(1,"Phase shift has not been calculated yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   ofstream ofs(out_file_name, ios::out | ios::binary);
   
   int    tmp_Nconf = analysis::Nconf;
   int    tmp_E_min = E_min;
   int    tmp_E_max = E_max;
   double tmp_mass  = mass;
   
   if (!analysis::machine_is_little_endian()) {
      analysis::endian_convert(&tmp_Nconf, 1);
      analysis::endian_convert(&tmp_E_min, 1);
      analysis::endian_convert(&tmp_E_max, 1);
      analysis::endian_convert(&tmp_mass,  1);
   }
   ofs.write((char*)&tmp_Nconf, sizeof(int));
   ofs.write((char*)&tmp_E_min, sizeof(int));   // write file header
   ofs.write((char*)&tmp_E_max, sizeof(int));
   ofs.write((char*)&tmp_mass,  sizeof(double));
   
   double tmp;
   for (int n=0; n<analysis::Nconf*(E_max-E_min+1); n++) {
      tmp = phase_shift[n];
      if (!analysis::machine_is_little_endian())
         analysis::endian_convert(&tmp, 1);
      ofs.write((char*)&tmp, sizeof(double));
   }
   ofs.close();
   
   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output mean of phase shift data with error
 */
//--------------------------------------------------------------------------
void PHASE_SHIFT::output_phase_shift_err( const char* out_file_name ) {
   
   func_name = "output_phase_shift_err";
   analysis::route(class_name, func_name, 1);
   
   if (phase_shift == NULL) {
      analysis::error(1,"Phase shift has not been calculated yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   double err, mean, sqr_mean;
   ofstream ofs(out_file_name, ios::out);
   
   for (int E=E_min; E<=E_max; E++) {
      mean     = 0.0;
      sqr_mean = 0.0;
      
      for (int i=0; i<analysis::Nconf; i++) {
         mean     +=     phase_shift[idx(i,E-E_min)];
         sqr_mean += pow(phase_shift[idx(i,E-E_min)],2);
      }
      mean     /= double(analysis::Nconf);
      sqr_mean /= double(analysis::Nconf);
      err       = sqrt(double(analysis::Nconf-1) * (sqr_mean - mean*mean));
      
      ofs << E << " " << mean << " " << err << endl;
   }
   ofs.close();
   
   analysis::route(class_name, func_name, 0);
}
