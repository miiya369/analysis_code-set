//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Header File for definition of fit function type
 * @author  Takaya Miyamoto
 * @since   Sun Jun 19 03:26:49 JST 2016
 */
//--------------------------------------------------------------------------

#ifndef FUNCTION_TYPE_CONST_H
#define FUNCTION_TYPE_CONST_H

//--------------------------------------------------------------------------
/**
 * @brief The class for fit function type
 */
//--------------------------------------------------------------------------
class FIT_FUNCTION
{
public:
   string name;
   string gnuplot_form;
   int    number;
   int    Nparam;

   FIT_FUNCTION()
   {
      name         = "UNKNOWN";
      gnuplot_form = "UNKNOWN";
   }
   FIT_FUNCTION(const FIT_FUNCTION& other)
   {
      name         = other.name;
      gnuplot_form = other.gnuplot_form;
   }
   FIT_FUNCTION(const int func_num)
   {
      char func_name[16];
      snprintf(func_name, sizeof(func_name), "%d", func_num);
      set(func_name);
   }
   FIT_FUNCTION(const string func_name)
   {
      set(func_name);
   }
   FIT_FUNCTION(const char* func_name)
   {
      const string func_name_str(func_name);
      set(func_name_str);
   }
   ~FIT_FUNCTION() {}
   
   void set(const int func_num)
   {
      char func_name[16];
      snprintf(func_name, sizeof(func_name), "%d", func_num);
      set(func_name);
   }
   void set(const char* func_name)
   {
      const string func_name_str(func_name);
      set(func_name_str);
   }
//==================== definition of fit function type ===================//
   void set(const string func_name)
   {
      if (func_name == "test" || func_name == "0")
      {
         name         = "test";
         gnuplot_form = "NONE";
         number       = 0;
         Nparam       = 0;
      }
      else if (func_name == "const" || func_name == "1")
      {
         name         = "constant";
         gnuplot_form = "%lf";
         number       = 1;
         Nparam       = 1;
      }
      else if (func_name == "1EXP" || func_name == "2")
      {
         name         = "one exponential";
         gnuplot_form = "%lf*exp(-(%lf)*x)";
         number       = 2;
         Nparam       = 2;
      }
      else if (func_name == "1G" || func_name == "3")
      {
         name         = "one gaussian";
         gnuplot_form = "%lf*exp(-(x/(%lf))**2)";
         number       = 3;
         Nparam       = 2;
      }
      else if (func_name == "2G" || func_name == "4")
      {
         name         = "two gaussians";
         gnuplot_form  = "%lf*exp(-(x/(%lf))**2)";
         gnuplot_form += "+(%lf)*exp(-(x/(%lf))**2)";
         number       = 4;
         Nparam       = 4;
      }
      else if (func_name == "3G" || func_name == "5")
      {
         name         = "three gaussians";
         gnuplot_form  = "%lf*exp(-(x/(%lf))**2)";
         gnuplot_form += "+(%lf)*exp(-(x/(%lf))**2)";
         gnuplot_form += "+(%lf)*exp(-(x/(%lf))**2)";
         number       = 5;
         Nparam       = 6;
      }
      else if (func_name == "1SG" || func_name == "6")
      {
         name         = "one x-shifted gaussian";
         gnuplot_form = "%lf*exp(-((x-(%lf))/(%lf))**2)";
         number       = 6;
         Nparam       = 3;
      }
      else if (func_name == "2SG" || func_name == "7")
      {
         name         = "two x-shifted gaussians";
         gnuplot_form  = "%lf*exp(-((x-(%lf))/(%lf))**2)";
         gnuplot_form += "+(%lf)*exp(-((x-(%lf))/(%lf))**2)";
         number       = 7;
         Nparam       = 6;
      }
      else if (func_name == "4G" || func_name == "8")
      {
         name         = "four gaussians";
         gnuplot_form  = "%lf*exp(-(x/(%lf))**2)";
         gnuplot_form += "+(%lf)*exp(-(x/(%lf))**2)";
         gnuplot_form += "+(%lf)*exp(-(x/(%lf))**2)";
         gnuplot_form += "+(%lf)*exp(-(x/(%lf))**2)";
         number       = 8;
         Nparam       = 8;
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
};

#endif
