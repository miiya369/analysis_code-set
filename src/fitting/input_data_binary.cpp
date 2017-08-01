//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Function for input the ( miyamoto-format ) fit data
 * @author  Takaya Miyamoto
 * @since   Thu Sep  3 18:14:14 JST 2015
 */
//--------------------------------------------------------------------------

#include <fitting/fitting.h>

//--------------------------------------------------------------------------
/**
 * @brief The function for input binary-fitting data (for header)
 */
//--------------------------------------------------------------------------
int fitting::input_data_binary(const char *infile_name,int &n_conf,int &n_data) {
   DEBUG_LOG
   
   int fit_type, n_conf_tmp, n_data_tmp;
   
   ifstream ifs(infile_name, ios::in | ios::binary);
   if (!ifs) ERROR_FOPEN(infile_name);

   ifs.read((char*)&fit_type,   sizeof(int));
   ifs.read((char*)&n_conf_tmp, sizeof(int));   // read file header
   ifs.read((char*)&n_data_tmp, sizeof(int));
   
   if (!analysis::machine_is_little_endian()) {
      analysis::endian_convert(&fit_type,   1);
      analysis::endian_convert(&n_conf_tmp, 1);
      analysis::endian_convert(&n_data_tmp, 1);
   }
   if(fit_type == 1 || fit_type == 2) {
      printf(" @ fit type         = %d\n", fit_type);
      printf(" @ #.confs          = %d\n", n_conf_tmp);
      printf(" @ #.data size      = %d\n", n_data_tmp);
      
   } else {
      printf(" @ fit type         = %d\n", fit_type);
      printf(" @ #.confs          = %d\n", n_conf_tmp);
      printf(" @ #.data size      = %d\n", n_data_tmp);
      ifs.close();
      ERROR_COMMENTS("This file is not miyamoto-format fit data");
   }
   ifs.close();
   
   n_conf = n_conf_tmp;
   n_data = n_data_tmp;
   
   return fit_type;
}

//--------------------------------------------------------------------------
/**
 * @brief The function for input binary-fitting data
 */
//--------------------------------------------------------------------------
int fitting::input_data_binary(  const char *infile_name
                                , double *cood, double *data, double *err ) {
   DEBUG_LOG
   
   int fit_type, n_conf_tmp, n_data_tmp;
   
   ifstream ifs(infile_name, ios::in | ios::binary);
   if (!ifs) ERROR_FOPEN(infile_name);
//======================== miyamoto-format notation =======================//
//
//                        !! ALWAYS LITTLE ENDIAN !!
//
//      1) fit type (int) -> 1 = corr type, 2 = pot type
//      2) #.conf   (int)
//      3) #.data   (int)
//
//      4) data     (double float)
//         -> for i = 0 to #.data {    |   for i = 0 to #.data {
//               err[i]                |      cood[i]
//               for n = 0 to #.conf   |       err[i]
//                  data[n+#.conf*i]   |      for n = 0 to #.conf
//            }                        |         data[n+#.conf*i]
//                                     |   }
//                 (at fit type = 1)   |        (at fit type = 2)
//
//=========================================================================//
   ifs.read((char*)&fit_type,   sizeof(int));
   ifs.read((char*)&n_conf_tmp, sizeof(int));   // read file header
   ifs.read((char*)&n_data_tmp, sizeof(int));
   
   if (!analysis::machine_is_little_endian()) {
      analysis::endian_convert(&fit_type,   1);
      analysis::endian_convert(&n_conf_tmp, 1);
      analysis::endian_convert(&n_data_tmp, 1);
   }
//=========================== read corr-type data =========================//
   if(fit_type == 1) {
      
      for (int t=0; t<n_data_tmp; t++) {
         cood[t] = t;
         ifs.read((char*)&err[t], sizeof(double));
         for (int i=0; i<n_conf_tmp; i++)
            ifs.read((char*)&data[i+n_conf_tmp*t], sizeof(double));
      }
      if (!analysis::machine_is_little_endian()) {
         analysis::endian_convert(data, n_data_tmp * n_conf_tmp);
         analysis::endian_convert(err,  n_data_tmp);
      }
//========================= read potential-type data ======================//
   }else if(fit_type == 2) {
      
      for (int r=0; r<n_data_tmp; r++) {
         ifs.read((char*)&cood[r], sizeof(double));
         ifs.read((char*)&err[r] , sizeof(double));
         for (int i=0; i<n_conf_tmp; i++)
            ifs.read((char*)&data[i+n_conf_tmp*r], sizeof(double));
      }
      if(!analysis::machine_is_little_endian()){
         analysis::endian_convert(cood, n_data_tmp);
         analysis::endian_convert(data, n_data_tmp * n_conf_tmp);
         analysis::endian_convert(err,  n_data_tmp);
      }
   }
//========================================================================//
   else printf("This file is not miyamoto-format fit data\n");
   
   ifs.close();
   
   return fit_type;
}
