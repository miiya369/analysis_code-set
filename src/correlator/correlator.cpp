//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Correlator
 * @brief   Definition of some functions for the correlator class
 * @author  Takaya Miyamoto
 * @since   Mon Nov  7 15:13:52 JST 2016
 */
//--------------------------------------------------------------------------

#include <correlator/correlator.h>

//======================== Constructor & Destructor ======================//
CORRELATOR::CORRELATOR() {
   DEBUG_LOG
   
   corr  = NULL;
   Ndata = analysis::tSIZE;
}
CORRELATOR::CORRELATOR(const CORRELATOR& other) {
   DEBUG_LOG
   
   corr  = NULL;
   Ndata = other.data_size();
   mem_alloc();
   
   for (int n=0; n<Ndata; n++)
      (*this)(n) = other(n);
}
CORRELATOR::CORRELATOR(int N_DATA) {
   DEBUG_LOG
   
   corr  = NULL;
   Ndata = N_DATA;
}
CORRELATOR::CORRELATOR(  HADRON_TYPE HADRON, int iconf, int MOM
                       , const char* FILE_TYPE) {
   DEBUG_LOG
   
   corr  = NULL;
   Ndata = analysis::tSIZE;
   set(HADRON, iconf, MOM, FILE_TYPE);
}
CORRELATOR::CORRELATOR(CHANNEL_TYPE CHANNEL, int iconf, SPIN_TYPE SPIN) {
   DEBUG_LOG
   
   corr  = NULL;
   Ndata = analysis::tSIZE / 2;
   set(CHANNEL, iconf, SPIN);
}
CORRELATOR::~CORRELATOR() {
   DEBUG_LOG
   
   if (corr != NULL) delete [] corr;
}
//============================= For initialize ===========================//
void CORRELATOR::mem_alloc() {
   DEBUG_LOG
   
   if (corr == NULL) corr = new cdouble[Ndata];
}
void CORRELATOR::mem_alloc(int N_DATA) {
   DEBUG_LOG
   
   if (Ndata != N_DATA) {
      mem_del();
      Ndata = N_DATA;
   }
   if (corr == NULL) corr = new cdouble[Ndata];
}
void CORRELATOR::set(  HADRON_TYPE HADRON, int iconf, int MOM
                     , const char* FILE_TYPE) {
   DEBUG_LOG
   
   if (Ndata != analysis::tSIZE) {
      mem_del();
      Ndata = analysis::tSIZE;
   }
   mem_alloc();
   input(HADRON, iconf, MOM, FILE_TYPE);
}
void CORRELATOR::set(CHANNEL_TYPE CHANNEL, int iconf, SPIN_TYPE SPIN) {
   DEBUG_LOG
   
   if (Ndata != analysis::tSIZE / 2) {
      mem_del();
      Ndata = analysis::tSIZE / 2;
   }
   mem_alloc();
   input(CHANNEL, iconf, SPIN);
}
void CORRELATOR::mem_del() {
   DEBUG_LOG
   
   if (corr != NULL) {
      delete [] corr;
      corr = NULL;
   }
}
//============================ Operator define ===========================//
CORRELATOR& CORRELATOR::operator=(const CORRELATOR &rhs) {
   if ((*this).data_size() != rhs.data_size())
      ERROR_COMMENTS("Different data sizes. Cannot copy.");
   
   mem_alloc();
   
   for (int n=0; n<(*this).data_size(); n++)
      (*this)(n) = rhs(n);
   
   return *this;
}
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
