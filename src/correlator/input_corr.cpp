//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Correlator
 * @brief   Function for Input correlator files
 * @author  Takaya Miyamoto
 * @since   Mon Sep  7 16:54:04 JST 2015
 */
//--------------------------------------------------------------------------

#include <correlator/correlator.h>

void CORRELATOR::input(  HADRON_TYPE hadron
                       , int num_conf, const char* FILE_TYPE) {
   
   func_name = "input_corr____________";
   analysis::route(class_name, func_name, 1);
   
   int    dummy_t;
   double tmp_r, tmp_i;
   
   snprintf(         analysis::data_list[N_TIME]
            , sizeof(analysis::data_list[N_TIME])
            , "-1");
   snprintf(         analysis::data_list[HADRON_NAME]
            , sizeof(analysis::data_list[HADRON_NAME])
            , "%s", hadron.name.c_str());
   snprintf(         analysis::data_list[CORR_DIRECTORY]
            , sizeof(analysis::data_list[CORR_DIRECTORY])
            , "%s", FILE_TYPE);
   
   string corrfile_name = analysis::set_path( num_conf );
   ifstream ifs(corrfile_name.c_str(), ios::in);
   if ( !ifs ) analysis::error(2, corrfile_name.c_str());
   
   for (int k = 0; k < analysis::tSIZE; k++) {   // Read correlator files
      ifs >> dummy_t >> tmp_r >> tmp_i;   ifs.ignore(10000, '\n');
      corr[dummy_t] = cdouble(tmp_r, tmp_i);
   }
   ifs.close();
   
   if(hadron.fb_mean_flg){
      
      string anti_HAD_name;   anti_HAD_name = "anti" + hadron.name;
      snprintf(         analysis::data_list[HADRON_NAME]
               , sizeof(analysis::data_list[HADRON_NAME])
               , "%s", anti_HAD_name.c_str());
         
      string anti_corrfile_name = analysis::set_path( num_conf );
      ifstream ifs(anti_corrfile_name.c_str(), ios::in);
      if ( !ifs ) analysis::error(2, anti_corrfile_name.c_str());
      
      ifs >> dummy_t >> tmp_r >> tmp_i;   ifs.ignore(10000, '\n');
      corr[0] = ( corr[0] + cdouble(tmp_r, tmp_i) ) * 0.5;
      
      for (int k = 1; k < analysis::tSIZE; k++) {   // Read anti-correlator files
         ifs >> dummy_t >> tmp_r >> tmp_i;   ifs.ignore(10000, '\n');
           corr[analysis::tSIZE - dummy_t] =
         ( corr[analysis::tSIZE - dummy_t] + cdouble(tmp_r, tmp_i) ) * 0.5;
      }
      ifs.close();
   }
   printf(" @ Finished input correlator : %s (%s), conf number=%d\n"
          , hadron.name.c_str(), FILE_TYPE, num_conf );
   
   analysis::route( class_name, func_name, 0 );
}
