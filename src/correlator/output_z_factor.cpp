//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Correlator
 * @brief   Functions for output Z-factor data
 * @author  Takaya Miyamoto
 * @since   Mon Jul 20 12:13:15 JST 2015
 */
//--------------------------------------------------------------------------

#include <correlator/correlator.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for output Z-factor data every gauge configurations
 */
//--------------------------------------------------------------------------
void CORRELATOR::output_z_factor_all( const char* out_file_PATH ) {
    
   func_name = "output_z_factor_all___";
   analysis::route(class_name, func_name, 1);
    
   if(Z_factor == NULL) {
      analysis::error(1,"Z-factor has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   char out_file_name[1024];
   snprintf(  out_file_name, sizeof(out_file_name)
            , "%s/%s_Z-factor_all", out_file_PATH, hadron.name.c_str() );
   ofstream ofs( out_file_name, ios::out | ios::binary );
   
   ofs.write( (char*)&Z_factor[0]
             , sizeof(cdouble) * analysis::tSIZE*analysis::Nconf );
   ofs.close();
   
   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output mean of Z-factor data with error
 */
//--------------------------------------------------------------------------
void CORRELATOR::output_z_factor_err( const char* out_file_PATH ) {
   
   func_name = "output_z_factor_err___";
   analysis::route(class_name, func_name, 1);
   
   if(Z_factor == NULL) {
      analysis::error(1,"Z-factor has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   cdouble err, mean, sqr_mean;
   
   char out_file_name[1024];
   snprintf(  out_file_name, sizeof(out_file_name)
            , "%s/%s_Z-factor_err", out_file_PATH, hadron.name.c_str() );
   ofstream ofs( out_file_name, ios::out );
   
   for(int t=0; t<analysis::tSIZE; t++) {
      mean     = COMP_ZERO;
      sqr_mean = COMP_ZERO;
      
      for(int i=0; i<analysis::Nconf; i++) {
         mean     += Z_factor[nt(i,t)];
         sqr_mean += cmp_sqr(Z_factor[nt(i,t)]);
      }
      mean     /= double(analysis::Nconf);
      sqr_mean /= double(analysis::Nconf);
      
      err = cmp_sqrt((sqr_mean - cmp_sqr(mean)) / double(analysis::Nconf-1));
      
      ofs << t << " " << mean.real() << " " << err.real()
               << " " << mean.imag() << " " << err.imag() << endl;
   }
   ofs.close();
   
   analysis::route(class_name, func_name, 0);
}
