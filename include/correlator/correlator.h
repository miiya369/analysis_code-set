//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Correlator
 * @brief   Header file for the correlator class
 * @author  Takaya Miyamoto
 * @since   Fri Dec 18 00:21:10 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef CORRELATOR_H
#define CORRELATOR_H

#include <common/analysis.h>

//--------------------------------------------------------------------------
/**
 * @brief The class for the correlator
 */
//--------------------------------------------------------------------------
class CORRELATOR
{
private:
   cdouble *corr;
   int      Ndata;
   
protected:
   
public:
//============================ For inner index ===========================//

//============================== For writing =============================//
   cdouble& operator()(int index) {
      return corr[index];
   }
//============================== For reading =============================//
   const cdouble& operator()(int index) const {
      return corr[index];
   }
//======================== Constructor & Destructor ======================//
   CORRELATOR();
   CORRELATOR(const CORRELATOR&);
   CORRELATOR(int);
   CORRELATOR(HADRON_TYPE, int, int, const char*);
   CORRELATOR(CHANNEL_TYPE, int, SPIN_TYPE);
   
   ~CORRELATOR();
//============================= For initialize ===========================//
   void mem_alloc();
   void mem_alloc(int);
   void set(HADRON_TYPE, int, int, const char*);
   void set(CHANNEL_TYPE, int, SPIN_TYPE);
   void mem_del();
//============================ Operator define ===========================//
   CORRELATOR& operator=(const CORRELATOR&);
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
   int info_class() { return CLASS_CORRELATOR; }
   int data_size () { return Ndata; }
   const int info_class() const { return CLASS_CORRELATOR; }
   const int data_size () const { return Ndata; }
   
   void input(HADRON_TYPE,  int, int, const char*);
   void input(CHANNEL_TYPE, int, SPIN_TYPE);
};

namespace analysis {
   //! The function for data output
   template <>
   void output_data_all<CORRELATOR>(CONFIG<CORRELATOR>&, const char*);
   template <>
   void output_data_err<CORRELATOR>(CONFIG<CORRELATOR>&, const char*, bool);
   template <>
   void output_data_fit<CORRELATOR>(CONFIG<CORRELATOR>&, const char*, bool);
}

#endif
