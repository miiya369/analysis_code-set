//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup phase shift
 * @brief   Functions for output phase shift data
 * @author  Takaya Miyamoto
 * @since   Wed Sep 23 18:39:33 JST 2015
 */
//--------------------------------------------------------------------------

#include <observable/phase_shift.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for output phase shift data
 */
//--------------------------------------------------------------------------
void observable::output_phase_shift(  const char* out_file_name
                                    , PHASE_SHIFT &data ) {
   
   string class_name = "________________________________";
   string func_name  = "output_phase_shift____";
   analysis::route(class_name, func_name, 1);
   
   ofstream ofs(out_file_name, ios::out);
   
   for (size_t iE=0; iE<data.info_data_size(); iE++)
      ofs << data.E(iE)
          << " " << data(iE).real() << " " << data(iE).imag() << endl;
   
   ofs.close();
   
   analysis::route(class_name, func_name, 0);
}

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
   
   double  ftmp;
   cdouble ctmp;
   for (size_t loop=0; loop<data(0).info_data_size(); loop++) {
      
      ftmp = data(0).E(loop);
      if (!analysis::machine_is_little_endian())
         analysis::endian_convert(&ftmp, 1);
      ofs.write((char*)&ftmp, sizeof(double));
      
      for (int conf=0; conf<data.info_conf(); conf++) {
         ctmp = data(conf)(loop);
         if (!analysis::machine_is_little_endian())
            analysis::endian_convert(&ctmp, 1);
         ofs.write((char*)&ctmp, sizeof(cdouble));
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
   
   cdouble err, mean, sqr_mean;
   for (size_t iE=0; iE<data(0).info_data_size(); iE++) {
      mean     = COMP_ZERO;
      sqr_mean = COMP_ZERO;
      
      for (int i=0; i<data.info_conf(); i++) {
         mean     +=     data(i)(iE);
         sqr_mean += pow(data(i)(iE),2);
      }
      mean     /= double(data.info_conf());
      sqr_mean /= double(data.info_conf());
      err       = sqrt(double(data.info_conf()-1) * (sqr_mean - pow(mean,2)));
      
      ofs << data(0).E(iE) << " " << mean.real() << " " << err.real()
                           << " " << mean.imag() << " " << err.imag() << endl;
   }
   ofs.close();
   
   analysis::route(class_name, func_name, 0);
}
