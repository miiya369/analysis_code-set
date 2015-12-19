//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBS wave function
 * @brief   Functions for output NBS wave function data
 * @author  Takaya Miyamoto
 * @since   Thu Jul 23 15:10:21 JST 2015
 */
//--------------------------------------------------------------------------

#include <NBSwave/NBSwave.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for output NBS wave functions data every gauge configurations
 */
//--------------------------------------------------------------------------
void NBS_WAVE::output_NBS_all( const char* out_file_PATH ) {
    
   func_name = "output_NBS_all________";
   analysis::route(class_name, func_name, 1);

   if (projNBS == NULL) {
      analysis::error(1,"NBS wave function has not been projected yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   char out_file_name[1024];
   snprintf( out_file_name, sizeof(out_file_name)
            , "%s/%s_NBS_all_t%d"
            , out_file_PATH, channel.name.c_str(), time_slice );
   ofstream ofs( out_file_name, ios::out | ios::binary );
   
   cdouble tmp;
   for (int n=0; n<NBSwave::xyznSIZE; n++) {
      tmp = projNBS[n];
      if (!analysis::machine_is_little_endian())
         analysis::endian_convert(&tmp, 1);
      ofs.write((char*)&tmp, sizeof(double));
   }
   ofs.close();
      
   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output mean of NBS wave functions data with error
 */
//--------------------------------------------------------------------------
void NBS_WAVE::output_NBS_err( const char* out_file_PATH ) {
    
   func_name = "output_NBS_err________";
   analysis::route(class_name, func_name, 1);
   
   if (projNBS == NULL) {
      analysis::error(1,"NBS wave function has not been projected yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   int x_shift, y_shift, z_shift;
   double R;
   cdouble err, mean, sqr_mean;
   
   char out_file_name[1024];
   snprintf( out_file_name, sizeof(out_file_name)
            , "%s/%s_NBS_err_t%d"
            , out_file_PATH, channel.name.c_str(), time_slice );
   ofstream ofs( out_file_name, ios::out );

   for (      int z=0; z<analysis::zSIZE; z++)
      for (   int y=0; y<analysis::ySIZE; y++)
         for (int x=0; x<analysis::xSIZE; x++) {
            mean     = COMP_ZERO;
            sqr_mean = COMP_ZERO;
                    
            for (int i=0; i<analysis::Nconf; i++) {
               mean     +=          projNBS[idx(x,y,z,i)];
               sqr_mean += cmp_sqr( projNBS[idx(x,y,z,i)] );
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