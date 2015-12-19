//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Header File for definition of hadron type
 * @author  Takaya Miyamoto
 * @since   Fri Jul 24 02:12:01 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef HADRON_NAME_CONST_H
#define HADRON_NAME_CONST_H

//--------------------------------------------------------------------------
/**
 * @brief The class for hadron type
 */
//--------------------------------------------------------------------------
class HADRON_TYPE {
   
public:
   string name, name_only;
   bool   fb_mean_flg;

   HADRON_TYPE() {
      name        = "UNKNOWN";
      name_only   = "UNKNOWN";
      fb_mean_flg = false;
   }
   HADRON_TYPE(const HADRON_TYPE& other) {
      name        = other.name;
      name_only   = other.name_only;
      fb_mean_flg = other.fb_mean_flg;
   }
   HADRON_TYPE(const string had_name) {
      set(had_name);
   }
   HADRON_TYPE(const char* had_name) {
      const string had_name_str(had_name);
      set(had_name_str);
   }
   ~HADRON_TYPE() {}
   
   void set(const char* had_name) {
      const string had_name_str(had_name);
      set(had_name_str);
   }
//======================== definition of hadron name =====================//
   void set(const string had_name) {
   
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
//---   ---//
      else {
         name        = "UNKNOWN";
         name_only   = "UNKNOWN";
         fb_mean_flg = false;
      }
   }
//========================================================================//
};

#endif
