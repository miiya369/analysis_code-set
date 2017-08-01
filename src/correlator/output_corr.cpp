//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Correlator
 * @brief   Definition for the output of correlator-type data
 * @author  Takaya Miyamoto
 * @since   Sat Jul 16 17:50:35 JST 2016
 */
//--------------------------------------------------------------------------

#include <correlator/correlator.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for all data output ( All conf, with binary )
 */
//--------------------------------------------------------------------------
template <>
void analysis::output_data_all<CORRELATOR>(  CONFIG<CORRELATOR> &data
                                           , const char* outfile_name )
{
   DEBUG_LOG
   
   ofstream ofs(outfile_name, ios::out | ios::binary);
   
   int     itmp1, itmp2;
   cdouble ctmp;
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
//      4) datas (complex double float(8+8 bytes) * (1)*(2))
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
   itmp2 = sizeof(int);
   if (!analysis::machine_is_little_endian()) {
      analysis::endian_convert(&itmp1, 1);
      analysis::endian_convert(&itmp2, 1);
   }
   ofs.write((char*)&itmp1, sizeof(int));
   ofs.write((char*)&itmp2, sizeof(int));
   
   for(int t=0; t<data(0).data_size(); t++) {
      itmp1 = t;
      if (!analysis::machine_is_little_endian())
         analysis::endian_convert(&itmp1, 1);
      ofs.write((char*)&itmp1, sizeof(int));
   }
   for(    int i=0; i<data.Nconf();  i++)
      for (int n=0; n<data(0).data_size(); n++) {
         ctmp = data(i)(n);
         if (!analysis::machine_is_little_endian())
            analysis::endian_convert(&ctmp, 1);
         ofs.write((char*)&ctmp, sizeof(cdouble));
      }
   ofs.close();
}

//--------------------------------------------------------------------------
/**
 * @brief Function for data output with error
 */
//--------------------------------------------------------------------------
template <>
void analysis::output_data_err<CORRELATOR>(  CONFIG<CORRELATOR> &data
                                           , const char* outfile_name
                                           , bool is_jack_knife_data ) {
   DEBUG_LOG
   
   cdouble err, mean, sqr_mean;
   double  factor;
   if (is_jack_knife_data)
      factor = double(data.Nconf()-1);
   else
      factor = 1.0 / double(data.Nconf()-1);

   ofstream ofs(outfile_name, ios::out);
   ofs << "#   t, mean.real, err.real, mean.imag, err.imag" << endl;
   
   for(int t=0; t<data(0).data_size(); t++) {
      mean     = COMP_ZERO;
      sqr_mean = COMP_ZERO;
      
      for( int i=0; i<data.Nconf(); i++) {
         mean     +=         data(i)(t);
         sqr_mean += cmp_sqr(data(i)(t));
      }
      mean     /= double(data.Nconf());
      sqr_mean /= double(data.Nconf());
      err = cmp_sqrt(factor * (sqr_mean - cmp_sqr(mean)));
      
      ofs << t << " " << mean.real() << " " << err.real()
      << " " << mean.imag() << " " << err.imag() << endl;
   }
   ofs.close();
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output miyamoto-format fitting data
 */
//--------------------------------------------------------------------------
template <>
void analysis::output_data_fit<CORRELATOR>(  CONFIG<CORRELATOR> &data
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
//         -> for i = 0 to #.data {    |   for i = 0 to #.data {
//               err[i]                |      cood[i]
//               for n = 0 to #.conf   |       err[i]
//                  data[n+#.conf*i]   |      for n = 0 to #.conf
//            }                        |         data[n+#.conf*i]
//                                     |   }
//                 (at fit type = 1)   |        (at fit type = 2)
//
//=========================================================================//
   ofstream ofs(outfile_name, ios::out | ios::binary);
   double   mean, sqr_mean, err, tmp;
   int      fit_type, tmp_Nconf, tmp_Ndata;
   
   double  factor;
   if (is_jack_knife_data)
      factor = double(data.Nconf()-1);
   else
      factor = 1.0 / double(data.Nconf()-1);
   
   fit_type  = 1;   // write file header
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
   
   for(int t=0; t<data(0).data_size(); t++) {
      mean     = 0.0;
      sqr_mean = 0.0;
      
      for(int i=0; i<data.Nconf(); i++) {
         mean     +=     data(i)(t).real();
         sqr_mean += pow(data(i)(t).real(),2);
      }
      mean     /= double(data.Nconf());
      sqr_mean /= double(data.Nconf());
      err       = sqrt(factor * (sqr_mean - pow(mean,2)));
      
      if (!analysis::machine_is_little_endian())
         analysis::endian_convert(&err, 1);
      ofs.write((char*)&err, sizeof(double));   // write fit data
      
      for(int i=0; i<data.Nconf(); i++) {
         tmp = data(i)(t).real();
         if (!analysis::machine_is_little_endian())
            analysis::endian_convert(&tmp,1);
         ofs.write((char*)&tmp, sizeof(double));
      }
   }
   ofs.close();
}
