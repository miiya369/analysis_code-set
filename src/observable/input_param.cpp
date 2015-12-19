//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup phase shift
 * @brief   Function for input the ( miyamoto-format ) fit parameter data
 * @author  Takaya Miyamoto
 * @since   Thu Jul 23 18:17:57 JST 2015
 */
//--------------------------------------------------------------------------

#include <observable/phase_shift.h>

void PHASE_SHIFT::input_param( const char *infile_name ) {
   
   func_name = "input_param___________";
   analysis::route(class_name, func_name, 1);
   
   ifstream ifs(infile_name, ios::in | ios::binary);
   if(!ifs) analysis::error(2, infile_name);
   
   int tmp_Nconf;
   int func_type_number;
   ifs.read( (char*)&tmp_Nconf,        sizeof(int) );   // read file header
   ifs.read( (char*)&func_type_number, sizeof(int) );
   
   if (!analysis::machine_is_little_endian()) {
      analysis::endian_convert(&tmp_Nconf,    1);
      analysis::endian_convert(&func_type_number, 1);
   }
   analysis::Nconf = tmp_Nconf;
   char tmp_str[10];
   snprintf(tmp_str, sizeof(tmp_str), "%d", func_type_number);
   func_type.set(tmp_str);
   
   if (param != NULL) delete [] param;
   param = new double[analysis::Nconf * func_type.Nparam];
   
   ifs.read((char*)&param[0], sizeof(double)*analysis::Nconf*func_type.Nparam);

   if (!analysis::machine_is_little_endian())
      analysis::endian_convert(param, analysis::Nconf * func_type.Nparam);
   
   printf(" @ #.confs       = %d\n", analysis::Nconf);
   printf(" @ function type = %s\n", func_type.name.c_str());
   ifs.close();
   
   analysis::route(class_name, func_name, 0);
}
