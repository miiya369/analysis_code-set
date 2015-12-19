//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup scattering length
 * @brief   Function for input phase shift data
 * @author  Takaya Miyamoto
 * @since   Fri Sep  4 03:23:49 JST 2015
 */
//--------------------------------------------------------------------------

#include <observable/observable_base.h>

//--------------------------------------------------------------------------
/**
 * @brief The function for input binary-phase shift data (for header)
 */
//--------------------------------------------------------------------------
void observable::input_phase_shift(  const char* infile_name
                                   , int &n_conf, int &n_data ) {
   
   string class_name = "________________________________";
   string func_name = "input_phase_shift_head";
   analysis::route(class_name, func_name, 1);
   
   ifstream ifs(infile_name, ios::in | ios::binary);
   if(!ifs) analysis::error(2, infile_name);
   
   int tmp_Nconf, tmp_Ndata;
   
   ifs.read((char*)&tmp_Nconf, sizeof(int));
   ifs.read((char*)&tmp_Ndata, sizeof(int));   // read file header
   
   if (!analysis::machine_is_little_endian()) {
      analysis::endian_convert(&tmp_Nconf, 1);
      analysis::endian_convert(&tmp_Ndata, 1);
   }
   printf(" @ #.confs = %d\n", tmp_Nconf);
   printf(" @ #.datas = %d\n", tmp_Ndata);
   ifs.close();
   
   n_conf = tmp_Nconf;
   n_data = tmp_Ndata;
   
   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief The function for input binary-phase shift data (for body)
 */
//--------------------------------------------------------------------------
void observable::input_phase_shift(  const char* infile_name
                                   , double *energy, double *phase ) {
   
   string class_name = "________________________________";
   string func_name = "input_phase_shift_body";
   analysis::route(class_name, func_name, 1);
   
   ifstream ifs(infile_name, ios::in | ios::binary);
   if(!ifs) analysis::error(2, infile_name);
   
   int tmp_Nconf, tmp_Ndata;
   
   ifs.read((char*)&tmp_Nconf, sizeof(int));
   ifs.read((char*)&tmp_Ndata, sizeof(int));   // read file header
   
   if (!analysis::machine_is_little_endian()) {
      analysis::endian_convert(&tmp_Nconf, 1);
      analysis::endian_convert(&tmp_Ndata, 1);
   }
   double tmp;
   for (int loop=0; loop<tmp_Ndata; loop++) {
      
      ifs.read((char*)&tmp, sizeof(double));
      if (!analysis::machine_is_little_endian())
         analysis::endian_convert(&tmp, 1);
      energy[loop] = tmp;
      
      for (int conf=0; conf<tmp_Nconf; conf++) {
         ifs.read((char*)&tmp, sizeof(double));
         if (!analysis::machine_is_little_endian())
            analysis::endian_convert(&tmp, 1);
         phase[conf+tmp_Nconf*loop] = tmp;
      }
   }
   ifs.close();
   
   analysis::route(class_name, func_name, 0);
}
