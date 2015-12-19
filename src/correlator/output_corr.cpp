//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Correlator
 * @brief   Functions for output correlator / effective mass data
 * @author  Takaya Miyamoto
 * @since   Mon Jul 20 12:12:25 JST 2015
 */
//--------------------------------------------------------------------------

#include <correlator/correlator.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for output correlators data every gauge configurations
 */
//--------------------------------------------------------------------------
void CORRELATOR::output_corr_all( const char* out_file_PATH ) {
   
   func_name = "output_corr_all_______";
   analysis::route(class_name, func_name, 1);
   
   if(corr == NULL) {
      analysis::error(1,"Correlator has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   char out_file_name[1024];
   snprintf(  out_file_name, sizeof(out_file_name)
            , "%s/%s_corr_all", out_file_PATH, hadron.name.c_str() );
   ofstream ofs( out_file_name, ios::out | ios::binary );
   
   ofs.write( (char*)&corr[0]
             , sizeof(cdouble) * analysis::tSIZE*analysis::Nconf );
   ofs.close();
   
   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output mean of correlators data with error
 */
//--------------------------------------------------------------------------
void CORRELATOR::output_corr_err( const char* out_file_PATH ) {
    
   func_name = "output_corr_err_______";
   analysis::route(class_name, func_name, 1);
    
   if(corr == NULL) {
      analysis::error(1,"Correlator has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   cdouble err, mean, sqr_mean;
   
   char out_file_name[1024];
   snprintf(  out_file_name, sizeof(out_file_name)
            , "%s/%s_corr_err", out_file_PATH, hadron.name.c_str() );
   ofstream ofs( out_file_name, ios::out );
   
   for(int t=0; t<analysis::tSIZE; t++) {
      mean     = COMP_ZERO;
      sqr_mean = COMP_ZERO;
      
      for( int i=0; i<analysis::Nconf; i++) {
         mean     += corr[nt(i,t)];
         sqr_mean += cmp_sqr(corr[nt(i,t)]);
      }
      mean     /= double(analysis::Nconf);
      sqr_mean /= double(analysis::Nconf);
      
      err = cmp_sqrt((sqr_mean - cmp_sqr(mean)) * double(analysis::Nconf-1));
      
      ofs << t << " " << mean.real() << " " << err.real()
               << " " << mean.imag() << " " << err.imag() << endl;
   }
   ofs.close();

   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output miyamoto-format fitting data of correlators
 */
//--------------------------------------------------------------------------
void CORRELATOR::output_corr_fit( const char* out_file_PATH ) {
 
   func_name = "output_corr_fit_______";
   analysis::route(class_name, func_name, 1);
    
   if(corr == NULL) {
      analysis::error(1,"Correlator has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   double err, mean, sqr_mean;
   double *tmp_corr = new double[analysis::tSIZE*analysis::Nconf];
   for(size_t i=0; i<analysis::tSIZE*analysis::Nconf; i++)
      tmp_corr[i] = corr[i].real();
   
   int  fit_type = 1;
   char out_file_name[1024];
   snprintf(  out_file_name, sizeof(out_file_name)
            , "%s/%s_corr_fitdata", out_file_PATH, hadron.name.c_str() );
   ofstream ofs( out_file_name, ios::out | ios::binary );
   
   ofs.write((char*)&fit_type,        sizeof(int));
   ofs.write((char*)&analysis::Nconf, sizeof(int));
   ofs.write((char*)&analysis::tSIZE, sizeof(int));   // write file header
   
   for(int t=0; t<analysis::tSIZE; t++) {
      mean     = 0.0;
      sqr_mean = 0.0;
      
      for(int i=0; i<analysis::Nconf; i++) {
         mean     += tmp_corr[nt(i,t)];
         sqr_mean += pow(tmp_corr[nt(i,t)],2);
      }
      mean     /= double(analysis::Nconf);
      sqr_mean /= double(analysis::Nconf);
      
      err = sqrt((sqr_mean - mean*mean) * double(analysis::Nconf-1));
      
      ofs.write((char*)&err, sizeof(double));   // write fit data
      for( int i=0; i<analysis::Nconf; i++)
         ofs.write((char*)&tmp_corr[nt(i,t)], sizeof(double));
   }
   ofs.close();
   delete [] tmp_corr;

   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output effective masses data every gauge configurations
 */
//--------------------------------------------------------------------------
void CORRELATOR::output_effmass_all( const char* out_file_PATH ) {
    
   func_name = "output_effmass_all____";
   analysis::route(class_name, func_name, 1);

   if(corr == NULL) {
      analysis::error(1,"Correlator has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   cdouble *eff_mass = new cdouble[(analysis::tSIZE-1)*analysis::Nconf];
   
   for(int t=0; t<analysis::tSIZE-1; t++) for(int i=0; i<analysis::Nconf; i++)
      eff_mass[nt(i,t)] = -log(corr[nt(i,t+1)] / corr[nt(i,t)]);
   
   char out_file_name[1024];
   snprintf(  out_file_name, sizeof(out_file_name)
            , "%s/%s_effmass_all", out_file_PATH, hadron.name.c_str() );
   ofstream ofs( out_file_name, ios::out | ios::binary );
   
   ofs.write(  (char*)&eff_mass[0]
             , sizeof(cdouble)*(analysis::tSIZE-1)*analysis::Nconf );
   ofs.close();
   delete [] eff_mass;
   
   analysis::route( class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output mean of effective masses data with error
 */
//--------------------------------------------------------------------------
void CORRELATOR::output_effmass_err( const char* out_file_PATH ) {
    
   func_name = "output_effmass_err____";
   analysis::route( class_name, func_name, 1 );
    
   if(corr == NULL) {
      analysis::error(1,"Correlator has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   cdouble *eff_mass = new cdouble[(analysis::tSIZE-1)*analysis::Nconf];
   
   for(int t=0; t<analysis::tSIZE-1; t++) for(int i=0; i<analysis::Nconf; i++)
      eff_mass[nt(i,t)] = -log(corr[nt(i,t+1)] / corr[nt(i,t)]);
   
   cdouble err, mean, sqr_mean;
   
   char out_file_name[1024];
   snprintf(  out_file_name, sizeof(out_file_name)
            , "%s/%s_effmass_err", out_file_PATH, hadron.name.c_str() );
   ofstream ofs( out_file_name, ios::out );
   
   for(int t=0; t<analysis::tSIZE-1 ; t++) {
      mean     = COMP_ZERO;
      sqr_mean = COMP_ZERO;
      
      for(int i=0; i<analysis::Nconf; i++) {
         mean     += eff_mass[nt(i,t)];
         sqr_mean += cmp_sqr(eff_mass[nt(i,t)]);
      }
      mean     /= double(analysis::Nconf);
      sqr_mean /= double(analysis::Nconf);
      
      err = cmp_sqrt((sqr_mean - cmp_sqr(mean)) * double(analysis::Nconf-1));
      
      ofs << t << " " << mean.real() << " " << err.real()
               << " " << mean.imag() << " " << err.imag() << endl;
   }
   ofs.close();
   delete [] eff_mass;

   analysis::route(class_name, func_name, 0);
}
