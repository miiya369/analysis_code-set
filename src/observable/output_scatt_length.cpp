//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup scattering length
 * @brief   Functions for output scattering length data
 * @author  Takaya Miyamoto
 * @since   Thu Jul 23 17:30:34 JST 2015
 */
//--------------------------------------------------------------------------

#include <observable/scattering_length.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for output scattering length data every gauge configurations
 */
//--------------------------------------------------------------------------
void SCATTERING_LENGTH::output_scatt_len_all( const char* out_file_name ) {
   
   func_name = "output_scatt_len_all__";
   analysis::route(class_name, func_name, 1);
   
   if (scatt_length == NULL) {
      analysis::error(1,"scattering length has not been calculated yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   ofstream ofs(out_file_name, ios::out | ios::binary);
   
   double tmp;
   for (size_t n=0; n<analysis::Nconf*(E_max-E_min+1); n++) {
      tmp = scatt_length[n];
      if (!analysis::machine_is_little_endian())
         analysis::endian_convert(&tmp, 1);
      ofs.write((char*)&tmp, sizeof(double));
   }
   ofs.close();
   
   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output mean of scattering length data with error
 */
//--------------------------------------------------------------------------
void SCATTERING_LENGTH::output_scatt_len_err( const char* out_file_name ) {
   
   func_name = "output_scatt_len_err__";
   analysis::route(class_name, func_name, 1);
   
   if (scatt_length == NULL) {
      analysis::error(1,"scattering length has not been calculated yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   double err, mean, sqr_mean;
   ofstream ofs(out_file_name, ios::out);
   
   for (int E=E_min; E<=E_max; E++) {
      mean     = 0.0;
      sqr_mean = 0.0;
      
      for (int i=0; i<analysis::Nconf; i++) {
         mean     +=     scatt_length[idx(i,E-E_min)];
         sqr_mean += pow(scatt_length[idx(i,E-E_min)],2);
      }
      mean     /= double(analysis::Nconf);
      sqr_mean /= double(analysis::Nconf);
      err       = sqrt(double(analysis::Nconf-1) * (sqr_mean - mean*mean));
      
      ofs << sqrt(2*mass*E) << " " << mean << " " << err << endl;
   }
   ofs.close();
   
   analysis::route(class_name, func_name, 0);
}
