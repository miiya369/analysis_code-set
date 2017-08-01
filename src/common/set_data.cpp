//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Function for data list setting, data_list is used on set_path
 * @author  Takaya Miyamoto
 * @since   Fri Nov 13 00:45:20 JST 2015
 */
//--------------------------------------------------------------------------

#include <common/analysis.h>

int analysis::xSIZE;
int analysis::ySIZE;
int analysis::zSIZE;
int analysis::tSIZE;
int analysis::Nconf;

char analysis::data_list[MAX_N_DATA][MAX_LEN_PATH];

void analysis::set_data_list(int SET_INDEX, const char* format, ...) {
   char tmp_c[MAX_LEN_PATH];
   va_list  args;
   va_start(args,  format);
   vsprintf(tmp_c, format, args);
   
   snprintf(        analysis::data_list[SET_INDEX]
            ,sizeof(analysis::data_list[SET_INDEX]), tmp_c );
   
   va_end(args);
}

int analysis::set_data_list(const char* path_conf_file_list) {
   int i=0;
   
   ifstream ifs(path_conf_file_list, ios::in);
   if (!ifs) ERROR_FOPEN(path_conf_file_list);
   
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
                             , const char* src_rela )
{
   set_data_list(MAIN_PATH, "%s", main_path);
   set_data_list(N_T_SHIFT, "%s", N_Tshift);
   set_data_list(N_X_SHIFT, "%s", N_Xshift);
   set_data_list(N_Y_SHIFT, "%s", N_Yshift);
   set_data_list(N_Z_SHIFT, "%s", N_Zshift);
   set_data_list(SNK_RELA,  "%s", snk_rela);
   set_data_list(SRC_RELA,  "%s", src_rela);
}

int analysis::set_data_list(  const char* main_path
                            , const char* N_Tshift
                            , const char* N_Xshift
                            , const char* N_Yshift
                            , const char* N_Zshift
                            , const char* snk_rela
                            , const char* src_rela
                            , const char* path_conf_file_list )
{
   set_data_list(MAIN_PATH, "%s", main_path);
   set_data_list(N_T_SHIFT, "%s", N_Tshift);
   set_data_list(N_X_SHIFT, "%s", N_Xshift);
   set_data_list(N_Y_SHIFT, "%s", N_Yshift);
   set_data_list(N_Z_SHIFT, "%s", N_Zshift);
   set_data_list(SNK_RELA,  "%s", snk_rela);
   set_data_list(SRC_RELA,  "%s", src_rela);
   
   int i=0;
   
   ifstream ifs(path_conf_file_list, ios::in);
   if (!ifs) ERROR_FOPEN(path_conf_file_list);
   
   while (ifs.getline(        analysis::data_list[MAX_PATH_ARG+i]
                      ,sizeof(analysis::data_list[MAX_PATH_ARG+i]))) i++;
   ifs.close();
   
   return i;
}

void analysis::set_data(int xxx, int yyy, int zzz, int ttt, int nnn)
{
   analysis::xSIZE = xxx;
   analysis::ySIZE = yyy;
   analysis::zSIZE = zzz;
   analysis::tSIZE = ttt;
   analysis::Nconf = nnn;
}
