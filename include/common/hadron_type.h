//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Header File for definition of hadron type
 * @author  Takaya Miyamoto
 * @since   Thu Sep 15 00:10:27 JST 2016
 */
//--------------------------------------------------------------------------

#ifndef HADRON_NAME_H
#define HADRON_NAME_H

//--------------------------------------------------------------------------
/**
 * @brief The class for hadron type
 */
//--------------------------------------------------------------------------
class HADRON_TYPE
{
public:
   string name, name_only;
   bool   fb_mean_flg;

   HADRON_TYPE();
   HADRON_TYPE(const HADRON_TYPE&);
   HADRON_TYPE(const string);
   HADRON_TYPE(const char*);
   ~HADRON_TYPE() {}
   
   void set(const char*);
   void set(const string);
};

#endif
