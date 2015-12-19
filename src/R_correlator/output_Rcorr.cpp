//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup R-correlator
 * @brief   Functions for output R-correlator data
 * @author  Takaya Miyamoto
 * @since   Wed Jul 29 02:11:22 JST 2015
 */
//--------------------------------------------------------------------------

#include <R_correlator/R_correlator.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for output R-correlators data every gauge configurations
 */
//--------------------------------------------------------------------------
void R_CORRELATOR::output_Rcorr_all( const char* out_file_PATH ) {
    
   func_name = "output_Rcorr_all______";
   analysis::route(class_name, func_name, 1);

   if (Rcorr == NULL) {
      analysis::error(1,"R-correlator has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   char out_file_name[1024];
   snprintf(  out_file_name, sizeof(out_file_name)
            , "%s/%s_Rcorr_S%dSz%d_all_t%d"
            , out_file_PATH, channel.name.c_str(), spin, spin_z, time_slice );
   ofstream ofs( out_file_name, ios::out | ios::binary );
   
   cdouble tmp;
   for (int n=0; n<NBSwave::xyznSIZE; n++) {
      tmp = Rcorr[n];
      if (!analysis::machine_is_little_endian())
         analysis::endian_convert(&tmp, 1);
      ofs.write((char*)&tmp, sizeof(double));
   }
   ofs.close();

   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output mean of R-correlators data with error
 * @brief ( with/without 1/48 data size reduction )
 */
//--------------------------------------------------------------------------
void R_CORRELATOR::output_Rcorr_err(  const char* out_file_PATH
                                    , bool        reduction_flg ) {
    
   func_name = "output_Rcorr_err______";
   analysis::route(class_name, func_name, 1);

   if (Rcorr == NULL) {
      analysis::error(1,"R-correlator has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   int x_shift, y_shift, z_shift;
   double R;
   cdouble err, mean, sqr_mean;
   
   char out_file_name[1024];
   snprintf( out_file_name, sizeof(out_file_name),
            "%s/%s_Rcorr_S%dSz%d_err_t%d"
            , out_file_PATH, channel.name.c_str(), spin, spin_z, time_slice );
   ofstream ofs( out_file_name, ios::out );
   ofs << "#   x, y, z, r, mean.real, err.real, mean.imag, err.imag" << endl;
   
   for (      int z=0; z<analysis::zSIZE; z++)
      for (   int y=0; y<analysis::ySIZE; y++)
         for (int x=0; x<analysis::xSIZE; x++) {
            if (reduction_flg)
               if (x>analysis::xSIZE/2 || y>x || z>y) continue;
            
            mean     = COMP_ZERO;
            sqr_mean = COMP_ZERO;
            for (int i=0; i<analysis::Nconf; i++) {
               mean     +=          Rcorr[idx(x,y,z,i)];
               sqr_mean += cmp_sqr( Rcorr[idx(x,y,z,i)] );
            }
            mean     /= double(analysis::Nconf);
            sqr_mean /= double(analysis::Nconf);
            err = cmp_sqrt( double(analysis::Nconf-1)
                           * ( sqr_mean - cmp_sqr( mean )) );
            
            x_shift = 0, y_shift = 0, z_shift = 0;
            if (x > analysis::xSIZE/2) x_shift = analysis::xSIZE;
            if (y > analysis::ySIZE/2) y_shift = analysis::ySIZE;
            if (z > analysis::zSIZE/2) z_shift = analysis::zSIZE;
            // make r for each coodinate with periodic boundary condition
            R = sqrt(  (x - x_shift)*(x - x_shift)
                     + (y - y_shift)*(y - y_shift)
                     + (z - z_shift)*(z - z_shift) );
            
            ofs << x << " " << y << " " << z << " " << R
                << " " << mean.real() << " " << err.real()
                << " " << mean.imag() << " " << err.imag() << endl;
         }
   ofs.close();

   analysis::route(class_name, func_name, 0);
}
