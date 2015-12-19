//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Print the function with gnuplot-format
 * @author  Takaya Miyamoto
 * @since   Wed Jul 22 09:15:44 JST 2015
 */
//--------------------------------------------------------------------------

#include <fitting/fitting.h>

void FIT::print_func_gnu( bool ini_flg ) {
   
   func_name = "print_func_gnu________";
   analysis::route(class_name, func_name, 1);
   
   if (!new_flg_param) {
      analysis::error(1,"Fit parameter has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   double* param = NULL;
   
   if (ini_flg) {
      param = param_ini;
      printf(" @ For gnuplot function ( Initial parameter )\n");
   } else {
      param = param_mean;
      printf(" @ For gnuplot function ( Fitted parameter )\n");
   }
   
   printf(" @ ");
   
   if (     func_type.Nparam == 1)
      printf(func_type.gnuplot_form.c_str(), param[0]);
   
   else if (func_type.Nparam == 2)
      printf(func_type.gnuplot_form.c_str(), param[0], param[1]);
   
   else if (func_type.Nparam == 3)
      printf(func_type.gnuplot_form.c_str(), param[0], param[1], param[2]);
   
   else if (func_type.Nparam == 4)
      printf(func_type.gnuplot_form.c_str(), param[0], param[1]
                                           , param[2], param[3]);
   
   else if (func_type.Nparam == 5)
      printf(func_type.gnuplot_form.c_str(), param[0], param[1], param[2]
                                           , param[3], param[4]);
   
   else if (func_type.Nparam == 6)
      printf(func_type.gnuplot_form.c_str(), param[0], param[1], param[2]
                                           , param[3], param[4], param[5]);
   
   else if (func_type.Nparam == 7)
      printf(func_type.gnuplot_form.c_str(), param[0], param[1], param[2]
                                           , param[3], param[4]
                                           , param[5], param[6]);
   
   else if (func_type.Nparam == 8)
      printf(func_type.gnuplot_form.c_str(), param[0], param[1], param[2]
                                           , param[3], param[4], param[5]
                                           , param[6], param[7]);
   
   else if (func_type.Nparam == 9)
      printf(func_type.gnuplot_form.c_str(), param[0], param[1], param[2]
                                           , param[3], param[4], param[5]
                                           , param[6], param[7], param[8]);
   else printf(" @ UNKOWN \n");
   
   printf("\n");

   analysis::route(class_name, func_name, 0);
}
