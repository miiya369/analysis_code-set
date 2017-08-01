//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Function for output parameter list file
 * @author  Takaya Miyamoto
 * @since   Fri Nov 13 01:41:50 JST 2015
 */
//--------------------------------------------------------------------------

#include <fitting/fitting.h>

void fitting::output_param( CONFIG<FIT> &data, const char* outFILE_name ) {
   DEBUG_LOG
   
   ofstream ofs(outFILE_name, ios::out | ios::binary);
   
   int tmp_Nconf    = data.Nconf();
   int tmp_func_num = data(0).info_func_num();
   
   if (!analysis::machine_is_little_endian()) {
      analysis::endian_convert(&tmp_Nconf,    1);
      analysis::endian_convert(&tmp_func_num, 1);
   }
   ofs.write((char*)&tmp_Nconf,    sizeof(int));   // write file header
   ofs.write((char*)&tmp_func_num, sizeof(int));
   
   double tmp;
   for (   int loop=0; loop<data(0).data_size(); loop++)
      for (int conf=0; conf<data.Nconf();        conf++) {
         
         tmp = data(conf)(loop);
         if (!analysis::machine_is_little_endian())
            analysis::endian_convert(&tmp, 1);
         ofs.write((char*)&tmp, sizeof(double));
      }
   ofs.close();
}
