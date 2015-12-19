//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Common header file for definition of data output
 * @author  Takaya Miyamoto
 * @since   Thu Sep  3 01:34:55 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef OUTPUT_DATA_H
#define OUTPUT_DATA_H

#include <common/analysis.h>

#define idx(x,y,z) (x + analysis::xSIZE *( y + analysis::ySIZE * z))

//--------------------------------------------------------------------------
/**
 * @brief Function for data output ( one conf. with binary )
 */
//--------------------------------------------------------------------------
template <class X> void analysis::output_data_all(  X &data
                                                  , const char* outfile_name ) {
   
   string class_name = "________________________________";
   string func_name  = "output_data_all_______";
   analysis::route(class_name, func_name, 1);

   ofstream ofs( outfile_name, ios::out | ios::binary );
   
   size_t  data_size = data.data_size();
   cdouble tmp;
   for (size_t n=0; n<data_size; n++) {
      tmp = data(n);
      if (!analysis::machine_is_little_endian())
         analysis::endian_convert(&tmp, 1);
      ofs.write((char*)&tmp, sizeof(cdouble));
   }
   ofs.close();
      
   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Function for data output with error
 * @brief ( with 1/48 data size reduction for POTENTIAL-TYPE datas )
 */
//--------------------------------------------------------------------------
template <class X> void analysis::output_data_err(  CONFIG<X> &data
                                                  , const char* outfile_name ) {
    
   string class_name = "________________________________";
   string func_name = "output_data_err_______";
   analysis::route(class_name, func_name, 1);
   
   cdouble err, mean, sqr_mean;

   ofstream ofs( outfile_name, ios::out );
   ofs << "#   t, mean.real, err.real, mean.imag, err.imag" << endl;
   
   if (data(0).info_class()==CLASS_CORRELATOR) {   // for CORRELATOR-TYPE datas
      
      for(int t=0; t<analysis::tSIZE; t++) {
         
         mean     = COMP_ZERO;
         sqr_mean = COMP_ZERO;
         
         for( int i=0; i<analysis::Nconf; i++) {
            mean     +=          data(i)(t);
            sqr_mean += cmp_sqr( data(i)(t) );
         }
         mean     /= double(analysis::Nconf);
         sqr_mean /= double(analysis::Nconf);
         err = cmp_sqrt(double(analysis::Nconf-1)*(sqr_mean - cmp_sqr(mean)));
         
         ofs << t << " " << mean.real() << " " << err.real()
             << " " << mean.imag() << " " << err.imag() << endl;
      }
      
   } else {   // for POTENTIAL-TYPE datas
      
      double R;
      for (      int z=0; z<analysis::zSIZE; z++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int x=0; x<analysis::xSIZE; x++) {
               if (x>analysis::xSIZE/2 || y>x || z>y) continue;
            
               mean     = COMP_ZERO;
               sqr_mean = COMP_ZERO;
            
               for (int i=0; i<analysis::Nconf; i++) {
                  mean     +=          data(i)(idx(x,y,z));
                  sqr_mean += cmp_sqr( data(i)(idx(x,y,z)) );
               }
               mean     /= double(analysis::Nconf);
               sqr_mean /= double(analysis::Nconf);
               err       = cmp_sqrt(  double(analysis::Nconf-1)
                                    * ( sqr_mean - cmp_sqr( mean )) );
               
               R = sqrt( x*x + y*y + z*z );
                    
               ofs << x << " " << y << " " << z << " " << R
                   << " " << mean.real() << " " << err.real()
                   << " " << mean.imag() << " " << err.imag() << endl;
            }
   }
   ofs.close();
   
   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output miyamoto-format fitting data
 */
//--------------------------------------------------------------------------
template <class X> void analysis::output_data_fit(  CONFIG<X> &data
                                                  , const char* outfile_name ) {
   
   string class_name = "________________________________";
   string func_name = "output_data_fit_______";
   analysis::route(class_name, func_name, 1);
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
   ofstream ofs( outfile_name, ios::out | ios::binary );
   double   mean, sqr_mean, err, tmp;
   int      fit_type, tmp_Nconf, tmp_Ndata;
   
   if (data(0).info_class()==CLASS_CORRELATOR) {   // for CORRELATOR-TYPE datas
      
      fit_type  = 1;   // write file header
      tmp_Nconf = analysis::Nconf;
      tmp_Ndata = analysis::tSIZE;
      
      if (!analysis::machine_is_little_endian()) {
         analysis::endian_convert(&fit_type, 1);
         analysis::endian_convert(&tmp_Nconf,1);
         analysis::endian_convert(&tmp_Ndata,1);
      }
      ofs.write((char*)&fit_type,  sizeof(int));
      ofs.write((char*)&tmp_Nconf, sizeof(int));
      ofs.write((char*)&tmp_Ndata, sizeof(int));
      
      for(int t=0; t<analysis::tSIZE; t++) {
         mean     = 0.0;
         sqr_mean = 0.0;
         
         for(int i=0; i<analysis::Nconf; i++) {
            mean     +=     data(i)(t).real();
            sqr_mean += pow(data(i)(t).real(),2);
         }
         mean     /= double(analysis::Nconf);
         sqr_mean /= double(analysis::Nconf);
         err       = sqrt(double(analysis::Nconf-1) * (sqr_mean - pow(mean,2)));
         
         if (!analysis::machine_is_little_endian())
            analysis::endian_convert(&err, 1);
         ofs.write((char*)&err, sizeof(double));   // write fit data
         
         for( int i=0; i<analysis::Nconf; i++) {
            tmp = data(i)(t).real();
            if (!analysis::machine_is_little_endian())
               analysis::endian_convert(&tmp,1);
            ofs.write((char*)&tmp, sizeof(double));
         }
      }
      
   } else {   // for POTENTIAL-TYPE datas
      
      fit_type  = 2;   // write file header
      tmp_Nconf = analysis::Nconf;
      int Ndata = analysis::reduced_Ndata();
      tmp_Ndata = Ndata;
      
      if (!analysis::machine_is_little_endian()) {
         analysis::endian_convert(&fit_type, 1);
         analysis::endian_convert(&tmp_Nconf,1);
         analysis::endian_convert(&tmp_Ndata,1);
      }
      ofs.write((char*)&fit_type,  sizeof(int));
      ofs.write((char*)&tmp_Nconf, sizeof(int));
      ofs.write((char*)&tmp_Ndata, sizeof(int));
 
      double R;
      for (      int z=0; z<analysis::zSIZE; z++)
         for (   int y=0; y<analysis::ySIZE; y++)
            for (int x=0; x<analysis::xSIZE; x++) {
               if (x>analysis::xSIZE/2 || y>x || z>y) continue;
               
               mean     = 0.0;
               sqr_mean = 0.0;
               
               for (int i=0; i<analysis::Nconf; i++) {
                  mean     +=      data(i)(idx(x,y,z)).real();
                  sqr_mean += pow( data(i)(idx(x,y,z)).real(), 2 );
               }
               mean     /= double(analysis::Nconf);
               sqr_mean /= double(analysis::Nconf);
               err       = sqrt(double(analysis::Nconf-1)*(sqr_mean-pow(mean,2)));
 
               R = sqrt( x*x + y*y + z*z );
               
               if (!analysis::machine_is_little_endian()) {
                  analysis::endian_convert(&R,  1);
                  analysis::endian_convert(&err,1);
               }
               ofs.write((char*)&R  , sizeof(double));
               ofs.write((char*)&err, sizeof(double));
               
               for (int i=0; i<analysis::Nconf; i++){
                  tmp = data(i)(idx(x,y,z)).real();
                  if (!analysis::machine_is_little_endian())
                     analysis::endian_convert(&tmp,1);
                  ofs.write((char*)&tmp, sizeof(double));
               }
            }
   }
   ofs.close();
   
   analysis::route(class_name, func_name, 0);
}

#undef idx

#endif
