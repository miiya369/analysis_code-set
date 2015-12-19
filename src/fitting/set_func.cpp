//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Function for setting the fit function
 * @author  Takaya Miyamoto
 * @since   Wed Jul 22 21:40:01 JST 2015
 */
//--------------------------------------------------------------------------

#include <fitting/fitting.h>

void FIT::set_func( const char* FUNC_NAME, double *PARAM, int N_param ) {
   
   func_name = "set_func______________";
   analysis::route(class_name, func_name, 1);
   
   if (new_flg_param) {
      reset_func();
      func_name = "set_func______________";
   }
   
   func_type.set(FUNC_NAME);
   if (N_param != func_type.Nparam)
      analysis::error(3,"The number of fit parameter is invalid !");
   
   printf(" @ fit function     = %s\n", func_type.name.c_str());
   
   param_ini   = new double[func_type.Nparam];
   param_out   = new double[func_type.Nparam * analysis::Nconf];
   param_err   = new double[func_type.Nparam];
   param_mean  = new double[func_type.Nparam];
   new_flg_param = true;
   
   for (int loop=0; loop<func_type.Nparam; loop++)
      param_ini[loop] = PARAM[loop];
   
   analysis::route(class_name, func_name, 0);
}
