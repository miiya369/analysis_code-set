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

void POTENTIAL::input( R_CORRELATOR& K_Rcorr, R_CORRELATOR& Rcorr ) {
   
   func_name = "input_pot_K_R_/_R_____";
   analysis::route(class_name, func_name, 1);
   
   size_t xyzSIZE = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   
   for (size_t xyz=0; xyz<xyzSIZE; xyz++)
      potential[xyz] = K_Rcorr(xyz) / Rcorr(xyz);
   
   analysis::route(class_name, func_name, 0);
}

void POTENTIAL::input( NBS_WAVE& K_NBS, NBS_WAVE& NBS, double E ) {
   
   func_name = "input_pot_E-K_NBS_/NBS";
   analysis::route(class_name, func_name, 1);
   
   size_t xyzSIZE = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   
   for (size_t xyz=0; xyz<xyzSIZE; xyz++)
      potential[xyz] = ( E - K_NBS(xyz) ) / NBS(xyz);
   
   analysis::route(class_name, func_name, 0);
}

void potential::set_from_binary(  CONFIG<POTENTIAL> data
                                , const char*       infile_name ) {
   
   string class_name = "________________________________";
   string func_name  = "set_pot_from_binary___";
   analysis::route(class_name, func_name, 1);
   
   int xyzSIZE = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   int data_size, num_conf;
   
   ifstream ifs(infile_name, ios::in | ios::binary);
   if (!ifs) analysis::error(2, infile_name);
   
   ifs.read((char*)&num_conf,  sizeof(int));   // input header
   ifs.read((char*)&data_size, sizeof(int));
   if (!analysis::machine_is_little_endian()) {
      analysis::endian_convert(&num_conf,  1);
      analysis::endian_convert(&data_size, 1);
   }
   if (data_size!=xyzSIZE) analysis::error(3, "Data size is differ");
   
   data.mem_alloc(num_conf);
   for(int i=0; i<num_conf;  i++) {
      data(i).mem_alloc();
      ifs.read((char*)&data(i)(0), sizeof(cdouble) * data_size);
      if (!analysis::machine_is_little_endian())
         analysis::endian_convert(&data(i)(0), data_size);
   }
   ifs.close();
   
   analysis::route(class_name, func_name, 0);
}
