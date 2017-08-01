//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Definition of the channel type
 * @author  Takaya Miyamoto
 * @since   Wed Sep 14 16:51:04 JST 2016
 */
//--------------------------------------------------------------------------

#include <common/analysis.h>
#include <common/channel_type.h>

CHANNEL_TYPE::CHANNEL_TYPE() {
   name      = "UNKNOWN";
   prop_name = "UNKNOWN";
   directory = "UNKNOWN";
   OperType  = "UNKNOWN";
   spin_name = "UNKNOWN";
}
CHANNEL_TYPE::CHANNEL_TYPE(const CHANNEL_TYPE& other) {
   name          = other.name;
   prop_name     = other.prop_name;
   directory     = other.directory;
   OperType      = other.OperType;
   hadron1       = other.hadron1;
   hadron2       = other.hadron2;
   flg_spin_proj = other.flg_spin_proj;
   spin_name     = other.spin_name;
}
CHANNEL_TYPE::CHANNEL_TYPE(const string channel_name) {
   set(channel_name);
}
CHANNEL_TYPE::CHANNEL_TYPE(const char* channel_name) {
   const string channel_name_str(channel_name);
   set(channel_name_str);
}

void CHANNEL_TYPE::set(const char* channel_name) {
   const string channel_name_str(channel_name);
   set(channel_name_str);
}
//====================== definition of channel type ======================//
void CHANNEL_TYPE::set(const string channel_name)
{
//~~~~~~~~~ Octet-Octet ~~~~~~~~~//
   if (channel_name == "Pro-Neu_Pro-Neu" ||
       channel_name == "BBwave.dir.S0.00") {
      name      = "Pro-Neu_Pro-Neu";
      prop_name = "O01O02_O01O02";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      directory = "BBwave.dir.S0.00";
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("proton");
      flg_spin_proj = false;
   }
//--- particle basis S = -1 ---//
   else if (channel_name == "Pro-Lam_Pro-Lam" ||
            channel_name == "BBwave.dir.S1.00") {
      name      = "Pro-Lam_Pro-Lam";
      directory = "BBwave.dir.S1.00";
      prop_name = "O01O08_O01O08";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Lambda");
      flg_spin_proj = false;
   }
   else if (channel_name == "Pro-Lam_Pro-SigZ" ||
            channel_name == "BBwave.dir.S1.01") {
      name      = "Pro-Lam_Pro-SigZ";
      directory = "BBwave.dir.S1.01";
      prop_name = "O01O08_O01O04";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Lambda");
      flg_spin_proj = false;
   }
   else if (channel_name == "Pro-Lam_Neu-SigP" ||
            channel_name == "BBwave.dir.S1.02") {
      name      = "Pro-Lam_Neu-SigP";
      directory = "BBwave.dir.S1.02";
      prop_name = "O01O08_O02O03";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Lambda");
      flg_spin_proj = false;
   }
   else if (channel_name == "Pro-SigZ_Pro-Lam" ||
            channel_name == "BBwave.dir.S1.03") {
      name      = "Pro-SigZ_Pro-Lam";
      directory = "BBwave.dir.S1.03";
      prop_name = "O01O04_O01O08";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
   else if (channel_name == "Pro-SigZ_Pro-SigZ" ||
            channel_name == "BBwave.dir.S1.04") {
      name      = "Pro-SigZ_Pro-SigZ";
      directory = "BBwave.dir.S1.04";
      prop_name = "O01O04_O01O04";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
   else if (channel_name == "Pro-SigZ_Neu-SigP" ||
            channel_name == "BBwave.dir.S1.05") {
      name      = "Pro-SigZ_Neu-SigP";
      directory = "BBwave.dir.S1.05";
      prop_name = "O01O04_O02O03";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
   else if (channel_name == "Neu-SigP_Pro-Lam" ||
            channel_name == "BBwave.dir.S1.06") {
      name      = "Neu-SigP_Pro-Lam";
      directory = "BBwave.dir.S1.06";
      prop_name = "O02O03_O01O08";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
   else if (channel_name == "Neu-SigP_Pro-SigZ" ||
            channel_name == "BBwave.dir.S1.07") {
      name      = "Neu-SigP_Pro-SigZ";
      directory = "BBwave.dir.S1.07";
      prop_name = "O02O03_O01O04";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
   else if (channel_name == "Neu-SigP_Neu-SigP" ||
            channel_name == "BBwave.dir.S1.08") {
      name      = "Neu-SigP_Neu-SigP";
      directory = "BBwave.dir.S1.08";
      prop_name = "O02O03_O02O03";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
//--- particle basis S = -2 ---//
   else if (channel_name == "Lam-Lam_Lam-Lam" ||
            channel_name == "BBwave.dir.S2.00") {
      name      = "Lam-Lam_Lam-Lam";
      directory = "BBwave.dir.S2.00";
      prop_name = "O08O08_O08O08";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Lambda");
      hadron2.set("Lambda");
      flg_spin_proj = false;
   }
   else if (channel_name == "Lam-Lam_Pro-XiM" ||
            channel_name == "BBwave.dir.S2.01") {
      name      = "Lam-Lam_Pro-XiM";
      directory = "BBwave.dir.S2.01";
      prop_name = "O08O08_O01O07";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Lambda");
      hadron2.set("Lambda");
      flg_spin_proj = false;
   }
   else if (channel_name == "Lam-Lam_Neu-XiZ" ||
            channel_name == "BBwave.dir.S2.02") {
      name      = "Lam-Lam_Neu-XiZ";
      directory = "BBwave.dir.S2.02";
      prop_name = "O08O08_O02O06";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Lambda");
      hadron2.set("Lambda");
      flg_spin_proj = false;
   }
   else if (channel_name == "Lam-Lam_SigP-SigM" ||
            channel_name == "BBwave.dir.S2.03") {
      name      = "Lam-Lam_SigP-SigM";
      directory = "BBwave.dir.S2.03";
      prop_name = "O08O08_O03O05";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Lambda");
      hadron2.set("Lambda");
      flg_spin_proj = false;
   }
   else if (channel_name == "Lam-Lam_SigZ-SigZ" ||
            channel_name == "BBwave.dir.S2.04") {
      name      = "Lam-Lam_SigZ-SigZ";
      directory = "BBwave.dir.S2.04";
      prop_name = "O08O08_O04O04";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Lambda");
      hadron2.set("Lambda");
      flg_spin_proj = false;
   }
   else if (channel_name == "Lam-Lam_SigZ-Lam" ||
            channel_name == "BBwave.dir.S2.05") {
      name      = "Lam-Lam_SigZ-Lam";
      directory = "BBwave.dir.S2.05";
      prop_name = "O08O08_O04O08";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Lambda");
      hadron2.set("Lambda");
      flg_spin_proj = false;
   }
   else if (channel_name == "Pro-XiM_Lam-Lam" ||
            channel_name == "BBwave.dir.S2.06") {
      name      = "Pro-XiM_Lam-Lam";
      directory = "BBwave.dir.S2.06";
      prop_name = "O01O07_O08O08";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Xi");
      flg_spin_proj = false;
   }
   else if (channel_name == "Pro-XiM_Pro-XiM" ||
            channel_name == "BBwave.dir.S2.07") {
      name      = "Pro-XiM_Pro-XiM";
      directory = "BBwave.dir.S2.07";
      prop_name = "O01O07_O01O07";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Xi");
      flg_spin_proj = false;
   }
   else if (channel_name == "Pro-XiM_Neu-XiZ" ||
            channel_name == "BBwave.dir.S2.08") {
      name      = "Pro-XiM_Neu-XiZ";
      directory = "BBwave.dir.S2.08";
      prop_name = "O01O07_O02O06";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Xi");
      flg_spin_proj = false;
   }
   else if (channel_name == "Pro-XiM_SigP-SigM" ||
            channel_name == "BBwave.dir.S2.09") {
      name      = "Pro-XiM_SigP-SigM";
      directory = "BBwave.dir.S2.09";
      prop_name = "O01O07_O03O05";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Xi");
      flg_spin_proj = false;
   }
   else if (channel_name == "Pro-XiM_SigZ-SigZ" ||
            channel_name == "BBwave.dir.S2.10") {
      name      = "Pro-XiM_SigZ-SigZ";
      directory = "BBwave.dir.S2.10";
      prop_name = "O01O07_O04O04";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Xi");
      flg_spin_proj = false;
   }
   else if (channel_name == "Pro-XiM_SigZ-Lam" ||
            channel_name == "BBwave.dir.S2.11") {
      name      = "Pro-XiM_SigZ-Lam";
      directory = "BBwave.dir.S2.11";
      prop_name = "O01O07_O04O08";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Xi");
      flg_spin_proj = false;
   }
   else if (channel_name == "Neu-XiZ_Lam-Lam" ||
            channel_name == "BBwave.dir.S2.12") {
      name      = "Neu-XiZ_Lam-Lam";
      directory = "BBwave.dir.S2.12";
      prop_name = "O02O06_O08O08";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Xi");
      flg_spin_proj = false;
   }
   else if (channel_name == "Neu-XiZ_Pro-XiM" ||
            channel_name == "BBwave.dir.S2.13") {
      name      = "Neu-XiZ_Pro-XiM";
      directory = "BBwave.dir.S2.13";
      prop_name = "O02O06_O01O07";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Xi");
      flg_spin_proj = false;
   }
   else if (channel_name == "Neu-XiZ_Neu-XiZ" ||
            channel_name == "BBwave.dir.S2.14") {
      name      = "Neu-XiZ_Neu-XiZ";
      directory = "BBwave.dir.S2.14";
      prop_name = "O02O06_O02O06";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Xi");
      flg_spin_proj = false;
   }
   else if (channel_name == "Neu-XiZ_SigP-SigM" ||
            channel_name == "BBwave.dir.S2.15") {
      name      = "Neu-XiZ_SigP-SigM";
      directory = "BBwave.dir.S2.15";
      prop_name = "O02O06_O03O05";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Xi");
      flg_spin_proj = false;
   }
   else if (channel_name == "Neu-XiZ_SigZ-SigZ" ||
            channel_name == "BBwave.dir.S2.16") {
      name      = "Neu-XiZ_SigZ-SigZ";
      directory = "BBwave.dir.S2.16";
      prop_name = "O02O06_O04O04";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Xi");
      flg_spin_proj = false;
   }
   else if (channel_name == "Neu-XiZ_SigZ-Lam" ||
            channel_name == "BBwave.dir.S2.17") {
      name      = "Neu-XiZ_SigZ-Lam";
      directory = "BBwave.dir.S2.17";
      prop_name = "O02O06_O04O08";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Xi");
      flg_spin_proj = false;
   }
   else if (channel_name == "SigP-SigM_Lam-Lam" ||
            channel_name == "BBwave.dir.S2.18") {
      name      = "SigP-SigM_Lam-Lam";
      directory = "BBwave.dir.S2.18";
      prop_name = "O03O05_O08O08";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Sigma");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
   else if (channel_name == "SigP-SigM_Pro-XiM" ||
            channel_name == "BBwave.dir.S2.19") {
      name      = "SigP-SigM_Pro-XiM";
      directory = "BBwave.dir.S2.19";
      prop_name = "O03O05_O01O07";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Sigma");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
   else if (channel_name == "SigP-SigM_Neu-XiZ" ||
            channel_name == "BBwave.dir.S2.20") {
      name      = "SigP-SigM_Neu-XiZ";
      directory = "BBwave.dir.S2.20";
      prop_name = "O03O05_O02O06";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Sigma");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
   else if (channel_name == "SigP-SigM_SigP-SigM" ||
            channel_name == "BBwave.dir.S2.21") {
      name      = "SigP-SigM_SigP-SigM";
      directory = "BBwave.dir.S2.21";
      prop_name = "O03O05_O03O05";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Sigma");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
   else if (channel_name == "SigP-SigM_SigZ-SigZ" ||
            channel_name == "BBwave.dir.S2.22") {
      name      = "SigP-SigM_SigZ-SigZ";
      directory = "BBwave.dir.S2.22";
      prop_name = "O03O05_O04O04";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Sigma");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
   else if (channel_name == "SigP-SigM_SigZ-Lam" ||
            channel_name == "BBwave.dir.S2.23") {
      name      = "SigP-SigM_SigZ-Lam";
      directory = "BBwave.dir.S2.23";
      prop_name = "O03O05_O04O08";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Sigma");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
   else if (channel_name == "SigZ-SigZ_Lam-Lam" ||
            channel_name == "BBwave.dir.S2.24") {
      name      = "SigZ-SigZ_Lam-Lam";
      directory = "BBwave.dir.S2.24";
      prop_name = "O04O04_O08O08";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Sigma");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
   else if (channel_name == "SigZ-SigZ_Pro-XiM" ||
            channel_name == "BBwave.dir.S2.25") {
      name      = "SigZ-SigZ_Pro-XiM";
      directory = "BBwave.dir.S2.25";
      prop_name = "O04O04_O01O07";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Sigma");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
   else if (channel_name == "SigZ-SigZ_Neu-XiZ" ||
            channel_name == "BBwave.dir.S2.26") {
      name      = "SigZ-SigZ_Neu-XiZ";
      directory = "BBwave.dir.S2.26";
      prop_name = "O04O04_O02O06";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Sigma");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
   else if (channel_name == "SigZ-SigZ_SigP-SigM" ||
            channel_name == "BBwave.dir.S2.27") {
      name      = "SigZ-SigZ_SigP-SigM";
      directory = "BBwave.dir.S2.27";
      prop_name = "O04O04_O03O05";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Sigma");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
   else if (channel_name == "SigZ-SigZ_SigZ-SigZ" ||
            channel_name == "BBwave.dir.S2.28") {
      name      = "SigZ-SigZ_SigZ-SigZ";
      directory = "BBwave.dir.S2.28";
      prop_name = "O04O04_O04O04";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Sigma");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
   else if (channel_name == "SigZ-SigZ_SigZ-Lam" ||
            channel_name == "BBwave.dir.S2.29") {
      name      = "SigZ-SigZ_SigZ-Lam";
      directory = "BBwave.dir.S2.29";
      prop_name = "O04O04_O04O08";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Sigma");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
   else if (channel_name == "SigZ-Lam_Lam-Lam" ||
            channel_name == "BBwave.dir.S2.30") {
      name      = "SigZ-Lam_Lam-Lam";
      directory = "BBwave.dir.S2.30";
      prop_name = "O04O08_O08O08";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Sigma");
      hadron2.set("Lambda");
      flg_spin_proj = false;
   }
   else if (channel_name == "SigZ-Lam_Pro-XiM" ||
            channel_name == "BBwave.dir.S2.31") {
      name      = "SigZ-Lam_Pro-XiM";
      directory = "BBwave.dir.S2.31";
      prop_name = "O04O08_O01O07";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Sigma");
      hadron2.set("Lambda");
      flg_spin_proj = false;
   }
   else if (channel_name == "SigZ-Lam_Neu-XiZ" ||
            channel_name == "BBwave.dir.S2.32") {
      name      = "SigZ-Lam_Neu-XiZ";
      directory = "BBwave.dir.S2.32";
      prop_name = "O04O08_O02O06";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Sigma");
      hadron2.set("Lambda");
      flg_spin_proj = false;
   }
   else if (channel_name == "SigZ-Lam_SigP-SigM" ||
            channel_name == "BBwave.dir.S2.33") {
      name      = "SigZ-Lam_SigP-SigM";
      directory = "BBwave.dir.S2.33";
      prop_name = "O04O08_O03O05";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Sigma");
      hadron2.set("Lambda");
      flg_spin_proj = false;
   }
   else if (channel_name == "SigZ-Lam_SigZ-SigZ" ||
            channel_name == "BBwave.dir.S2.34") {
      name      = "SigZ-Lam_SigZ-SigZ";
      directory = "BBwave.dir.S2.34";
      prop_name = "O04O08_O04O04";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Sigma");
      hadron2.set("Lambda");
      flg_spin_proj = false;
   }
   else if (channel_name == "SigZ-Lam_SigZ-Lam" ||
            channel_name == "BBwave.dir.S2.35") {
      name      = "SigZ-Lam_SigZ-Lam";
      directory = "BBwave.dir.S2.35";
      prop_name = "O04O08_O04O08";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Sigma");
      hadron2.set("Lambda");
      flg_spin_proj = false;
   }
//--- particle basis S = -3 ---//
   else if (channel_name == "XiZ-Lam_XiZ-Lam" ||
            channel_name == "BBwave.dir.S3.00") {
      name      = "XiZ-Lam_XiZ-Lam";
      directory = "BBwave.dir.S3.00";
      prop_name = "O06O08_O06O08";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Xi");
      hadron2.set("Lambda");
      flg_spin_proj = false;
   }
   else if (channel_name == "XiZ-Lam_XiZ-SigZ" ||
            channel_name == "BBwave.dir.S3.01") {
      name      = "XiZ-Lam_XiZ-SigZ";
      directory = "BBwave.dir.S3.01";
      prop_name = "O06O08_O06O04";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Xi");
      hadron2.set("Lambda");
      flg_spin_proj = false;
   }
   else if (channel_name == "XiZ-Lam_XiM-SigP" ||
            channel_name == "BBwave.dir.S3.02") {
      name      = "XiZ-Lam_XiM-SigP";
      directory = "BBwave.dir.S3.02";
      prop_name = "O06O08_O07O03";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Xi");
      hadron2.set("Lambda");
      flg_spin_proj = false;
   }
   else if (channel_name == "XiZ-SigZ_XiZ-Lam" ||
            channel_name == "BBwave.dir.S3.03") {
      name      = "XiZ-SigZ_XiZ-Lam";
      directory = "BBwave.dir.S3.03";
      prop_name = "O06O04_O06O08";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Xi");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
   else if (channel_name == "XiZ-SigZ_XiZ-SigZ" ||
            channel_name == "BBwave.dir.S3.04") {
      name      = "XiZ-SigZ_XiZ-SigZ";
      directory = "BBwave.dir.S3.04";
      prop_name = "O06O04_O06O04";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Xi");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
   else if (channel_name == "XiZ-SigZ_XiM-SigP" ||
            channel_name == "BBwave.dir.S3.05") {
      name      = "XiZ-SigZ_XiM-SigP";
      directory = "BBwave.dir.S3.05";
      prop_name = "O06O04_O07O03";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Xi");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
   else if (channel_name == "XiM-SigP_XiZ-Lam" ||
            channel_name == "BBwave.dir.S3.06") {
      name      = "XiM-SigP_XiZ-Lam";
      directory = "BBwave.dir.S3.06";
      prop_name = "O07O03_O06O08";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Xi");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
   else if (channel_name == "XiM-SigP_XiZ-SigZ" ||
            channel_name == "BBwave.dir.S3.07") {
      name      = "XiM-SigP_XiZ-SigZ";
      directory = "BBwave.dir.S3.07";
      prop_name = "O07O03_O06O04";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Xi");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
   else if (channel_name == "XiM-SigP_XiM-SigP" ||
            channel_name == "BBwave.dir.S3.08") {
      name      = "XiM-SigP_XiM-SigP";
      directory = "BBwave.dir.S3.08";
      prop_name = "O07O03_O07O03";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Xi");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
//--- particle basis S = -4 ---//
   else if (channel_name == "XiZ-XiM_XiZ-XiM" ||
            channel_name == "BBwave.dir.S4.00") {
      name      = "XiZ-XiM_XiZ-XiM";
      directory = "BBwave.dir.S4.00";
      prop_name = "O06O07_O06O07";
      prop_name += "_";
      prop_name += analysis::data_list[SNK_RELA];
      prop_name += "_";
      prop_name += analysis::data_list[SRC_RELA];
      OperType  = "NUC";
      hadron1.set("Xi");
      hadron2.set("Xi");
      flg_spin_proj = false;
   }
//--- isospin basis S = -1, I = 1/2 ---//
   else if (channel_name == "Nuc-Lam12_Nuc-Lam12" ||
            channel_name == "BBwave.iso.S1.00") {
      name      = "Nuc-Lam12_Nuc-Lam12";
      directory = "BBwave.iso.S1.00";
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Lambda");
      flg_spin_proj = false;
   }
   else if (channel_name == "Nuc-Lam12_Nuc-Sig12" ||
            channel_name == "BBwave.iso.S1.01") {
      name      = "Nuc-Lam12_Nuc-Sig12";
      directory = "BBwave.iso.S1.01";
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Lambda");
      flg_spin_proj = false;
   }
   else if (channel_name == "Nuc-Sig12_Nuc-Lam12" ||
            channel_name == "BBwave.iso.S1.02") {
      name      = "Nuc-Sig12_Nuc-Lam12";
      directory = "BBwave.iso.S1.02";
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
   else if (channel_name == "Nuc-Sig12_Nuc-Sig12" ||
            channel_name == "BBwave.iso.S1.03") {
      name      = "Nuc-Sig12_Nuc-Sig12";
      directory = "BBwave.iso.S1.03";
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
//--- isospin basis S = -1, I = 3/2 ---//
   else if (channel_name == "Nuc-Sig32_Nuc-Sig32" ||
            channel_name == "BBwave.iso.S1.04") {
      name      = "Nuc-Sig32_Nuc-Sig32";
      directory = "BBwave.iso.S1.04";
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
//--- isospin basis S = -3, I = 1/2 ---//
   else if (channel_name == "Xi-Lam12_Xi-Lam12" ||
            channel_name == "BBwave.iso.S3.00") {
      name      = "Xi-Lam12_Xi-Lam12";
      directory = "BBwave.iso.S3.00";
      OperType  = "NUC";
      hadron1.set("Xi");
      hadron2.set("Lambda");
      flg_spin_proj = false;
   }
   else if (channel_name == "Xi-Lam12_Xi-Sig12" ||
            channel_name == "BBwave.iso.S3.01") {
      name      = "Xi-Lam12_Xi-Sig12";
      directory = "BBwave.iso.S3.01";
      OperType  = "NUC";
      hadron1.set("Xi");
      hadron2.set("Lambda");
      flg_spin_proj = false;
   }
   else if (channel_name == "Xi-Sig12_Xi-Lam12" ||
            channel_name == "BBwave.iso.S3.02") {
      name      = "Xi-Sig12_Xi-Lam12";
      directory = "BBwave.iso.S3.02";
      OperType  = "NUC";
      hadron1.set("Xi");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
   else if (channel_name == "Xi-Sig12_Xi-Sig12" ||
            channel_name == "BBwave.iso.S3.03") {
      name      = "Xi-Sig12_Xi-Sig12";
      directory = "BBwave.iso.S3.03";
      OperType  = "NUC";
      hadron1.set("Xi");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
//--- isospin basis S = -2, I = 0 ---//
   else if (channel_name == "Lam-Lam0_Lam-Lam0" ||
            channel_name == "BBwave.iso.S2.00") {
      name      = "Lam-Lam0_Lam-Lam0";
      directory = "BBwave.iso.S2.00";
      OperType  = "NUC";
      hadron1.set("Lambda");
      hadron2.set("Lambda");
      flg_spin_proj = false;
   }
   else if (channel_name == "Lam-Lam0_Sig-Sig0" ||
            channel_name == "BBwave.iso.S2.01") {
      name      = "Lam-Lam0_Sig-Sig0";
      directory = "BBwave.iso.S2.01";
      OperType  = "NUC";
      hadron1.set("Lambda");
      hadron2.set("Lambda");
      flg_spin_proj = false;
   }
   else if (channel_name == "Lam-Lam0_Nuc-Xi0" ||
            channel_name == "BBwave.iso.S2.02") {
      name      = "Lam-Lam0_Nuc-Xi0";
      directory = "BBwave.iso.S2.02";
      OperType  = "NUC";
      hadron1.set("Lambda");
      hadron2.set("Lambda");
      flg_spin_proj = false;
   }
   else if (channel_name == "Sig-Sig0_Lam-Lam0" ||
            channel_name == "BBwave.iso.S2.03") {
      name      = "Sig-Sig0_Lam-Lam0";
      directory = "BBwave.iso.S2.03";
      OperType  = "NUC";
      hadron1.set("Sigma");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
   else if (channel_name == "Sig-Sig0_Sig-Sig0" ||
            channel_name == "BBwave.iso.S2.04") {
      name      = "Sig-Sig0_Sig-Sig0";
      directory = "BBwave.iso.S2.04";
      OperType  = "NUC";
      hadron1.set("Sigma");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
   else if (channel_name == "Sig-Sig0_Nuc-Xi0" ||
            channel_name == "BBwave.iso.S2.05") {
      name      = "Sig-Sig0_Nuc-Xi0";
      directory = "BBwave.iso.S2.05";
      OperType  = "NUC";
      hadron1.set("Sigma");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
   else if (channel_name == "Nuc-Xi0_Lam-Lam0" ||
            channel_name == "BBwave.iso.S2.06") {
      name      = "Nuc-Xi0_Lam-Lam0";
      directory = "BBwave.iso.S2.06";
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Xi");
      flg_spin_proj = false;
   }
   else if (channel_name == "Nuc-Xi0_Sig-Sig0" ||
            channel_name == "BBwave.iso.S2.07") {
      name      = "Nuc-Xi0_Sig-Sig0";
      directory = "BBwave.iso.S2.07";
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Xi");
      flg_spin_proj = false;
   }
   else if (channel_name == "Nuc-Xi0_Nuc-Xi0" ||
            channel_name == "BBwave.iso.S2.08") {
      name      = "Nuc-Xi0_Nuc-Xi0";
      directory = "BBwave.iso.S2.08";
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Xi");
      flg_spin_proj = false;
   }
//--- isospin basis S = -2, I = 1 ---//
   else if (channel_name == "Nuc-Xi1_Nuc-Xi1" ||
            channel_name == "BBwave.iso.S2.09") {
      name      = "Nuc-Xi1_Nuc-Xi1";
      directory = "BBwave.iso.S2.09";
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Xi");
      flg_spin_proj = false;
   }
   else if (channel_name == "Nuc-Xi1_Sig-Lam1" ||
            channel_name == "BBwave.iso.S2.10") {
      name      = "Nuc-Xi1_Sig-Lam1";
      directory = "BBwave.iso.S2.10";
      OperType  = "NUC";
      hadron1.set("proton");
      hadron2.set("Xi");
      flg_spin_proj = false;
   }
   else if (channel_name == "Sig-Lam1_Nuc-Xi1" ||
            channel_name == "BBwave.iso.S2.11") {
      name      = "Sig-Lam1_Nuc-Xi1";
      directory = "BBwave.iso.S2.11";
      OperType  = "NUC";
      hadron1.set("Sigma");
      hadron2.set("Lambda");
      flg_spin_proj = false;
   }
   else if (channel_name == "Sig-Lam1_Sig-Lam1" ||
            channel_name == "BBwave.iso.S2.12") {
      name      = "Sig-Lam1_Sig-Lam1";
      directory = "BBwave.iso.S2.12";
      OperType  = "NUC";
      hadron1.set("Sigma");
      hadron2.set("Lambda");
      flg_spin_proj = false;
   }
//--- isospin basis S = -2, I = 2 ---//
   else if (channel_name == "Sig-Sig2_Sig-Sig2" ||
            channel_name == "BBwave.iso.S2.13") {
      name      = "Sig-Sig2_Sig-Sig2";
      directory = "BBwave.iso.S2.13";
      OperType  = "NUC";
      hadron1.set("Sigma");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
//--- isospin basis S = -3, I = 3/2 ---//
   else if (channel_name == "Xi-Sig32_Xi-Sig32" ||
            channel_name == "BBwave.iso.S3.04") {
      name      = "Xi-Sig32_Xi-Sig32";
      directory = "BBwave.iso.S3.04";
      OperType  = "NUC";
      hadron1.set("Xi");
      hadron2.set("Sigma");
      flg_spin_proj = false;
   }
//~~~~~~~~~ Meson-Meson ~~~~~~~~~//
   else if (channel_name == "PiP-PiP_PiP-PiP" ||
            channel_name == "PPwave.dir.S0.00") {
      name      = "PiP-PiP_PiP-PiP";
      directory = "PPwave.dir.S0.00";
      OperType  = "PP";
      hadron1.set("pion");
      hadron2.set("pion");
      flg_spin_proj = true;
      spin_name     = "S0_Sz0";
   }
   else if (channel_name == "PiP-KaZ_PiP-KaZ" ||
            channel_name == "PPwave.dir.S1.00") {
      name      = "PiP-KaZ_PiP-KaZ";
      directory = "PPwave.dir.S1.00";
      OperType  = "PP";
      hadron1.set("pion");
      hadron2.set("Kaon");
      flg_spin_proj = true;
      spin_name     = "S0_Sz0";
   }
   else if (channel_name == "KaZ-KaZ_KaZ-KaZ" ||
            channel_name == "PPwave.dir.S2.00") {
      name      = "KaZ-KaZ_KaZ-KaZ";
      directory = "PPwave.dir.S2.00";
      OperType  = "PP";
      hadron1.set("Kaon");
      hadron2.set("Kaon");
      flg_spin_proj = true;
      spin_name     = "S0_Sz0";
   }
//~~~~~~~~~ Decuplet-Decuplet ~~~~~~~~~//
   else if (channel_name == "OmgM-OmgM_OmgM-OmgM" ||
            channel_name == "Proj.DDwave.dir.S6.00/spin0z+0.0z+0") {
      name      = "OmgM-OmgM_OmgM-OmgM";
      directory = "Proj.DDwave.dir.S6.00/spin0z+0.0z+0";
      OperType  = "DD";
      hadron1.set("Omega");
      hadron2.set("Omega");
      flg_spin_proj = true;
      spin_name     = "S0_Sz0";
   }
//~~~~~~~~~ Octet-Decuplet ~~~~~~~~~//
   else if (channel_name == "OmgM-Pro_OmgM-Pro_spin1" ||
            channel_name == "Proj.DOwave.dir.S3.35/spin1_ave") {
      name      = "OmgM-Pro_OmgM-Pro";
      directory = "Proj.DOwave.dir.S3.35/spin1_ave";
      OperType  = "DO";
      hadron1.set("Omega");
      hadron2.set("proton");
      flg_spin_proj = true;
      spin_name     = "S1_Sz0";
   }
   else if (channel_name == "OmgM-Pro_OmgM-Pro_spin2" ||
            channel_name == "Proj.DOwave.dir.S3.35/spin2_ave") {
      name      = "OmgM-Pro_OmgM-Pro";
      directory = "Proj.DOwave.dir.S3.35/spin2_ave";
      OperType  = "DO";
      hadron1.set("Omega");
      hadron2.set("proton");
      flg_spin_proj = true;
      spin_name     = "S2_Sz0";
   }
   else if (channel_name == "OmgM-XiZ_OmgM-XiZ_spin1" ||
            channel_name == "Proj.DOwave.dir.S5.00/spin1_ave") {
     name      = "OmgM-XiZ_OmgM-XiZ";
     directory = "Proj.DOwave.dir.S5.00/spin1_ave";
     OperType  = "DO";
     hadron1.set("Omega");
     hadron2.set("Xi");
     flg_spin_proj = true;
     spin_name     = "S1_Sz0";
   }
   else if (channel_name == "OmgM-XiZ_OmgM-XiZ_spin2" ||
            channel_name == "Proj.DOwave.dir.S5.00/spin2_ave") {
      name      = "OmgM-XiZ_OmgM-XiZ";
      directory = "Proj.DOwave.dir.S5.00/spin2_ave";
      OperType  = "DO";
      hadron1.set("Omega");
      hadron2.set("Xi");
      flg_spin_proj = true;
      spin_name     = "S2_Sz0";
   }
//~~~~~~~~~ Others ~~~~~~~~~//
   else {
      name      = "UNKNOWN";
      prop_name = "UNKNOWN";
      directory = "UNKNOWN";
      OperType  = "UNKNOWN";
      spin_name = "UNKNOWN";
   }
}
//========================================================================//
