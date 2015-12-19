//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Functions for output potential data
 * @author  Takaya Miyamoto
 * @since   Wed Jul 22 07:52:42 JST 2015
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>

//--------------------------------------------------------------------------
/**
 * @brief Function for output potentials data every gauge configurations
 */
//--------------------------------------------------------------------------
void POTENTIAL::output_single_pot_all( const char* out_file_PATH ) {
    
    func_name = "output_single_pot_all_";
    analysis::route(class_name, func_name, 1);

   if (potential == NULL) {
      analysis::error(1,"Potential has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   char out_file_name[1024];
   snprintf(  out_file_name, sizeof(out_file_name)
            , "%s/%s_%s_all_t%d"
            , out_file_PATH, channel.name.c_str()
            , potential_type.c_str(), time_slice );
   ofstream ofs(out_file_name, ios::out | ios::binary);
   
   cdouble *Rcorr_ptr = NULL;
   for (int ttt=0; ttt<3; ttt++)
      if (Rcorr_t[ttt] == time_slice)
         Rcorr_ptr = Rcorr[ttt].Rcorr;
   
   cdouble tmp;
   for (size_t n=0; n<NBSwave::xyznSIZE; n++) {
      tmp =  potential[n] / Rcorr_ptr[n];
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
 */
//--------------------------------------------------------------------------
void POTENTIAL::output_single_pot_err( const char* out_file_PATH ) {
    
   func_name = "output_single_pot_err_";
   analysis::route(class_name, func_name, 1);
   
   if (potential == NULL) {
      analysis::error(1,"Potential has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   int x_shift, y_shift, z_shift;
   double R;
   cdouble err, mean, sqr_mean;
   int Ndata = NBSwave::data_reduction(-1);
   
   char out_file_name[1024];
   snprintf(  out_file_name, sizeof(out_file_name)
            , "%s/%s_%s_err_t%d"
            , out_file_PATH, channel.name.c_str()
            , potential_type.c_str(), time_slice );
   ofstream ofs(out_file_name, ios::out);
   
   cdouble *Rcorr_ptr = NULL;
   for(int ttt=0; ttt<3; ttt++)
      if(Rcorr_t[ttt] == time_slice)
         Rcorr_ptr = Rcorr[ttt].Rcorr;
   
   for (size_t n=0; n<Ndata; n++) {
      size_t reduce_xyz = NBSwave::data_reduction(n);
      int x, y, z;
      analysis::convert_xyz(&x, &y, &z, reduce_xyz);
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
      err = cmp_sqrt(double(analysis::Nconf-1) * (sqr_mean - cmp_sqr(mean)));
      
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

//--------------------------------------------------------------------------
/**
 * @brief Function for output miyamoto-format fitting data of potentail
 */
//--------------------------------------------------------------------------
void POTENTIAL::output_single_pot_fit( const char* out_file_PATH ) {
   
   func_name = "output_single_pot_fit_";
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
            , "%s/%s_%s_fitdata_t%d"
            , out_file_PATH, channel.name.c_str()
            , potential_type.c_str(), time_slice );
   ofstream ofs(out_file_name, ios::out);
   
   int fit_type = 2;   // write file header
   int Ndata    = NBSwave::data_reduction(-1);
   int tmp_Ndata = Ndata;
   int tmp_Nconf = analysis::Nconf;
   
   if (!analysis::machine_is_little_endian()) {
      analysis::endian_convert(&fit_type, 1);
      analysis::endian_convert(&tmp_Nconf,1);
      analysis::endian_convert(&tmp_Ndata,1);
   }
   ofs.write((char*)&fit_type,  sizeof(int));
   ofs.write((char*)&tmp_Nconf, sizeof(int));
   ofs.write((char*)&tmp_Ndata, sizeof(int));
   
   cdouble *Rcorr_ptr = NULL;
   for(int ttt=0; ttt<3; ttt++)
      if(Rcorr_t[ttt] == time_slice)
         Rcorr_ptr = Rcorr[ttt].Rcorr;
   
   for (size_t n=0; n<Ndata; n++) {
      size_t reduce_xyz = NBSwave::data_reduction(n);
      int x, y, z;
      analysis::convert_xyz(&x, &y, &z, reduce_xyz);
      
      mean          = 0.0;
      sqr_mean      = 0.0;
      for (int i=0; i<analysis::Nconf; i++) {
         potential_tmp = potential[idx(x,y,z,i)] / Rcorr_ptr[idx(x,y,z,i)];
         mean         +=     potential_tmp.real();
         sqr_mean     += pow(potential_tmp.real(), 2);
      }
      mean     /= double(analysis::Nconf);
      sqr_mean /= double(analysis::Nconf);
      err       = sqrt(double(analysis::Nconf-1) * (sqr_mean - pow(mean,2)));
      
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
         potential_tmp = potential[idx(x,y,z,i)] / Rcorr_ptr[idx(x,y,z,i)];
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
 * @brief Function for output miyamoto-format fitting data of potentail ((old))
 */
//--------------------------------------------------------------------------
void POTENTIAL::output_single_pot_fit_old( const char* out_file_PATH ) {
   
   func_name = "output_single_pot_fit_";
   analysis::route(class_name, func_name, 1);
   
   if (potential == NULL) {
      analysis::error(1,"Potential has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   char out_file_name[1024];
   snprintf(  out_file_name, sizeof(out_file_name)
            , "%s/%s_%s_fitdata_t%d"
            , out_file_PATH, channel.name.c_str()
            , potential_type.c_str(), time_slice );
   ofstream ofs(out_file_name, ios::out | ios::binary);
   
   double  *potential_tmp = new double[NBSwave::xyznSIZE];
   cdouble tmp;
   cdouble *Rcorr_ptr = NULL;
   
   for (int ttt=0; ttt<3; ttt++)
      if (Rcorr_t[ttt] == time_slice)
         Rcorr_ptr = Rcorr[ttt].Rcorr;
   
   for (size_t n=0; n<NBSwave::xyznSIZE; n++) {
      tmp =   potential[n] / Rcorr_ptr[n];
      potential_tmp[n] = tmp.real();
   }

///*********   error calculate   *********
   double *err = new double[NBSwave::xyzSIZE];
   double mean, sqr_mean;
   for (      int z=0; z<analysis::zSIZE; z++)
      for (   int y=0; y<analysis::ySIZE; y++)
         for (int x=0; x<analysis::xSIZE; x++) {
            mean     = 0.0;
            sqr_mean = 0.0;
            
            for (int i=0; i<analysis::Nconf; i++) {
               mean     +=     potential_tmp[idx(x,y,z,i)];
               sqr_mean += pow(potential_tmp[idx(x,y,z,i)],2);
            }
            mean     /= double(analysis::Nconf);
            sqr_mean /= double(analysis::Nconf);
            
            err[xyz(x,y,z)] = sqrt( double(analysis::Nconf-1)
                                     * (sqr_mean - mean*mean) );
         }
   
///*********   write file header   *********
   int fit_type = 2;
   ofs.write((char*)&fit_type,        sizeof(int) );
   ofs.write((char*)&analysis::Nconf, sizeof(int) );

///*********   data reduction   *********
   int min_Ndata = 999999;
   int tmp_Ndata;
   int data_count = 0;
   bool *data_checker = new bool[NBSwave::xyzSIZE];
   for (size_t i=0; i<NBSwave::xyzSIZE; i++)
      data_checker[i] = true;
   
//============== If data reduction is wrong, chenge this one =============//
   double reduction_precision = 0.0000000001;
//========================================================================//

   printf(" @ Data reduction          |   0%%");
   for (size_t i=0; i<analysis::Nconf; i++) {
      tmp_Ndata = NBSwave::xyzSIZE;
      
      for (size_t j=0; j<NBSwave::xyzSIZE-1; j++) {
         if (!data_checker[j]) continue;
         
         for (size_t k=j+1; k<NBSwave::xyzSIZE; k++) {
            if(   potential_tmp[j+NBSwave::xyzSIZE*i] + reduction_precision
               >= potential_tmp[k+NBSwave::xyzSIZE*i]
               &&
                  potential_tmp[j+NBSwave::xyzSIZE*i] - reduction_precision
               <= potential_tmp[k+NBSwave::xyzSIZE*i] ) {
               
               data_checker[k] = false;
               tmp_Ndata--;
            }
         }
      }
      if (       min_Ndata >  tmp_Ndata) {
         min_Ndata = tmp_Ndata;
         data_count = 1;
      } else if (min_Ndata == tmp_Ndata) {
         data_count++;
      }
      printf("\b\b\b\b%3.0lf%%",double(i+1)/double(analysis::Nconf)*100);
      fflush(stdout);
   }
   printf("\n @@@ #.data     = %d       ### FOR DEBUG\n", min_Ndata);
   printf(" @@@ Data count = %d / %d ### FOR DEBUG\n"
          , data_count, analysis::Nconf);
   ofs.write((char*)&min_Ndata, sizeof(int));
   
///*********   write data   *********
   double R;
   int x_shift, y_shift, z_shift;
   for (      int z=0; z<analysis::zSIZE; z++)
      for (   int y=0; y<analysis::ySIZE; y++)
         for (int x=0; x<analysis::xSIZE; x++) {
            if (!data_checker[xyz(x,y,z)]) continue;
            
            x_shift = 0, y_shift = 0, z_shift = 0;
            if (x > analysis::xSIZE/2) x_shift = analysis::xSIZE;
            if (y > analysis::ySIZE/2) y_shift = analysis::ySIZE;
            if (z > analysis::zSIZE/2) z_shift = analysis::zSIZE;
            
            R = sqrt(  (x - x_shift)*(x - x_shift)
                     + (y - y_shift)*(y - y_shift)
                     + (z - z_shift)*(z - z_shift) );
            
            ofs.write((char*)&R              , sizeof(double));
            ofs.write((char*)&err[xyz(x,y,z)], sizeof(double));
            
            for (int i=0; i<analysis::Nconf; i++)
               ofs.write((char*)&potential_tmp[idx(x,y,z,i)], sizeof(double));
         }
   ofs.close();
   delete [] err;
   delete [] potential_tmp;
   delete [] data_checker;

   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Function for output potentials data every gauge configurations (for CCP)
 */
//--------------------------------------------------------------------------
void POTENTIAL::output_couple_pot_all( const char* out_file_PATH ) {

   func_name = "output_single_pot_all_";
   analysis::route(class_name, func_name, 1);
   
   if (potential == NULL) {
      analysis::error(1,"Potential has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   char out_file_name[1024];
   snprintf(  out_file_name, sizeof(out_file_name)
            , "%s/%s_%s_ccp_all_t%d"
            , out_file_PATH, channel.name.c_str()
            , potential_type.c_str(), time_slice );
   ofstream ofs(out_file_name, ios::out | ios::binary);
   
   cdouble tmp;
   for (size_t n=0; n<NBSwave::xyznSIZE; n++) {
      tmp =  potential[n];
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
void POTENTIAL::output_couple_pot_err( const char* out_file_PATH ) {
   
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
   int Ndata = NBSwave::data_reduction(-1);
   
   char out_file_name[1024];
   snprintf(  out_file_name, sizeof(out_file_name)
            , "%s/%s_%s_ccp_err_t%d"
            , out_file_PATH, channel.name.c_str()
            , potential_type.c_str(), time_slice );
   ofstream ofs(out_file_name, ios::out);
   
   for (size_t n=0; n<Ndata; n++) {
      size_t reduce_xyz = NBSwave::data_reduction(n);
      int x, y, z;
      analysis::convert_xyz(&x, &y, &z, reduce_xyz);
      mean     = COMP_ZERO;
      sqr_mean = COMP_ZERO;
      
      for (int i=0; i<analysis::Nconf; i++) {
         mean     +=        (  potential[idx(x,y,z,i)] );
         sqr_mean += cmp_sqr(  potential[idx(x,y,z,i)] );
      }
      mean     /= double(analysis::Nconf);
      sqr_mean /= double(analysis::Nconf);
      err = cmp_sqrt(double(analysis::Nconf-1) * (sqr_mean - cmp_sqr(mean)));
      
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
            , "%s/%s_%s_ccp_fitdata_t%d"
            , out_file_PATH, channel.name.c_str()
            , potential_type.c_str(), time_slice );
   ofstream ofs(out_file_name, ios::out);
   
   int fit_type = 2;   // write file header
   int Ndata    = NBSwave::data_reduction(-1);
   int tmp_Ndata = Ndata;
   int tmp_Nconf = analysis::Nconf;
   
   if (!analysis::machine_is_little_endian()) {
      analysis::endian_convert(&fit_type, 1);
      analysis::endian_convert(&tmp_Nconf,1);
      analysis::endian_convert(&tmp_Ndata,1);
   }
   ofs.write((char*)&fit_type,  sizeof(int));
   ofs.write((char*)&tmp_Nconf, sizeof(int));
   ofs.write((char*)&tmp_Ndata, sizeof(int));
   
   for (size_t n=0; n<Ndata; n++) {
      size_t reduce_xyz = NBSwave::data_reduction(n);
      int x, y, z;
      analysis::convert_xyz(&x, &y, &z, reduce_xyz);
      
      mean          = 0.0;
      sqr_mean      = 0.0;
      for (int i=0; i<analysis::Nconf; i++) {
         potential_tmp = potential[idx(x,y,z,i)];
         mean         +=     potential_tmp.real();
         sqr_mean     += pow(potential_tmp.real(), 2);
      }
      mean     /= double(analysis::Nconf);
      sqr_mean /= double(analysis::Nconf);
      err       = sqrt(double(analysis::Nconf-1) * (sqr_mean - pow(mean,2)));
      
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
