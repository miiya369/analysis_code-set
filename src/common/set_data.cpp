//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Function for data list setting, data_list is used on set_path
 * @author  Takaya Miyamoto
 * @since   Wed Sep 16 22:06:04 JST 2015
 */
//--------------------------------------------------------------------------

#include <common/analysis.h>

int analysis::xSIZE;
int analysis::ySIZE;
int analysis::zSIZE;
int analysis::tSIZE;
int analysis::Nconf;

char analysis::data_list[MAX_N_DATA][MAX_LEN_PATH];

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

void analysis::set_data_list( int SET_INDEX, const char* format, ... ) {
   
   char tmp_c[MAX_LEN_PATH];
   va_list args;
   va_start(args,  format);
   vsprintf(tmp_c, format, args);
   
   snprintf(        analysis::data_list[SET_INDEX]
            ,sizeof(analysis::data_list[SET_INDEX]), tmp_c );
   
   va_end(args);
}

void analysis::set_data( int xxx, int yyy, int zzz, int ttt, int nnn ) {
   
   analysis::xSIZE = xxx;
   analysis::ySIZE = yyy;
   analysis::zSIZE = zzz;
   analysis::tSIZE = ttt;
   analysis::Nconf = nnn;
}
