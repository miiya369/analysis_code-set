//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Functions for input for potential from binary file
 * @author  Takaya Miyamoto
 * @since   Wed Jul 22 05:57:25 JST 2015
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>

void POTENTIAL::set_pot_from_binary(const char* infile_name ) {
    
   func_name = "set_pot_from_binary___";
   analysis::route(class_name, func_name, 1);
   
   NBSwave::xyzSIZE  = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   NBSwave::xyznSIZE = NBSwave::xyzSIZE * analysis::Nconf;
   
   if (potential == NULL) potential = new cdouble[NBSwave::xyznSIZE];
   if (Rcorr     != NULL) delete_pot();
   
   ifstream ifs(infile_name, ios::in | ios::binary);
   if (!ifs) analysis::error(2, infile_name);
   
   ifs.read((char*)&potential[0], sizeof(cdouble) * NBSwave::xyznSIZE);
   ifs.close();
   
   if (!analysis::machine_is_little_endian())
      analysis::endian_convert(potential, NBSwave::xyznSIZE);
   
   func_name = "set_pot_from_binary___";
   analysis::route(class_name, func_name, 0);
}
