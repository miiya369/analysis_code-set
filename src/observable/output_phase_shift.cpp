//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup phase shift
 * @brief   Functions for output phase shift data
 * @author  Takaya Miyamoto
 * @since   Fri Sep  4 03:25:58 JST 2015
 */
//--------------------------------------------------------------------------

#include <observable/phase_shift.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for output phase shift data every gauge configurations
 */
//--------------------------------------------------------------------------
void observable::output_phase_shift_all(  const char* out_file_name
                                        , CONFIG<PHASE_SHIFT> &data) {
    
   string class_name = "________________________________";
   string func_name  = "output_phase_shift_all";
   analysis::route(class_name, func_name, 1);
   
   ofstream ofs(out_file_name, ios::out | ios::binary);
   
   int tmp_Nconf = data.info_conf();
   int tmp_Ndata = data(0).info_data_size();
   
   if (!analysis::machine_is_little_endian()) {
      analysis::endian_convert(&tmp_Nconf, 1);
      analysis::endian_convert(&tmp_Ndata, 1);
   }
   ofs.write((char*)&tmp_Nconf, sizeof(int));
   ofs.write((char*)&tmp_Ndata, sizeof(int));   // write file header
   
   double tmp;
   for (size_t loop=0; loop<data(0).info_data_size(); loop++) {
      
      tmp = data(0).E(loop);
      if (!analysis::machine_is_little_endian())
         analysis::endian_convert(&tmp, 1);
      ofs.write((char*)&tmp, sizeof(double));
      
      for (int conf=0; conf<data.info_conf(); conf++) {
         tmp = data(conf)(loop);
         if (!analysis::machine_is_little_endian())
            analysis::endian_convert(&tmp, 1);
         ofs.write((char*)&tmp, sizeof(double));
      }
   }
   ofs.close();
   
   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output mean of phase shift data with error
 */
//--------------------------------------------------------------------------
void observable::output_phase_shift_err(  const char* out_file_name
                                        , CONFIG<PHASE_SHIFT> &data ) {
   
   string class_name = "________________________________";
   string func_name  = "output_phase_shift_err";
   analysis::route(class_name, func_name, 1);
   
   ofstream ofs(out_file_name, ios::out);
   
   double err, mean, sqr_mean;
   for (size_t iE=0; iE<data(0).info_data_size(); iE++) {
      mean     = 0.0;
      sqr_mean = 0.0;
      
      for (int i=0; i<data.info_conf(); i++) {
         mean     +=     data(i)(iE);
         sqr_mean += pow(data(i)(iE),2);
      }
      mean     /= double(data.info_conf());
      sqr_mean /= double(data.info_conf());
      err       = sqrt(double(data.info_conf()-1) * (sqr_mean - pow(mean,2)));
      
      ofs << data(0).E(iE) << " " << mean << " " << err << endl;
   }
   ofs.close();
   
   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output mean of scattering length data with error
 */
//--------------------------------------------------------------------------
void observable::output_scatt_length(  const char* out_file_name
                                     , CONFIG<PHASE_SHIFT> &data, double mm ) {
   
   string class_name = "________________________________";
   string func_name  = "output_scatt_length___";
   analysis::route(class_name, func_name, 1);
   
   ofstream ofs(out_file_name, ios::out);
   
   int    n_conf = data.info_conf();
   double err, mean, sqr_mean, tmp_E;
   for (size_t iE=0; iE<data(0).info_data_size(); iE++) {
      mean     = 0.0;
      sqr_mean = 0.0;
      tmp_E    = data(0).E(iE);
      
      for (int i=0; i<n_conf; i++) {
         mean     +=      observable::scatt_len(data(i)(iE), tmp_E, mm);
         sqr_mean += pow( observable::scatt_len(data(i)(iE), tmp_E, mm), 2 );
      }
      mean     /= double(n_conf);
      sqr_mean /= double(n_conf);
      err       = sqrt(double(n_conf-1) * (sqr_mean - pow(mean,2)));
      
      ofs << sqrt(2.0 * mm * tmp_E) << " " << mean << " " << err << endl;
   }
   ofs.close();
   
   analysis::route(class_name, func_name, 0);
}
