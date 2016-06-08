//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Setting path of the gauge confs.
 * @author  Takaya Miyamoto
 * @since   Wed Jun  8 17:05:20 JST 2016
 */
//--------------------------------------------------------------------------

#include <common/analysis.h>

bool analysis::use_doi_san_path = false;

string analysis::set_path( int num_conf ) {
   
   char file_name[1024];
   char dir_name[30];
   char header[60];
   char footer[30] = "\0";
   
   if(strcmp(data_list[N_TIME],"-1")==0){
      snprintf(  dir_name, sizeof(dir_name), "correlator.%s.dir"
               , data_list[CORR_DIRECTORY] );
      snprintf(  header,   sizeof(header),   "%s_correlator."
               , data_list[HADRON_NAME]    );
   }
   else{
      snprintf(  dir_name, sizeof(dir_name), "%s.S%s"
               , data_list[NBS_DIRECTORY], data_list[N_CHANNEL]   );
      snprintf(  header,   sizeof(header),   "NBSwave.+%s", data_list[N_TIME] );
      snprintf(  footer,   sizeof(footer),   ".%s_%s.%s_%s"
               , data_list[OPER_TYPE], data_list[SNK_RELA]
               , data_list[OPER_TYPE], data_list[SRC_RELA] );
   }
   if (!use_doi_san_path)
      snprintf(  file_name, sizeof(file_name)
               ,"%s/%s/%s/%s+%s.%s.%s.%s.%s%s"
               , data_list[MAIN_PATH], dir_name
               , data_list[num_conf+MAX_PATH_ARG]
               , header, data_list[N_T_SHIFT], data_list[N_X_SHIFT]
               , data_list[N_Y_SHIFT], data_list[N_Z_SHIFT]
               , data_list[num_conf+MAX_PATH_ARG], footer );
   else
      snprintf(  file_name, sizeof(file_name)
               ,"%s/%s/%s/%s+%s.%s%s"
               , data_list[MAIN_PATH], dir_name
               , data_list[num_conf+MAX_PATH_ARG]
               , header, data_list[N_T_SHIFT]
               , data_list[num_conf+MAX_PATH_ARG], footer );
   
   string FILE_NAME( file_name );
   return FILE_NAME;
}
