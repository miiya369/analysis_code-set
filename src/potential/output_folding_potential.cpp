//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Definition for the output of folding-potential-type data
 * @author  Takaya Miyamoto
 * @since   Sat Jul 16 17:50:35 JST 2016
 */
//--------------------------------------------------------------------------

#include <potential/folding_potential.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for all data output ( All conf, with binary )
 */
//--------------------------------------------------------------------------
template <>
void analysis::output_data_all<F_POTENTIAL_GAUSS>(  CONFIG<F_POTENTIAL_GAUSS> &data
                                                  , const char* outfile_name )
{
   DEBUG_LOG
   
   ofstream ofs(outfile_name, ios::out | ios::binary);
   
   int    itmp1, itmp2;
   double dtmp;
//======================== miyamoto-format notation =======================//
//
//                        !! ALWAYS LITTLE ENDIAN !!
//
//      1) #.conf                   (int)
//      2) #.data/conf              (int)
//      3) bytes of data coordinate (int)
//
//      3) data coordinates         ((2)*(3) bytes)
//         -> for n = 0 to #.data
//               crd[n]
//
//      4) datas (double float(8 bytes) * (1)*(2))
//         -> for    i = 0 to #.conf
//               for n = 0 to #.data
//                  data[n+#.data*i]
//
//=========================================================================//
   itmp1 = data.Nconf();
   if (!analysis::machine_is_little_endian())
      analysis::endian_convert(&itmp1, 1);
   ofs.write((char*)&itmp1, sizeof(int));
   
   itmp1 = data(0).data_size();
   itmp2 = sizeof(double);
   if (!analysis::machine_is_little_endian()) {
      analysis::endian_convert(&itmp1, 1);
      analysis::endian_convert(&itmp2, 1);
   }
   ofs.write((char*)&itmp1, sizeof(int));
   ofs.write((char*)&itmp2, sizeof(int)); // write file header
   
   double div_r = data(0).max_range() / (double)data(0).data_size();
   for(int n=0; n<data(0).data_size(); n++) {
      dtmp = div_r * (n+1);
      if (!analysis::machine_is_little_endian())
         analysis::endian_convert(&dtmp, 1);
      ofs.write((char*)&dtmp, sizeof(double));
   }
   for(    int i=0; i<data.Nconf();  i++)
      for (int n=0; n<data(0).data_size(); n++) {
         dtmp = data(i)(n);
         if (!analysis::machine_is_little_endian())
            analysis::endian_convert(&dtmp, 1);
         ofs.write((char*)&dtmp, sizeof(double));
      }
   ofs.close();
}

//--------------------------------------------------------------------------
/**
 * @brief Function for data output with error
 */
//--------------------------------------------------------------------------
template <>
void analysis::output_data_err<F_POTENTIAL_GAUSS>(  CONFIG<F_POTENTIAL_GAUSS> &data
                                                  , const char* outfile_name
                                                  , bool is_jack_knife_data ) {
   DEBUG_LOG
   
   double err, mean, sqr_mean;
   double factor;
   if (is_jack_knife_data)
      factor = double(data.Nconf()-1);
   else
      factor = 1.0 / double(data.Nconf()-1);

   ofstream ofs(outfile_name, ios::out);
   ofs << "#   r, mean, err" << endl;
   
   double div_r = data(0).max_range() / (double)data(0).data_size();
   double r     = 0.0;
   for(int n=0; n<data(0).data_size(); n++) {
      mean     = 0.0;
      sqr_mean = 0.0;
      
      for( int i=0; i<data.Nconf(); i++) {
         mean     +=     data(i)(n);
         sqr_mean += pow(data(i)(n), 2.0);
      }
      mean     /= double(data.Nconf());
      sqr_mean /= double(data.Nconf());
      err = sqrt(factor * (sqr_mean - pow(mean, 2.0)));
      
      r += div_r;
      ofs << r << " " << mean << " " << err << endl;
   }
   ofs.close();
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output miyamoto-format fitting data
 */
//--------------------------------------------------------------------------
template <>
void analysis::output_data_fit<F_POTENTIAL_GAUSS>(  CONFIG<F_POTENTIAL_GAUSS> &data
                                                  , const char* outfile_name
                                                  , bool is_jack_knife_data ) {
   DEBUG_LOG
//======================== miyamoto-format notation =======================//
//
//                        !! ALWAYS LITTLE ENDIAN !!
//
//      1) fit type (int) -> 1 = corr type, 2 = pot type
//      2) #.conf   (int)
//      3) #.data   (int)
//
//      4) data     (double float)
//         -> for n = 0 to #.data {    |   for n = 0 to #.data {
//               err[n]                |      cood[n]
//               for i = 0 to #.conf   |       err[n]
//                  data[i+#.conf*n]   |      for i = 0 to #.conf
//            }                        |         data[i+#.conf*n]
//                                     |   }
//                 (at fit type = 1)   |        (at fit type = 2)
//
//=========================================================================//
   ofstream ofs(outfile_name, ios::out | ios::binary);
   double   mean, sqr_mean, err, tmp;
   int      fit_type, tmp_Nconf, tmp_Ndata;
   
   double factor;
   if (is_jack_knife_data)
      factor = double(data.Nconf()-1);
   else
      factor = 1.0 / double(data.Nconf()-1);
   
   fit_type  = 2;   // write file header
   tmp_Nconf = data.Nconf();
   tmp_Ndata = data(0).data_size();
   
   if (!analysis::machine_is_little_endian()) {
      analysis::endian_convert(&fit_type, 1);
      analysis::endian_convert(&tmp_Nconf,1);
      analysis::endian_convert(&tmp_Ndata,1);
   }
   ofs.write((char*)&fit_type,  sizeof(int));
   ofs.write((char*)&tmp_Nconf, sizeof(int));
   ofs.write((char*)&tmp_Ndata, sizeof(int));
   
   double div_r = data(0).max_range() / (double)data(0).data_size();
   double r     = 0.0;
   for(int n=0; n<data(0).data_size(); n++) {
      mean     = 0.0;
      sqr_mean = 0.0;
      
      for(int i=0; i<data.Nconf(); i++) {
         mean     +=     data(i)(n);
         sqr_mean += pow(data(i)(n), 2);
      }
      mean     /= double(data.Nconf());
      sqr_mean /= double(data.Nconf());
      err       = sqrt(factor * (sqr_mean - pow(mean,2)));
      
      r  += div_r;
      tmp = r;
      if (!analysis::machine_is_little_endian()) {
         analysis::endian_convert(&tmp, 1);
         analysis::endian_convert(&err, 1);
      }
      ofs.write((char*)&tmp, sizeof(double));   // write fit data
      ofs.write((char*)&err, sizeof(double));
      
      for(int i=0; i<data.Nconf(); i++) {
         tmp = data(i)(n);
         if (!analysis::machine_is_little_endian())
            analysis::endian_convert(&tmp, 1);
         ofs.write((char*)&tmp, sizeof(double));
      }
   }
   ofs.close();
}
