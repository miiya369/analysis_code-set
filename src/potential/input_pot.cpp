//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Functions for input for potential
 * @author  Takaya Miyamoto
 * @since   Fri Dec 18 00:56:00 JST 2015
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>

void POTENTIAL::input(const R_CORRELATOR& K_Rcorr, const R_CORRELATOR& Rcorr) {
   DEBUG_LOG
   
   for (int xyz=0; xyz<data_size(); xyz++)
      potential[xyz] = K_Rcorr(xyz) / Rcorr(xyz);
}

void POTENTIAL::input(const NBS_WAVE& K_NBS, const NBS_WAVE& NBS, const double E) {
   DEBUG_LOG
   
   for (int xyz=0; xyz<data_size(); xyz++)
      potential[xyz] = (E - K_NBS(xyz)) / NBS(xyz);
}

void potential::set_from_binary(CONFIG<POTENTIAL> data, const char* infile_name) {
   DEBUG_LOG
   
   int xyzSIZE = data(0).data_size();
   int data_size, num_conf;
   
   ifstream ifs(infile_name, ios::in | ios::binary);
   if (!ifs) ERROR_FOPEN(infile_name);
   
   ifs.read((char*)&num_conf,  sizeof(int));   // input header
   ifs.read((char*)&data_size, sizeof(int));
   if (!analysis::machine_is_little_endian()) {
      analysis::endian_convert(&num_conf,  1);
      analysis::endian_convert(&data_size, 1);
   }
   if (data_size != xyzSIZE) ERROR_COMMENTS("Data size is differ");
   
   data.mem_alloc(num_conf);
   for(int i=0; i<num_conf;  i++) {
      data(i).mem_alloc();
      ifs.read((char*)&data(i)(0), sizeof(cdouble) * data_size);
      if (!analysis::machine_is_little_endian())
         analysis::endian_convert(&data(i)(0), data_size);
   }
   ifs.close();
}
