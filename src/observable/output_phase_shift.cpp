//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup phase shift
 * @brief   Functions for output phase shift data
 * @author  Takaya Miyamoto
 * @since   Fri Dec 11 22:25:13 JST 2015
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
   
   for (size_t iE=0; iE<data.data_size(); iE++)
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
   
   int tmp_Nconf = data.Nconf();
   int tmp_Ndata = data(0).data_size();
   
   if (!analysis::machine_is_little_endian()) {
      analysis::endian_convert(&tmp_Nconf, 1);
      analysis::endian_convert(&tmp_Ndata, 1);
   }
   ofs.write((char*)&tmp_Nconf, sizeof(int));
   ofs.write((char*)&tmp_Ndata, sizeof(int));   // write file header
   
   double  ftmp;
   cdouble ctmp;
   for (size_t loop=0; loop<data(0).data_size(); loop++) {
      
      ftmp = data(0).E(loop);
      if (!analysis::machine_is_little_endian())
         analysis::endian_convert(&ftmp, 1);
      ofs.write((char*)&ftmp, sizeof(double));
      
      for (int conf=0; conf<data.Nconf(); conf++) {
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
                                        , CONFIG<PHASE_SHIFT> &data
                                        , bool is_jack_knife_data ) {
   
   string class_name = "________________________________";
   string func_name  = "output_phase_shift_err";
   analysis::route(class_name, func_name, 1);
   
   cdouble err, mean, sqr_mean;
   int     num_conf  = data.Nconf();
   int     data_size = data(0).data_size();
   
   double  factor;
   if (is_jack_knife_data)
      factor = double(num_conf-1);
   else
      factor = 1.0 / double(num_conf-1);
   
   ofstream ofs(out_file_name, ios::out);
   ofs << "#   t, mean.real, err.real, mean.imag, err.imag" << endl;
   
   for (int iE=0; iE<data_size; iE++) {
      mean     = COMP_ZERO;
      sqr_mean = COMP_ZERO;
      
      for (int i=0; i<num_conf; i++) {
         mean     +=          data(i)(iE);
         sqr_mean += cmp_sqr( data(i)(iE) );
      }
      mean     /= double(num_conf);
      sqr_mean /= double(num_conf);
      err       = cmp_sqrt(factor * (sqr_mean - pow(mean,2)) );
      
      ofs << data(0).E(iE) << " " << mean.real() << " " << err.real()
                           << " " << mean.imag() << " " << err.imag() << endl;
   }
   ofs.close();
   
   analysis::route(class_name, func_name, 0);
}
