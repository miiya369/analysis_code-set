//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Correlator
 * @brief   Function for Calculation Z-factor
 * @author  Takaya Miyamoto
 * @since   Mon Jul 20 12:48:46 JST 2015
 */
//--------------------------------------------------------------------------

#include <correlator/correlator.h>

void CORRELATOR::calc_z_factor() {
    
   func_name = "calc_z_factor_________";
   analysis::route(class_name, func_name, 1);
    
   if(corr == NULL) {
      analysis::error(1,"Correlator has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   if(Z_factor == NULL) Z_factor = new cdouble[analysis::tSIZE*analysis::Nconf];
   
   cdouble *tmp_corr = new cdouble[analysis::tSIZE*analysis::Nconf];
   int dummy_t;
   double tmp_r, tmp_i;
   
   snprintf(         analysis::data_list[N_TIME]
            , sizeof(analysis::data_list[N_TIME])
            , "-1");
   snprintf(         analysis::data_list[HADRON_NAME]
            , sizeof(analysis::data_list[HADRON_NAME])
            , "%s", hadron.name.c_str());
   snprintf(         analysis::data_list[CORR_DIRECTORY]
            , sizeof(analysis::data_list[CORR_DIRECTORY])
            , "SS");
   
   time_t start_t, end_t;
   time( &start_t );
   printf(" @ correlator (SS) reading |   0%%");
   for (int conf=0; conf<analysis::Nconf; conf++) {   // Read correlator files
      
      string corrfile_name = analysis::set_path( conf );
      ifstream ifs(corrfile_name.c_str(), ios::in);
      if ( !ifs ) analysis::error(2, corrfile_name.c_str());
      
      for (int k = 0; k < analysis::tSIZE; k++) {
         ifs >> dummy_t >> tmp_r >> tmp_i;
         ifs.ignore(10000, '\n');
         tmp_corr[nt(conf, dummy_t)] = cdouble(tmp_r, tmp_i);
      }
      ifs.close();
      
      printf("\b\b\b\b%3.0lf%%",double(conf+1)/double(analysis::Nconf)*100);
      fflush(stdout);
   } // conf
   time( &end_t );
   printf("  ( %d second )\n",(int)difftime(end_t,start_t));
   
   if(hadron.fb_mean_flg) {
      
      string anti_HAD_name;   anti_HAD_name = "anti" + hadron.name;
      snprintf(         analysis::data_list[HADRON_NAME]
               , sizeof(analysis::data_list[HADRON_NAME])
               , "%s", anti_HAD_name.c_str());
      
      time_t start_t, end_t;
      time( &start_t );
      printf(" @ anti-corr  (SS) reading |   0%%");
      for(int conf=0; conf<analysis::Nconf; conf++){ // Read anti-correlator files
         
         string anti_corrfile_name = analysis::set_path( conf );
         ifstream ifs(anti_corrfile_name.c_str(), ios::in);
         if ( !ifs ) analysis::error(2, anti_corrfile_name.c_str());
         
         ifs >> dummy_t >> tmp_r >> tmp_i;
         ifs.ignore(10000, '\n');
         tmp_corr[nt(conf, 0)] = (  tmp_corr[nt(conf, 0)]
                                  + cdouble(tmp_r, tmp_i)) * 0.5;
         
         for (int k = 1; k < analysis::tSIZE; k++) {
            ifs >> dummy_t >> tmp_r >> tmp_i;
            ifs.ignore(10000, '\n');
               tmp_corr[nt(conf, analysis::tSIZE - dummy_t)] =
            (  tmp_corr[nt(conf, analysis::tSIZE - dummy_t)]
             + cdouble(tmp_r, tmp_i) ) * 0.5;
         }
         ifs.close();
         
         printf("\b\b\b\b%3.0lf%%",double(conf+1)/double(analysis::Nconf)*100);
         fflush(stdout);
      } // conf
      time( &end_t );
      printf("  ( %d second )\n",(int)difftime(end_t,start_t));
   }
   for(   int conf = 0; conf<analysis::Nconf; conf++)
      for(int it   = 0; it  <analysis::tSIZE; it++  )
         Z_factor[nt(conf,it)] =      corr[nt(conf,it)]
                                / tmp_corr[nt(conf,it)];
   delete [] tmp_corr;

   analysis::route(class_name, func_name, 0);
}
