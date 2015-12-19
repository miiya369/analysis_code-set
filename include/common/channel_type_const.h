//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Header File for definition of channel type
 * @author  Takaya Miyamoto
 * @since   Mon Sep  7 02:49:51 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef CHANNEL_TYPE_CONST_H
#define CHANNEL_TYPE_CONST_H

//--------------------------------------------------------------------------
/**
 * @brief The class for channel type of NBS wave function
 */
//--------------------------------------------------------------------------
class CHANNEL_TYPE {
   
public:
   string name;
   string prop_name;
   string directory;
   string number;
   HADRON_TYPE hadron1;
   HADRON_TYPE hadron2;

   CHANNEL_TYPE() {
      name      = "UNKNOWN";
      prop_name = "UNKNOWN";
      directory = "UNKNOWN";
      number    = "UNKNOWN";
   }
   CHANNEL_TYPE(const CHANNEL_TYPE& other) {
      name      = other.name;
      prop_name = other.prop_name;
      directory = other.directory;
      number    = other.number;
      hadron1   = other.hadron1;
      hadron2   = other.hadron2;
   }
   CHANNEL_TYPE(const string channel_name) {
      set(channel_name);
   }
   CHANNEL_TYPE(const char* channel_name) {
      const string channel_name_str(channel_name);
      set(channel_name_str);
   }
   ~CHANNEL_TYPE() {}
   
