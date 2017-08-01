//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup observable
 * @brief   Functions for output phase shift data
 * @author  Takaya Miyamoto
 * @since   Fri Dec 11 22:25:13 JST 2015
 */
//--------------------------------------------------------------------------

#include <observable/phase_shift.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for all data output ( All conf, with binary )
 */
//--------------------------------------------------------------------------
template <>
void analysis::output_data_all<PHASE_SHIFT>(  CONFIG<PHASE_SHIFT> &data
                                            , const char* outfile_name )
{
   DEBUG_LOG
   
   ofstream ofs(outfile_name, ios::out | ios::binary);
   
   int     itmp1, itmp2;
   double  dtmp;
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
   
   for (int n=0; n<data(0).data_size(); n++) {
      dtmp = data(0).E(n);
      if (!analysis::machine_is_little_endian())
         analysis::endian_convert(&dtmp, 1);
      ofs.write((char*)&dtmp, sizeof(double));
   }
   for (int i=0; i<data.Nconf(); i++)
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
void analysis::output_data_err<PHASE_SHIFT>(  CONFIG<PHASE_SHIFT> &data
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
   ofs << "#   E, mean, err" << endl;
   
   for (int iE=0; iE<data(0).data_size(); iE++) {
      mean     = 0.0;
      sqr_mean = 0.0;
      
      for (int i=0; i<data.Nconf(); i++) {
         mean     +=     data(i)(iE);
         sqr_mean += pow(data(i)(iE), 2.0);
      }
      mean     /= double(data.Nconf());
      sqr_mean /= double(data.Nconf());
      err       = sqrt(factor * (sqr_mean - pow(mean, 2.0)));
      
      ofs << data(0).E(iE) << " " << mean << " " << err << endl;
   }
   ofs.close();
}
