//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Function for output parameter list file
 * @author  Takaya Miyamoto
 * @since   Thu Jul 23 14:24:38 JST 2015
 */
//--------------------------------------------------------------------------

#include <fitting/fitting.h>

void FIT::output_param( const char* outFILE_name ) {
   
   func_name = "input_data____________";
   analysis::route(class_name, func_name, 1);
   
   if (!fitting_flg) {
      analysis::error(1,"Data fitting has not done yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   ofstream ofs(outFILE_name, ios::out | ios::binary);
   
   int tmp_Nconf    = analysis::Nconf;
   int tmp_func_num = func_type.number;
   
   if (!analysis::machine_is_little_endian()) {
      analysis::endian_convert(&tmp_Nconf,    1);
      analysis::endian_convert(&tmp_func_num, 1);
   }
   ofs.write( (char*)&tmp_Nconf,    sizeof(int) );   // write file header
   ofs.write( (char*)&tmp_func_num, sizeof(int) );
   
   double tmp;
   for (   int loop=0; loop<func_type.Nparam; loop++)
      for (int conf=0; conf<analysis::Nconf;  conf++) {
         
         tmp = param_out[idx(conf,loop)];
         if (!analysis::machine_is_little_endian())
            analysis::endian_convert(&tmp, 1);
         ofs.write((char*)&tmp, sizeof(double));
      }
   ofs.close();
   
   analysis::route(class_name, func_name, 0);
}
