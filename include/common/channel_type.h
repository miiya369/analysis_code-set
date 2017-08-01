//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Header File for definition of channel type
 * @author  Takaya Miyamoto
 * @since   Wed Sep 14 16:51:04 JST 2016
 */
//--------------------------------------------------------------------------

#ifndef CHANNEL_TYPE_H
#define CHANNEL_TYPE_H

//--------------------------------------------------------------------------
/**
 * @brief The class for channel type of NBS wave function
 */
//--------------------------------------------------------------------------
class CHANNEL_TYPE
{
public:
   string name;
   string prop_name;
   string directory;
   string OperType;
   HADRON_TYPE hadron1;
   HADRON_TYPE hadron2;
   bool   flg_spin_proj; // true->projected, false->not projected
   string spin_name;

   CHANNEL_TYPE();
   CHANNEL_TYPE(const CHANNEL_TYPE&);
   CHANNEL_TYPE(const string);
   CHANNEL_TYPE(const char*);
   ~CHANNEL_TYPE() {}
   
   void set(const char*);
   void set(const string);
};

#endif
