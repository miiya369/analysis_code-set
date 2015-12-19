//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Functions for input for potential from binary file
 * @author  Takaya Miyamoto
 * @since   Mon Aug 31 16:46:51 JST 2015
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>

void POTENTIAL::set_from_binary( const char* infile_name ) {
    
   func_name = "set_pot_from_binary___";
   analysis::route(class_name, func_name, 1);
   
   size_t xyzSIZE = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   if (potential == NULL) potential = new cdouble[xyzSIZE];
   
   ifstream ifs(infile_name, ios::in | ios::binary);
   if (!ifs) analysis::error(2, infile_name);
   
   ifs.read((char*)&potential[0], sizeof(cdouble) * xyzSIZE);
   ifs.close();
   
   if (!analysis::machine_is_little_endian())
      analysis::endian_convert(potential, xyzSIZE);
   
   analysis::route(class_name, func_name, 0);
}
