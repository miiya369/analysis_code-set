//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Definition of the fit function type
 * @author  Takaya Miyamoto
 * @since   Sat Nov 12 03:33:54 JST 2016
 */
//--------------------------------------------------------------------------

#include <common/analysis.h>
#include <fitting/function_type.h>

FIT_FUNCTION::FIT_FUNCTION() {
   name         = "UNKNOWN";
   gnuplot_form = "UNKNOWN";
}
FIT_FUNCTION::FIT_FUNCTION(const FIT_FUNCTION& other) {
   name         = other.name;
   gnuplot_form = other.gnuplot_form;
}
FIT_FUNCTION::FIT_FUNCTION(const int func_num) {
   char func_name[16];
   snprintf(func_name, sizeof(func_name), "%d", func_num);
   set(func_name);
}
FIT_FUNCTION::FIT_FUNCTION(const string func_name) {
   set(func_name);
}
FIT_FUNCTION::FIT_FUNCTION(const char* func_name) {
   const string func_name_str(func_name);
   set(func_name_str);
}

void FIT_FUNCTION::set(const int func_num) {
   char func_name[16];
   snprintf(func_name, sizeof(func_name), "%d", func_num);
   set(func_name);
}
void FIT_FUNCTION::set(const char* func_name) {
   const string func_name_str(func_name);
   set(func_name_str);
}
//==================== definition of fit function type ===================//
void FIT_FUNCTION::set(const string func_name)
{
   if (func_name == "test" || func_name == "0")
   {
      name         = "Test";
      gnuplot_form = "NONE";
      number       = 0;
      Nparam       = 0;
   }
   else if (func_name == "const" || func_name == "1")
   {
      name         = "Constant";
      gnuplot_form = "%e";
      number       = 1;
      Nparam       = 1;
   }
   else if (func_name == "1EXP" || func_name == "2")
   {
      name         = "One exponential";
      gnuplot_form = "%e*exp(-(%e)*x)";
      number       = 2;
      Nparam       = 2;
   }
   else if (func_name == "1G" || func_name == "3")
   {
      name         = "One Gaussian";
      gnuplot_form = "%e*exp(-(x/(%e))**2)";
      number       = 3;
      Nparam       = 2;
   }
   else if (func_name == "2G" || func_name == "4")
   {
      name         = "Two Gaussians";
      gnuplot_form  = "%e*exp(-(x/(%e))**2)";
      gnuplot_form += "+(%e)*exp(-(x/(%e))**2)";
      number       = 4;
      Nparam       = 4;
   }
   else if (func_name == "3G" || func_name == "5")
   {
      name         = "Three Gaussians";
      gnuplot_form  = "%e*exp(-(x/(%e))**2)";
      gnuplot_form += "+(%e)*exp(-(x/(%e))**2)";
      gnuplot_form += "+(%e)*exp(-(x/(%e))**2)";
      number       = 5;
      Nparam       = 6;
   }
   else if (func_name == "1SG" || func_name == "6")
   {
      name         = "One x-shifted Gaussian";
      gnuplot_form = "%e*exp(-((x-(%e))/(%e))**2)";
      number       = 6;
      Nparam       = 3;
   }
   else if (func_name == "2SG" || func_name == "7")
   {
      name         = "Two x-shifted Gaussians";
      gnuplot_form  = "%e*exp(-((x-(%e))/(%e))**2)";
      gnuplot_form += "+(%e)*exp(-((x-(%e))/(%e))**2)";
      number       = 7;
      Nparam       = 6;
   }
   else if (func_name == "4G" || func_name == "8")
   {
      name         = "Four Gaussians";
      gnuplot_form  = "%e*exp(-(x/(%e))**2)";
      gnuplot_form += "+(%e)*exp(-(x/(%e))**2)";
      gnuplot_form += "+(%e)*exp(-(x/(%e))**2)";
      gnuplot_form += "+(%e)*exp(-(x/(%e))**2)";
      number       = 8;
      Nparam       = 8;
   }
   else if (func_name == "1G1Y" || func_name == "9")
   {
      name         = "One Gaussians and one Yukawa";
      gnuplot_form  = "%e*exp(-(x/(%e))**2)";
      gnuplot_form += "+(1-exp(-(x/(%e))**2))*exp(-((%e)*x))/((%e)*x)";
      number       = 9;
      Nparam       = 5;
   }
//---   ---//
   else
   {
      name         = "UNKNOWN";
      gnuplot_form = "UNKNOWN";
      number       = -1;
      Nparam       = 0;
   }
}
//========================================================================//
