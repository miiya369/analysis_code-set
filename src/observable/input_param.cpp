//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup observable
 * @brief   Function for input the ( miyamoto-format ) fit parameter data
 * @author  Takaya Miyamoto
 * @since   Fri Sep  4 02:23:09 JST 2015
 */
//--------------------------------------------------------------------------

#include <observable/observable_base.h>
#include <fitting/function_type_const.h>

//--------------------------------------------------------------------------
/**
 * @brief The function for input binary-parameter data (for header)
 */
//--------------------------------------------------------------------------
void observable::input_param(  const char *infile_name
                             , int &n_conf, int &n_param, int &n_func ) {
   
   string class_name = "________________________________";
   string func_name = "input_param_header____";
   analysis::route(class_name, func_name, 1);
   
   ifstream ifs(infile_name, ios::in | ios::binary);
   if(!ifs) analysis::error(2, infile_name);
   
   int          tmp_Nconf;
   int          func_type_number;
   FIT_FUNCTION func_type;
   
   ifs.read( (char*)&tmp_Nconf,        sizeof(int) );   // read file header
   ifs.read( (char*)&func_type_number, sizeof(int) );
   
   if (!analysis::machine_is_little_endian()) {
      analysis::endian_convert(&tmp_Nconf,    1);
      analysis::endian_convert(&func_type_number, 1);
   }
   char tmp_str[10];
   snprintf(tmp_str, sizeof(tmp_str), "%d", func_type_number);
   func_type.set(tmp_str);
   
   printf(" @ #.confs       = %d\n", tmp_Nconf);
   printf(" @ function type = %s\n", func_type.name.c_str());
   ifs.close();
   
   n_conf  = tmp_Nconf;
   n_param = func_type.Nparam;
   n_func  = func_type_number;
   
   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief The function for input binary-parameter data (for body)
 */
//--------------------------------------------------------------------------
void observable::input_param( const char *infile_name, double *param ) {
   
   string class_name = "________________________________";
   string func_name = "input_param_body______";
   analysis::route(class_name, func_name, 1);
   
   ifstream ifs(infile_name, ios::in | ios::binary);
   if(!ifs) analysis::error(2, infile_name);
   
   int          tmp_Nconf;
   int          func_type_number;
   FIT_FUNCTION func_type;
   
   ifs.read( (char*)&tmp_Nconf,        sizeof(int) );   // read file header
   ifs.read( (char*)&func_type_number, sizeof(int) );
   
   if (!analysis::machine_is_little_endian()) {
      analysis::endian_convert(&tmp_Nconf,    1);
      analysis::endian_convert(&func_type_number, 1);
   }
   char tmp_str[10];
   snprintf(tmp_str, sizeof(tmp_str), "%d", func_type_number);
   func_type.set(tmp_str);
   
   ifs.read((char*)&param[0], sizeof(double) * tmp_Nconf * func_type.Nparam);
   
   if (!analysis::machine_is_little_endian())
      analysis::endian_convert(param, tmp_Nconf * func_type.Nparam);
   
   ifs.close();
   
   analysis::route(class_name, func_name, 0);
}
