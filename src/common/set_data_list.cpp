//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Function for data list setting, data_list is used on set_path
 * @author  Takaya Miyamoto
 * @since   Wed Jul 22 04:02:42 JST 2015
 */
//--------------------------------------------------------------------------

#include <common/analysis.h>

int analysis::set_data_list( const char* path_conf_file_list ) {
   
   int i=0;
   
   ifstream ifs(path_conf_file_list, ios::in);
   if (!ifs) error(2, path_conf_file_list);
   
   while (ifs.getline(        analysis::data_list[MAX_PATH_ARG+i]
                      ,sizeof(analysis::data_list[MAX_PATH_ARG+i]))) i++;
   ifs.close();
   
   return i;
}

void analysis::set_data_list(  const char* main_path
                             , const char* N_Tshift
                             , const char* N_Xshift
                             , const char* N_Yshift
                             , const char* N_Zshift
                             , const char* snk_rela
                             , const char* src_rela ) {
   
   snprintf(  analysis::data_list[MAIN_PATH]
            ,sizeof(analysis::data_list[MAIN_PATH])
            , "%s", main_path );
   snprintf(  analysis::data_list[N_T_SHIFT]
            ,sizeof(analysis::data_list[N_T_SHIFT])
            , "%s", N_Tshift );
   snprintf(  analysis::data_list[N_X_SHIFT]
            ,sizeof(analysis::data_list[N_X_SHIFT])
            , "%s", N_Xshift );
   snprintf(  analysis::data_list[N_Y_SHIFT]
            ,sizeof(analysis::data_list[N_Y_SHIFT])
            , "%s", N_Yshift );
   snprintf(  analysis::data_list[N_Z_SHIFT]
            ,sizeof(analysis::data_list[N_Z_SHIFT])
            , "%s", N_Zshift );
   snprintf(  analysis::data_list[SNK_RELA]
            ,sizeof(analysis::data_list[SNK_RELA])
            , "%s", snk_rela );
   snprintf(  analysis::data_list[SRC_RELA]
            ,sizeof(analysis::data_list[SRC_RELA])
            , "%s", src_rela );
}

int analysis::set_data_list(  const char* main_path
                             , const char* N_Tshift
                             , const char* N_Xshift
                             , const char* N_Yshift
                             , const char* N_Zshift
                             , const char* snk_rela
                             , const char* src_rela
                             , const char* path_conf_file_list ) {
   
   snprintf(  analysis::data_list[MAIN_PATH]
            ,sizeof(analysis::data_list[MAIN_PATH])
            , "%s", main_path );
   snprintf(  analysis::data_list[N_T_SHIFT]
            ,sizeof(analysis::data_list[N_T_SHIFT])
            , "%s", N_Tshift );
   snprintf(  analysis::data_list[N_X_SHIFT]
            ,sizeof(analysis::data_list[N_X_SHIFT])
            , "%s", N_Xshift );
   snprintf(  analysis::data_list[N_Y_SHIFT]
            ,sizeof(analysis::data_list[N_Y_SHIFT])
            , "%s", N_Yshift );
   snprintf(  analysis::data_list[N_Z_SHIFT]
            ,sizeof(analysis::data_list[N_Z_SHIFT])
            , "%s", N_Zshift );
   snprintf(  analysis::data_list[SNK_RELA]
            ,sizeof(analysis::data_list[SNK_RELA])
            , "%s", snk_rela );
   snprintf(  analysis::data_list[SRC_RELA]
            ,sizeof(analysis::data_list[SRC_RELA])
            , "%s", src_rela );
   
   int i=0;
   
   ifstream ifs(path_conf_file_list, ios::in);
   if (!ifs) error(2, path_conf_file_list);
   
   while (ifs.getline(        analysis::data_list[MAX_PATH_ARG+i]
                      ,sizeof(analysis::data_list[MAX_PATH_ARG+i]))) i++;
   ifs.close();
   
   return i;
}
