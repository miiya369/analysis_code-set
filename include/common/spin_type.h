//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Header File for definition of spin type
 * @author  Takaya Miyamoto
 * @since   Tue Jan 19 15:35:12 JST 2016
 */
//--------------------------------------------------------------------------

#ifndef SPIN_TYPE_H
#define SPIN_TYPE_H

//--------------------------------------------------------------------------
/**
 * @brief The class for spin type
 */
//--------------------------------------------------------------------------
class SPIN_TYPE
{
public:
   string name;
   int    number;

   SPIN_TYPE();
   SPIN_TYPE(const SPIN_TYPE&);
   SPIN_TYPE(const string);
   SPIN_TYPE(const char*);
   
   SPIN_TYPE(const int);
   ~SPIN_TYPE() {}
   
   void set(const char*);
   void set(const string);
   void set(const int);
};

#endif
