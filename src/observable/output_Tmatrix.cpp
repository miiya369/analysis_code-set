//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup observable
 * @brief   Functions for output phase shift data
 * @author  Takaya Miyamoto
 * @since   Fri Dec 11 22:25:13 JST 2015
 */
//--------------------------------------------------------------------------

#include <observable/Tmatrix.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for all data output ( All conf, with binary )
 */
//--------------------------------------------------------------------------
template <>
void analysis::output_data_all<T_MATRIX>(  CONFIG<T_MATRIX> &data
                                         , const char* outfile_name )
{
   DEBUG_LOG
   
   ofstream ofs(outfile_name, ios::out | ios::binary);
   
   int     itmp1, itmp2, itmp3;
   double  dtmp;
   cdouble ctmp;
//======================== miyamoto-format notation =======================//
//
//                        !! ALWAYS LITTLE ENDIAN !!
//
//      1) #.conf                   (int)
//      2) #.data/conf              (int)
//      3) #.channel                (int)
//      4) bytes of data coordinate (int)
//
//      5) data coordinates         ((2)*(4) bytes)
//         -> for n = 0 to #.data
//               crd[n]
//
//      6) datas (complex double float(16 bytes) * (1)*(2)*(3)*(3))
//         -> for       i = 0 to #.conf
//               for    n = 0 to #.data
//                  for c = 0 to (#.channel)^2
//                     data[c + (#.channel)^2 * (n + #.data * i)]
//
//=========================================================================//
   itmp1 = data.Nconf();
   if (!analysis::machine_is_little_endian())
      analysis::endian_convert(&itmp1, 1);
   ofs.write((char*)&itmp1, sizeof(int));
   
   itmp1 = data(0).data_size();
   itmp2 = data(0).Nch();
   itmp3 = sizeof(double);
   if (!analysis::machine_is_little_endian()) {
      analysis::endian_convert(&itmp1, 1);
      analysis::endian_convert(&itmp2, 1);
      analysis::endian_convert(&itmp3, 1);
   }
   ofs.write((char*)&itmp1, sizeof(int));
   ofs.write((char*)&itmp2, sizeof(int)); // write file header
   ofs.write((char*)&itmp3, sizeof(int));
   
   for (int n=0; n<data(0).data_size(); n++) {
      dtmp = data(0).E(n);
      if (!analysis::machine_is_little_endian())
         analysis::endian_convert(&dtmp, 1);
      ofs.write((char*)&dtmp, sizeof(double));
   }
   for       (int i=0; i<data.Nconf();                i++)
      for    (int n=0; n<data(0).data_size();         n++)
         for (int c=0; c<data(0).Nch()*data(0).Nch(); c++) {
            ctmp = data(i)(n)(c);
            if (!analysis::machine_is_little_endian())
               analysis::endian_convert(&ctmp, 1);
            ofs.write((char*)&ctmp, sizeof(cdouble));
      }
   ofs.close();
}