   void set(const char* channel_name) {
      const string channel_name_str(channel_name);
      set(channel_name_str);
   }
//====================== definition of channel type ======================//
   void set(const string channel_name) {
      
      if (channel_name == "Pro-Neu_Pro-Neu") {
         name      = channel_name;
         prop_name = "O01O02_O01O02";
         prop_name += "_";
         prop_name += analysis::data_list[SNK_RELA];
         prop_name += "_";
         prop_name += analysis::data_list[SRC_RELA];
         directory = "BBwave.dir";
         number    = "0.00";
         hadron1.set("proton");
         hadron2.set("proton");
      }
//--- particle base S = -1 ---//
      else if (channel_name == "Pro-Lam_Pro-Lam") {
         name      = channel_name;
         prop_name = "O01O08_O01O08";
         prop_name += "_";
         prop_name += analysis::data_list[SNK_RELA];
         prop_name += "_";
         prop_name += analysis::data_list[SRC_RELA];
         directory = "BBwave.dir";
         number    = "1.00";
         hadron1.set("proton");
         hadron2.set("Lambda");
      }
      else if (channel_name == "Pro-Lam_Pro-SigZ") {
         name      = channel_name;
         prop_name = "O01O08_O01O04";
         prop_name += "_";
         prop_name += analysis::data_list[SNK_RELA];
         prop_name += "_";
         prop_name += analysis::data_list[SRC_RELA];
         directory = "BBwave.dir";
         number    = "1.01";
         hadron1.set("proton");
         hadron2.set("Lambda");
      }
      else if (channel_name == "Pro-Lam_Neu-SigP") {
         name      = channel_name;
         prop_name = "O01O08_O02O03";
         prop_name += "_";
         prop_name += analysis::data_list[SNK_RELA];
         prop_name += "_";
         prop_name += analysis::data_list[SRC_RELA];
         directory = "BBwave.dir";
         number    = "1.02";
         hadron1.set("proton");
         hadron2.set("Lambda");
      }
      else if (channel_name == "Pro-SigZ_Pro-Lam") {
         name      = channel_name;
         prop_name = "O01O04_O01O08";
         prop_name += "_";
         prop_name += analysis::data_list[SNK_RELA];
         prop_name += "_";
         prop_name += analysis::data_list[SRC_RELA];
         directory = "BBwave.dir";
         number    = "1.03";
         hadron1.set("proton");
         hadron2.set("Sigma");
      }
      else if (channel_name == "Pro-SigZ_Pro-SigZ") {
         name      = channel_name;
         prop_name = "O01O04_O01O04";
         prop_name += "_";
         prop_name += analysis::data_list[SNK_RELA];
         prop_name += "_";
         prop_name += analysis::data_list[SRC_RELA];
         directory = "BBwave.dir";
         number    = "1.04";
         hadron1.set("proton");
         hadron2.set("Sigma");
      }
      else if (channel_name == "Pro-SigZ_Neu-SigP") {
         name      = channel_name;
         prop_name = "O01O04_O02O03";
         prop_name += "_";
         prop_name += analysis::data_list[SNK_RELA];
         prop_name += "_";
         prop_name += analysis::data_list[SRC_RELA];
         directory = "BBwave.dir";
         number    = "1.05";
         hadron1.set("proton");
         hadron2.set("Sigma");
      }
      else if (channel_name == "Neu-SigP_Pro-Lam") {
         name      = channel_name;
         prop_name = "O02O03_O01O08";
         prop_name += "_";
         prop_name += analysis::data_list[SNK_RELA];
         prop_name += "_";
         prop_name += analysis::data_list[SRC_RELA];
         directory = "BBwave.dir";
         number    = "1.06";
         hadron1.set("proton");
         hadron2.set("Sigma");
      }
      else if (channel_name == "Neu-SigP_Pro-SigZ") {
         name      = channel_name;
         prop_name = "O02O03_O01O04";
         prop_name += "_";
         prop_name += analysis::data_list[SNK_RELA];
         prop_name += "_";
         prop_name += analysis::data_list[SRC_RELA];
         directory = "BBwave.dir";
         number    = "1.07";
         hadron1.set("proton");
         hadron2.set("Sigma");
      }
      else if (channel_name == "Neu-SigP_Neu-SigP") {
         name      = channel_name;
         prop_name = "O02O03_O02O03";
         prop_name += "_";
         prop_name += analysis::data_list[SNK_RELA];
         prop_name += "_";
         prop_name += analysis::data_list[SRC_RELA];
         directory = "BBwave.dir";
         number    = "1.08";
         hadron1.set("proton");
         hadron2.set("Sigma");
      }
//--- particle base S = -2 ---//
      else if (channel_name == "Lam-Lam_Lam-Lam") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.00";
         hadron1.set("Lambda");
         hadron2.set("Lambda");
      }
      else if (channel_name == "Lam-Lam_Pro-XiM") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.01";
         hadron1.set("Lambda");
         hadron2.set("Lambda");
      }
      else if (channel_name == "Lam-Lam_Neu-XiZ") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.02";
         hadron1.set("Lambda");
         hadron2.set("Lambda");
      }
      else if (channel_name == "Lam-Lam_SigP-SigM") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.03";
         hadron1.set("Lambda");
         hadron2.set("Lambda");
      }
      else if (channel_name == "Lam-Lam_SigZ-SigZ") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.04";
         hadron1.set("Lambda");
         hadron2.set("Lambda");
      }
      else if (channel_name == "Lam-Lam_SigZ-Lam") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.05";
         hadron1.set("Lambda");
         hadron2.set("Lambda");
      }
      else if (channel_name == "Pro-XiM_Lam-Lam") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.06";
         hadron1.set("proton");
         hadron2.set("Xi");
      }
      else if (channel_name == "Pro-XiM_Pro-XiM") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.07";
         hadron1.set("proton");
         hadron2.set("Xi");
      }
      else if (channel_name == "Pro-XiM_Neu-XiZ") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.08";
         hadron1.set("proton");
         hadron2.set("Xi");
      }
      else if (channel_name == "Pro-XiM_SigP-SigM") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.09";
         hadron1.set("proton");
         hadron2.set("Xi");
      }
      else if (channel_name == "Pro-XiM_SigZ-SigZ") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.10";
         hadron1.set("proton");
         hadron2.set("Xi");
      }
      else if (channel_name == "Pro-XiM_SigZ-Lam") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.11";
         hadron1.set("proton");
         hadron2.set("Xi");
      }
      else if (channel_name == "Neu-XiZ_Lam-Lam") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.12";
         hadron1.set("proton");
         hadron2.set("Xi");
      }
      else if (channel_name == "Neu-XiZ_Pro-XiM") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.13";
         hadron1.set("proton");
         hadron2.set("Xi");
      }
      else if (channel_name == "Neu-XiZ_Neu-XiZ") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.14";
         hadron1.set("proton");
         hadron2.set("Xi");
      }
      else if (channel_name == "Neu-XiZ_SigP-SigM") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.15";
         hadron1.set("proton");
         hadron2.set("Xi");
      }
      else if (channel_name == "Neu-XiZ_SigZ-SigZ") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.16";
         hadron1.set("proton");
         hadron2.set("Xi");
      }
      else if (channel_name == "Neu-XiZ_SigZ-Lam") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.17";
         hadron1.set("proton");
         hadron2.set("Xi");
      }
      else if (channel_name == "SigP-SigM_Lam-Lam") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.18";
         hadron1.set("Sigma");
         hadron2.set("Sigma");
      }
      else if (channel_name == "SigP-SigM_Pro-XiM") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.19";
         hadron1.set("Sigma");
         hadron2.set("Sigma");
      }
      else if (channel_name == "SigP-SigM_Neu-XiZ") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.20";
         hadron1.set("Sigma");
         hadron2.set("Sigma");
      }
      else if (channel_name == "SigP-SigM_SigP-SigM") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.21";
         hadron1.set("Sigma");
         hadron2.set("Sigma");
      }
      else if (channel_name == "SigP-SigM_SigZ-SigZ") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.22";
         hadron1.set("Sigma");
         hadron2.set("Sigma");
      }
      else if (channel_name == "SigP-SigM_SigZ-Lam") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.23";
         hadron1.set("Sigma");
         hadron2.set("Sigma");
      }
      else if (channel_name == "SigZ-SigZ_Lam-Lam") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.24";
         hadron1.set("Sigma");
         hadron2.set("Sigma");
      }
      else if (channel_name == "SigZ-SigZ_Pro-XiM") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.25";
         hadron1.set("Sigma");
         hadron2.set("Sigma");
      }
      else if (channel_name == "SigZ-SigZ_Neu-XiZ") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.26";
         hadron1.set("Sigma");
         hadron2.set("Sigma");
      }
      else if (channel_name == "SigZ-SigZ_SigP-SigM") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.27";
         hadron1.set("Sigma");
         hadron2.set("Sigma");
      }
      else if (channel_name == "SigZ-SigZ_SigZ-SigZ") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.28";
         hadron1.set("Sigma");
         hadron2.set("Sigma");
      }
      else if (channel_name == "SigZ-SigZ_SigZ-Lam") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.29";
         hadron1.set("Sigma");
         hadron2.set("Sigma");
      }
      else if (channel_name == "SigZ-Lam_Lam-Lam") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.30";
         hadron1.set("Sigma");
         hadron2.set("Lambda");
      }
      else if (channel_name == "SigZ-Lam_Pro-XiM") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.31";
         hadron1.set("Sigma");
         hadron2.set("Lambda");
      }
      else if (channel_name == "SigZ-Lam_Neu-XiZ") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.32";
         hadron1.set("Sigma");
         hadron2.set("Lambda");
      }
      else if (channel_name == "SigZ-Lam_SigP-SigM") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.33";
         hadron1.set("Sigma");
         hadron2.set("Lambda");
      }
      else if (channel_name == "SigZ-Lam_SigZ-SigZ") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.34";
         hadron1.set("Sigma");
         hadron2.set("Lambda");
      }
      else if (channel_name == "SigZ-Lam_SigZ-Lam") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "2.35";
         hadron1.set("Sigma");
         hadron2.set("Lambda");
      }
