//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Definition of the spin type
 * @author  Takaya Miyamoto
 * @since   Thu Sep 15 00:10:27 JST 2016
 */
//--------------------------------------------------------------------------

#include <common/analysis.h>
#include <common/spin_type.h>

SPIN_TYPE::SPIN_TYPE() {
   name   = "UNKNOWN";
   number = 999;
}
SPIN_TYPE::SPIN_TYPE(const SPIN_TYPE& other) {
   name   = other.name;
   number = other.number;
}
SPIN_TYPE::SPIN_TYPE(const string spin_name) {
   set(spin_name);
}
SPIN_TYPE::SPIN_TYPE(const char* spin_name) {
   const string spin_name_str(spin_name);
   set(spin_name_str);
}
SPIN_TYPE::SPIN_TYPE(const int spin_type) {
   set(spin_type);
}
void SPIN_TYPE::set(const char* spin_name) {
   const string spin_name_str(spin_name);
   set(spin_name_str);
}
//======================== definition of spin type =====================//
void SPIN_TYPE::set(const string spin_name)
{
   if (spin_name == "S0_Sz0") {
      name   = spin_name;
      number = SPIN_0_0;
   }
   else if (spin_name == "S1_Sz+1") {
      name   = spin_name;
      number = SPIN_1_p1;
   }
   else if (spin_name == "S1_Sz0") {
      name   = spin_name;
      number = SPIN_1_0;
   }
   else if (spin_name == "S1_Sz0ud") {
      name   = spin_name;
      number = SPIN_1_0ud;
   }
   else if (spin_name == "S1_Sz0du") {
      name   = spin_name;
      number = SPIN_1_0du;
   }
   else if (spin_name == "S1_Sz-1") {
      name   = spin_name;
      number = SPIN_1_m1;
   }
   else if (spin_name == "S1_SzAve") {
      name   = spin_name;
      number = SPIN_1_ave;
   }
   else if (spin_name == "S2_Sz0") {
      name   = spin_name;
      number = SPIN_2_0;
   }
//---   ---//
   else {
      name   = "UNKNOWN";
      number = 999;
   }
}
//------------------------------------------------------------------------//
void SPIN_TYPE::set(const int spin_type)
{
   if (spin_type == SPIN_0_0) {
      name   = "S0_Sz0";
      number = spin_type;
   }
   else if (spin_type == SPIN_1_p1) {
      name   = "S1_Sz+1";
      number = spin_type;
   }
   else if (spin_type == SPIN_1_0) {
      name   = "S1_Sz0";
      number = spin_type;
   }
   else if (spin_type == SPIN_1_0ud) {
      name   = "S1_Sz0ud";
      number = spin_type;
   }
   else if (spin_type == SPIN_1_0du) {
      name   = "S1_Sz0du";
      number = spin_type;
   }
   else if (spin_type == SPIN_1_m1) {
      name   = "S1_Sz-1";
      number = spin_type;
   }
   else if (spin_type == SPIN_1_ave) {
      name   = "S1_SzAve";
      number = spin_type;
   }
   else if (spin_type == SPIN_2_0) {
      name   = "S2_Sz0";
      number = spin_type;
   }
//---   ---//
   else {
      name   = "UNKNOWN";
      number = 999;
   }
}
