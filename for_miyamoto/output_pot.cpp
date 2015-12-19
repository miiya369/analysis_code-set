//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Functions for output potential data
 * @author  Takaya Miyamoto
 * @since   Wed Jul 29 02:06:45 JST 2015
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for output potentials data every gauge configurations
 */
//--------------------------------------------------------------------------
void POTENTIAL::output_pot_all( const char* out_file_PATH ) {
    
    func_name = "output_pot_all________";
    analysis::route(class_name, func_name, 1);

   if (potential == NULL) {
      analysis::error(1,"Potential has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   char out_file_name[1024];
   snprintf(  out_file_name, sizeof(out_file_name)
            , "%s/%s_%s_S%dSz%d_all_t%d"
            , out_file_PATH, channel.name.c_str()
            , potential_type.c_str(), spin, spin_z, time_slice );
   ofstream ofs(out_file_name, ios::out | ios::binary);
   
   cdouble *Rcorr_ptr = NULL;
   for (int ttt=0; ttt<3; ttt++)
      if (Rcorr_t[ttt] == time_slice)
         Rcorr_ptr = Rcorr[ttt].Rcorr;
   
   cdouble tmp;
   for (size_t n=0; n<NBSwave::xyznSIZE; n++) {
      tmp = potential[n] / Rcorr_ptr[n];
      if (!analysis::machine_is_little_endian())
         analysis::endian_convert(&tmp, 1);
      ofs.write( (char*)&tmp, sizeof(cdouble) );
   }
   ofs.close();
   
   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output mean of potentials data with error 
 * @brief ( with/without 1/48 data size reduction )
 */
//--------------------------------------------------------------------------
void POTENTIAL::output_pot_err( const char* out_file_PATH, bool reduction_flg ) {
    
   func_name = "output_pot_err________";
   analysis::route(class_name, func_name, 1);
   
   if (potential == NULL) {
      analysis::error(1,"Potential has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   int x_shift, y_shift, z_shift;
   double R;
   cdouble err, mean, sqr_mean;
   
   char out_file_name[1024];
   snprintf(  out_file_name, sizeof(out_file_name)
            , "%s/%s_%s_S%dSz%d_err_t%d"
            , out_file_PATH, channel.name.c_str()
            , potential_type.c_str(), spin, spin_z, time_slice );
   ofstream ofs(out_file_name, ios::out);
   ofs << "#   x, y, z, r, mean.real, err.real, mean.imag, err.imag" << endl;
   
   cdouble *Rcorr_ptr = NULL;
   for(int ttt=0; ttt<3; ttt++)
      if(Rcorr_t[ttt] == time_slice)
         Rcorr_ptr = Rcorr[ttt].Rcorr;
   
   for (      int z=0; z<analysis::zSIZE; z++)
      for (   int y=0; y<analysis::ySIZE; y++)
         for (int x=0; x<analysis::xSIZE; x++) {
            if (reduction_flg)
               if (x>analysis::xSIZE/2 || y>x || z>y) continue;
   
            mean     = COMP_ZERO;
            sqr_mean = COMP_ZERO;
            for (int i=0; i<analysis::Nconf; i++) {
               mean     +=        (  potential[idx(x,y,z,i)]
                                   / Rcorr_ptr[idx(x,y,z,i)] );
               sqr_mean += cmp_sqr(  potential[idx(x,y,z,i)]
                                   / Rcorr_ptr[idx(x,y,z,i)] );
            }
            mean     /= double(analysis::Nconf);
            sqr_mean /= double(analysis::Nconf);
            err = cmp_sqrt( double(analysis::Nconf-1)
                           * (sqr_mean - cmp_sqr(mean)));
   
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

//--------------------------------------------------------------------------
/**
 * @brief Function for output miyamoto-format fitting data of potentail
 */
//--------------------------------------------------------------------------
void POTENTIAL::output_pot_fit( const char* out_file_PATH ) {
   
   func_name = "output_pot_fit________";
   analysis::route(class_name, func_name, 1);
   
   if (potential == NULL) {
      analysis::error(1,"Potential has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   int x_shift, y_shift, z_shift;
   double R, err, mean, sqr_mean, tmp;
   cdouble potential_tmp;
   
   char out_file_name[1024];
   snprintf(  out_file_name, sizeof(out_file_name)
            , "%s/%s_%s_S%dSz%d_fitdata_t%d"
            , out_file_PATH, channel.name.c_str()
            , potential_type.c_str(), spin, spin_z, time_slice );
   ofstream ofs(out_file_name, ios::out);
   
   cdouble *Rcorr_ptr = NULL;
   for(int ttt=0; ttt<3; ttt++)
      if(Rcorr_t[ttt] == time_slice)
         Rcorr_ptr = Rcorr[ttt].Rcorr;
   
   int fit_type  = 2;   // write file header
   int tmp_Nconf = analysis::Nconf;
   int Ndata     = NBSwave::reduced_Ndata();
   int tmp_Ndata = Ndata;
   
   if (!analysis::machine_is_little_endian()) {
      analysis::endian_convert(&fit_type, 1);
      analysis::endian_convert(&tmp_Nconf,1);
      analysis::endian_convert(&tmp_Ndata,1);
   }
   ofs.write((char*)&fit_type,  sizeof(int));
   ofs.write((char*)&tmp_Nconf, sizeof(int));
   ofs.write((char*)&tmp_Ndata, sizeof(int));
   
   for (      int z=0; z<analysis::zSIZE; z++)
      for (   int y=0; y<analysis::ySIZE; y++)
         for (int x=0; x<analysis::xSIZE; x++) {
            if (x>analysis::xSIZE/2 || y>x || z>y) continue;
   
            mean          = 0.0;
            sqr_mean      = 0.0;
            for (int i=0; i<analysis::Nconf; i++) {
               potential_tmp = potential[idx(x,y,z,i)]/Rcorr_ptr[idx(x,y,z,i)];
               mean         +=     potential_tmp.real();
               sqr_mean     += pow(potential_tmp.real(), 2);
            }
            mean     /= double(analysis::Nconf);
            sqr_mean /= double(analysis::Nconf);
            err       = sqrt(double(analysis::Nconf-1)*(sqr_mean-pow(mean,2)));
            
            x_shift = 0, y_shift = 0, z_shift = 0;
            if (x > analysis::xSIZE/2) x_shift = analysis::xSIZE;
            if (y > analysis::ySIZE/2) y_shift = analysis::ySIZE;
            if (z > analysis::zSIZE/2) z_shift = analysis::zSIZE;
            // make r for each coodinate with periodic boundary condition
            R = sqrt(  (x - x_shift)*(x - x_shift)
                     + (y - y_shift)*(y - y_shift)
                     + (z - z_shift)*(z - z_shift) );
            
            if (!analysis::machine_is_little_endian()) {
               analysis::endian_convert(&R,  1);
               analysis::endian_convert(&err,1);
            }
            ofs.write((char*)&R  , sizeof(double));
            ofs.write((char*)&err, sizeof(double));
            
            for (int i=0; i<analysis::Nconf; i++){
               potential_tmp = potential[idx(x,y,z,i)]/Rcorr_ptr[idx(x,y,z,i)];
               tmp = potential_tmp.real();
               if (!analysis::machine_is_little_endian())
                  analysis::endian_convert(&tmp,1);
               ofs.write((char*)&tmp, sizeof(double));
            }
         }
   ofs.close();
   
   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output potentials data every gauge configurations (for CCP)
 */
//--------------------------------------------------------------------------
void POTENTIAL::output_couple_pot_all( const char* out_file_PATH ) {

   func_name = "output_couple_pot_all_";
   analysis::route(class_name, func_name, 1);
   
   if (potential == NULL) {
      analysis::error(1,"Potential has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   char out_file_name[1024];
   snprintf(  out_file_name, sizeof(out_file_name)
            , "%s/%s_%s_S%dSz%d_ccp_all_t%d"
            , out_file_PATH, channel.name.c_str()
            , potential_type.c_str(), spin, spin_z, time_slice );
   ofstream ofs(out_file_name, ios::out | ios::binary);
   
   cdouble tmp;
   for (size_t n=0; n<NBSwave::xyznSIZE; n++) {
      tmp = potential[n];
      if (!analysis::machine_is_little_endian())
         analysis::endian_convert(&tmp, 1);
      ofs.write( (char*)&tmp, sizeof(cdouble) );
   }
   ofs.close();
   
   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output mean of potentials data with error (for CCP)
 */
//--------------------------------------------------------------------------
void POTENTIAL::output_couple_pot_err(  const char* out_file_PATH
                                      , bool        reduction_flg  ) {
   
   func_name = "output_couple_pot_err_";
   analysis::route(class_name, func_name, 1);
   
   if (potential == NULL) {
      analysis::error(1,"Potential has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   int x_shift, y_shift, z_shift;
   double R;
   cdouble err, mean, sqr_mean;
   
   char out_file_name[1024];
   snprintf(  out_file_name, sizeof(out_file_name)
            , "%s/%s_%s_S%dSz%d_ccp_err_t%d"
            , out_file_PATH, channel.name.c_str()
            , potential_type.c_str(), spin, spin_z, time_slice );
   ofstream ofs(out_file_name, ios::out);
   ofs << "#   x, y, z, r, mean.real, err.real, mean.imag, err.imag" << endl;
   
   for (      int z=0; z<analysis::zSIZE; z++)
      for (   int y=0; y<analysis::ySIZE; y++)
         for (int x=0; x<analysis::xSIZE; x++) {
            if (reduction_flg)
               if (x>analysis::xSIZE/2 || y>x || z>y) continue;
            
            mean     = COMP_ZERO;
            sqr_mean = COMP_ZERO;
            for (int i=0; i<analysis::Nconf; i++) {
               mean     +=        ( potential[idx(x,y,z,i)] );
               sqr_mean += cmp_sqr( potential[idx(x,y,z,i)] );
            }
            mean     /= double(analysis::Nconf);
            sqr_mean /= double(analysis::Nconf);
            err = cmp_sqrt( double(analysis::Nconf-1)
                           * (sqr_mean - cmp_sqr(mean)));
            
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

//--------------------------------------------------------------------------
/**
 * @brief Function for output miyamoto-format fitting data of potentail (for CCP)
 */
//--------------------------------------------------------------------------
void POTENTIAL::output_couple_pot_fit( const char* out_file_PATH ) {
   
   func_name = "output_couple_pot_fit_";
   analysis::route(class_name, func_name, 1);
   
   if (potential == NULL) {
      analysis::error(1,"Potential has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   int x_shift, y_shift, z_shift;
   double R, err, mean, sqr_mean, tmp;
   cdouble potential_tmp;
   
   char out_file_name[1024];
   snprintf(  out_file_name, sizeof(out_file_name)
            , "%s/%s_%s_S%dSz%d_ccp_fitdata_t%d"
            , out_file_PATH, channel.name.c_str()
            , potential_type.c_str(), spin, spin_z, time_slice );
   ofstream ofs(out_file_name, ios::out);
   
   int fit_type  = 2;   // write file header
   int tmp_Nconf = analysis::Nconf;
   int Ndata     = NBSwave::reduced_Ndata();
   int tmp_Ndata = Ndata;
   
   if (!analysis::machine_is_little_endian()) {
      analysis::endian_convert(&fit_type, 1);
      analysis::endian_convert(&tmp_Nconf,1);
      analysis::endian_convert(&tmp_Ndata,1);
   }
   ofs.write((char*)&fit_type,  sizeof(int));
   ofs.write((char*)&tmp_Nconf, sizeof(int));
   ofs.write((char*)&tmp_Ndata, sizeof(int));
   
   for (      int z=0; z<analysis::zSIZE; z++)
      for (   int y=0; y<analysis::ySIZE; y++)
         for (int x=0; x<analysis::xSIZE; x++) {
            if (x>analysis::xSIZE/2 || y>x || z>y) continue;
            
            mean          = 0.0;
            sqr_mean      = 0.0;
            for (int i=0; i<analysis::Nconf; i++) {
               potential_tmp = potential[idx(x,y,z,i)];
               mean         +=     potential_tmp.real();
               sqr_mean     += pow(potential_tmp.real(), 2);
            }
            mean     /= double(analysis::Nconf);
            sqr_mean /= double(analysis::Nconf);
            err       = sqrt(double(analysis::Nconf-1)*(sqr_mean-pow(mean,2)));
            
            x_shift = 0, y_shift = 0, z_shift = 0;
            if (x > analysis::xSIZE/2) x_shift = analysis::xSIZE;
            if (y > analysis::ySIZE/2) y_shift = analysis::ySIZE;
            if (z > analysis::zSIZE/2) z_shift = analysis::zSIZE;
            // make r for each coodinate with periodic boundary condition
            R = sqrt(  (x - x_shift)*(x - x_shift)
                     + (y - y_shift)*(y - y_shift)
                     + (z - z_shift)*(z - z_shift) );
            
            if (!analysis::machine_is_little_endian()) {
               analysis::endian_convert(&R,  1);
               analysis::endian_convert(&err,1);
            }
            ofs.write((char*)&R  , sizeof(double));
            ofs.write((char*)&err, sizeof(double));
            
            for (int i=0; i<analysis::Nconf; i++){
               potential_tmp = potential[idx(x,y,z,i)];
               tmp = potential_tmp.real();
               if (!analysis::machine_is_little_endian())
                  analysis::endian_convert(&tmp,1);
               ofs.write((char*)&tmp, sizeof(double));
            }
         }
   ofs.close();
   
   analysis::route(class_name, func_name, 0);
}
