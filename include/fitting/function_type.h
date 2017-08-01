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

   FIT_FUNCTION();
   FIT_FUNCTION(const FIT_FUNCTION&);
   FIT_FUNCTION(const int);
   FIT_FUNCTION(const string);
   FIT_FUNCTION(const char*);
   ~FIT_FUNCTION() {}
   
   void set(const int);
   void set(const char*);
//==================== definition of fit function type ===================//
   void set(const string);
//========================================================================//
};

#endif
