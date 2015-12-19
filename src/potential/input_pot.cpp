//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Functions for input for potential
 * @author  Takaya Miyamoto
 * @since   Mon Aug 31 14:57:36 JST 2015
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

void POTENTIAL::input( R_CORRELATOR& Rcorr ) {
   
   func_name = "input_pot_R___________";
   analysis::route(class_name, func_name, 1);
   
   size_t xyzSIZE = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   
   for (size_t xyz=0; xyz<xyzSIZE; xyz++)
      potential[xyz] = Rcorr(xyz);
   
   analysis::route(class_name, func_name, 0);
}