//--- particle base S = -3 ---//
      else if (channel_name == "XiZ-Lam_XiZ-Lam") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "3.00";
         hadron1.set("Xi");
         hadron2.set("Lambda");
      }
      else if (channel_name == "XiZ-Lam_XiZ-SigZ") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "3.01";
         hadron1.set("Xi");
         hadron2.set("Lambda");
      }
      else if (channel_name == "XiZ-Lam_XiM-SigP") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "3.02";
         hadron1.set("Xi");
         hadron2.set("Lambda");
      }
      else if (channel_name == "XiZ-SigZ_XiZ-Lam") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "3.03";
         hadron1.set("Xi");
         hadron2.set("Sigma");
      }
      else if (channel_name == "XiZ-SigZ_XiZ-SigZ") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "3.04";
         hadron1.set("Xi");
         hadron2.set("Sigma");
      }
      else if (channel_name == "XiZ-SigZ_XiM-SigP") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "3.05";
         hadron1.set("Xi");
         hadron2.set("Sigma");
      }
      else if (channel_name == "XiM-SigP_XiZ-Lam") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "3.06";
         hadron1.set("Xi");
         hadron2.set("Sigma");
      }
      else if (channel_name == "XiM-SigP_XiZ-SigZ") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "3.07";
         hadron1.set("Xi");
         hadron2.set("Sigma");
      }
      else if (channel_name == "XiM-SigP_XiM-SigP") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "3.08";
         hadron1.set("Xi");
         hadron2.set("Sigma");
      }
