//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup R-correlator
 * @brief   Functions for output R-correlator data
 * @author  Takaya Miyamoto
 * @since   Tue Jul 21 00:30:10 JST 2015
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
            , "%s/%s_Rcorr_all_t%d"
            , out_file_PATH, channel.name.c_str(), time_slice );
   ofstream ofs( out_file_name, ios::out | ios::binary );
   
   ofs.write( (char*)&Rcorr[0], sizeof(cdouble) * NBSwave::xyznSIZE);
   ofs.close();

   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output mean of R-correlators data with error
 */
//--------------------------------------------------------------------------
void R_CORRELATOR::output_Rcorr_err( const char* out_file_PATH ) {
    
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
            "%s/%s_Rcorr_err_t%d"
            , out_file_PATH, channel.name.c_str(), time_slice );
   ofstream ofs( out_file_name, ios::out );
   
   for (      int z=0; z<analysis::zSIZE; z++)
      for (   int y=0; y<analysis::ySIZE; y++)
         for (int x=0; x<analysis::xSIZE; x++) {
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
            
            ofs << R << " " << mean.real() << " " << err.real()
                     << " " << mean.imag() << " " << err.imag() << endl;
         }
   ofs.close();

   analysis::route(class_name, func_name, 0);
}
