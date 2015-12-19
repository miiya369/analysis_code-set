//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Correlator
 * @brief   Function for Input correlator files
 * @author  Takaya Miyamoto
 * @since   Fri Dec 18 00:36:19 JST 2015
 */
//--------------------------------------------------------------------------

#include <correlator/correlator.h>

void CORRELATOR::input(  HADRON_TYPE hadron, int num_conf, int momentum
                       , const char* CORRtype) {
   
   func_name = "input_corr_PS/PP______";
   analysis::route(class_name, func_name, 1);
   
   if (momentum<0 || momentum>4)
      analysis::error(3, "The momentum of correlator should set [0-4].");
   
   analysis::set_data_list(N_TIME,         "-1");
   analysis::set_data_list(HADRON_NAME,    "%s", hadron.name.c_str());
   analysis::set_data_list(CORR_DIRECTORY, "%s", CORRtype);
   
   FILE* fp = fopen( analysis::set_path( num_conf ).c_str(), "r" );
   if(fp == NULL) analysis::error(2, analysis::set_path( num_conf ).c_str());
   
   int    t_tmp;
   double tmp[10];
   for (int itt=0; itt<Ndata; itt++) {
      if ( fscanf(  fp, " %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf ", &t_tmp
                  , &tmp[0], &tmp[1], &tmp[2], &tmp[3], &tmp[4]
                  , &tmp[5], &tmp[6], &tmp[7], &tmp[8], &tmp[9] ) != 11 )
         analysis::error(3, "invalid reading for correlator file");
      if (itt != t_tmp) analysis::error(3, "read a different time slice");
      corr[t_tmp] = cdouble(tmp[0+momentum*2], tmp[1+momentum*2]);
   }
   fclose(fp);
   
   if(hadron.fb_mean_flg){   // Read anti-correlator files
      string anti_HAD_name;   anti_HAD_name = "anti" + hadron.name;
      analysis::set_data_list(HADRON_NAME, "%s", anti_HAD_name.c_str());
      
      FILE* fp = fopen( analysis::set_path( num_conf ).c_str(), "r" );
      if(fp == NULL) analysis::error(2, analysis::set_path( num_conf ).c_str());

      if ( fscanf(  fp, " %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf ", &t_tmp
                  , &tmp[0], &tmp[1], &tmp[2], &tmp[3], &tmp[4]
                  , &tmp[5], &tmp[6], &tmp[7], &tmp[8], &tmp[9] ) != 11 )
         analysis::error(3, "invalid reading for correlator file");
      if (0 != t_tmp) analysis::error(3, "read a different time slice");
      corr[0] = ( corr[0] + cdouble(tmp[0+momentum*2], tmp[1+momentum*2]) )*0.5;
      
      for (int itt=1; itt<Ndata; itt++) {
         if ( fscanf(  fp, " %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf ", &t_tmp
                     , &tmp[0], &tmp[1], &tmp[2], &tmp[3], &tmp[4]
                     , &tmp[5], &tmp[6], &tmp[7], &tmp[8], &tmp[9] ) != 11 )
            analysis::error(3, "invalid reading for correlator file");
         if (itt != t_tmp) analysis::error(3, "read a different time slice");
           corr[Ndata - t_tmp] =
         ( corr[Ndata - t_tmp] + cdouble(  tmp[0+momentum*2]
                                         , tmp[1+momentum*2]) )*0.5;
      }
      fclose(fp);
   }
   printf(" @ Finished input correlator : %s (%s), conf number=%d\n"
          , hadron.name.c_str(), CORRtype, num_conf );
   
   analysis::route( class_name, func_name, 0 );
}

void CORRELATOR::input(CHANNEL_TYPE channel, int num_conf, SPIN_TYPE spin) {
   
   func_name = "input_corr_multi______";
   analysis::route(class_name, func_name, 1);
   
   int spin_num = 999;
   if (spin.number == 999) analysis::error(3, "UNKNOWN spin type.");
   else if (spin.number == SPIN_0_0  ) spin_num = 0;
   else if (spin.number == SPIN_1_ave) spin_num = 1;
   else if (spin.number == SPIN_1_p1 ) spin_num = 2;
   else if (spin.number == SPIN_1_0  ) spin_num = 3;
   else if (spin.number == SPIN_1_m1 ) spin_num = 4;
   
   analysis::set_data_list(N_TIME,         "-1");
   analysis::set_data_list(HADRON_NAME,    "%s", channel.prop_name.c_str());
   analysis::set_data_list(CORR_DIRECTORY, "multi");
   
   FILE* fp = fopen( analysis::set_path( num_conf ).c_str(), "r" );
   if(fp == NULL) analysis::error(2, analysis::set_path( num_conf ).c_str());
   
   int    t_tmp;
   double tmp[10];
   for (int itt=0; itt<Ndata; itt++) {
      if ( fscanf(  fp, " %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf ", &t_tmp
                  , &tmp[0], &tmp[1], &tmp[2], &tmp[3], &tmp[4]
                  , &tmp[5], &tmp[6], &tmp[7], &tmp[8], &tmp[9] ) != 11 )
         analysis::error(3, "invalid reading for correlator file");
      if (itt != t_tmp) analysis::error(3, "read a different time slice");
      corr[t_tmp] = cdouble(tmp[0+spin_num*2], tmp[1+spin_num*2]);
   }
   fclose(fp);

   printf(" @ Finished input correlator : %s, conf number=%d\n"
          , channel.prop_name.c_str(), num_conf );
   
   analysis::route( class_name, func_name, 0 );
}
