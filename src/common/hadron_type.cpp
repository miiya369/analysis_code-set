//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Definition of the hadron type
 * @author  Takaya Miyamoto
 * @since   Thu Sep 15 00:10:27 JST 2016
 */
//--------------------------------------------------------------------------

#include <common/analysis.h>
#include <common/hadron_type.h>

HADRON_TYPE::HADRON_TYPE() {
   name        = "UNKNOWN";
   name_only   = "UNKNOWN";
   fb_mean_flg = false;
}
HADRON_TYPE::HADRON_TYPE(const HADRON_TYPE& other) {
   name        = other.name;
   name_only   = other.name_only;
   fb_mean_flg = other.fb_mean_flg;
}
HADRON_TYPE::HADRON_TYPE(const string had_name) {
   set(had_name);
}
HADRON_TYPE::HADRON_TYPE(const char* had_name) {
   const string had_name_str(had_name);
   set(had_name_str);
}

void HADRON_TYPE::set(const char* had_name) {
   const string had_name_str(had_name);
   set(had_name_str);
}
//======================== definition of hadron name =====================//
void HADRON_TYPE::set(const string had_name)
{
//--- meson ---//
   if (had_name == "pion") {
      name        = had_name;
      name_only   = had_name;
      fb_mean_flg = false;
   }
   else if (had_name == "Kaon") {
      name        = had_name;
      name_only   = had_name;
      fb_mean_flg = false;
   }
   else if (had_name == "etaSS") {
      name        = had_name;
      name_only   = had_name;
      fb_mean_flg = false;
   }
   else if (had_name == "rho") {
      name        = had_name;
      name_only   = had_name;
      fb_mean_flg = false;
   }
   else if (had_name == "Kstar") {
      name        = had_name;
      name_only   = had_name;
      fb_mean_flg = false;
   }
   else if (had_name == "phi") {
      name        = had_name;
      name_only   = had_name;
      fb_mean_flg = false;
   }
//--- baryon ---//
   else if (had_name == "proton") {
      name        = had_name;
      name_only   = had_name;
      name       += "_";
      name       += analysis::data_list[SNK_RELA];
      name       += "_";
      name       += analysis::data_list[SRC_RELA];
      fb_mean_flg = true;
   }
   else if (had_name == "Sigma") {
      name        = had_name;
      name_only   = had_name;
      name       += "_";
      name       += analysis::data_list[SNK_RELA];
      name       += "_";
      name       += analysis::data_list[SRC_RELA];
      fb_mean_flg = true;
   }
   else if (had_name == "Xi") {
      name        = had_name;
      name_only   = had_name;
      name       += "_";
      name       += analysis::data_list[SNK_RELA];
      name       += "_";
      name       += analysis::data_list[SRC_RELA];
      fb_mean_flg = true;
   }
   else if (had_name == "Lambda") {
      name        = had_name;
      name_only   = had_name;
      name       += "_";
      name       += analysis::data_list[SNK_RELA];
      name       += "_";
      name       += analysis::data_list[SRC_RELA];
      fb_mean_flg = true;
   }
   else if (had_name == "Delta") {
      name        = had_name;
      name_only   = had_name;
      fb_mean_flg = true;
   }
   else if (had_name == "Omega") {
      name        = had_name;
      name_only   = had_name;
      fb_mean_flg = true;
   }
   else if (had_name == "Sigma32") {
      name        = had_name;
      name_only   = had_name;
      fb_mean_flg = true;
   }
   else if (had_name == "Xi32") {
      name        = had_name;
      name_only   = had_name;
      fb_mean_flg = true;
   }
   else if (had_name == "OMEGA") {
      name        = had_name;
      name_only   = had_name;
      fb_mean_flg = true;
   }
//--- Others ---//
   else {
      name        = "UNKNOWN";
      name_only   = "UNKNOWN";
      fb_mean_flg = false;
   }
}
//========================================================================//
