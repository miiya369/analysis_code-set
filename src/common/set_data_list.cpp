//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Function for setting gauge configuration data
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <analysis.h>

int set_data_list( const char* path_conf_file_list,
                         char  list[MAX_N_DATA][MAX_LEN_PATH] ){
    
    int i=0;
    
    ifstream ifs( path_conf_file_list, ios::in );
    if(!ifs) error(2, path_conf_file_list);

    while(ifs.getline(list[MAX_PATH_ARG+i],sizeof(list[MAX_PATH_ARG+i]))) i++;
    
    ifs.close();
    
    return i;
}