//--- particle base S = -4 ---//
      else if (channel_name == "XiZ-XiM_XiZ-XiM") {
         name      = channel_name;
         directory = "BBwave.dir";
         number    = "4.00";
         hadron1.set("Xi");
         hadron2.set("Xi");
      }
////--- isospin base S=1, I=1/2 ---//
//      else if (channel_name == "Nuc-Lam12_Nuc-Lam12") {
//         name      = channel_name;
//         directory = "BBwave.prj";
//         number    = "1.00";
//         hadron1.set("proton");
//         hadron2.set("Lambda");
//      }
//      else if (channel_name == "Nuc-Lam12_Nuc-Sig12") {
//         name      = channel_name;
//         directory = "BBwave.prj";
//         number    = "1.01";
//         hadron1.set("proton");
//         hadron2.set("Lambda");
//      }
//      else if (channel_name == "Nuc-Sig12_Nuc-Lam12") {
//         name      = channel_name;
//         directory = "BBwave.prj";
//         number    = "1.02";
//         hadron1.set("proton");
//         hadron2.set("Sigma");
//      }
//      else if (channel_name == "Nuc-Sig12_Nuc-Sig12") {
//         name      = channel_name;
//         directory = "BBwave.prj";
//         number    = "1.03";
//         hadron1.set("proton");
//         hadron2.set("Sigma");
//      }
////--- isospin base S=1, I=3/2 ---//
//      else if (channel_name == "Nuc-Lam12_Nuc-Sig32") {
//         name      = channel_name;
//         directory = "BBwave.prj";
//         number    = "1.04";
//         hadron1.set("proton");
//         hadron2.set("Lambda");
//      }
//      else if (channel_name == "Nuc-Sig32_Nuc-Lam12") {
//         name      = channel_name;
//         directory = "BBwave.prj";
//         number    = "1.05";
//         hadron1.set("proton");
//         hadron2.set("Sigma");
//      }
//      else if (channel_name == "Nuc-Sig32_Nuc-Sig32") {
//         name      = channel_name;
//         directory = "BBwave.prj";
//         number    = "1.06";
//         hadron1.set("proton");
//         hadron2.set("Sigma");
//      }
//---   ---//
      else {
         name      = "UNKNOWN";
         prop_name = "UNKNOWN";
         directory = "UNKNOWN";
         number    = "UNKNOWN";
      }
   }
//========================================================================//
};

#endif
