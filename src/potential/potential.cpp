//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Definition of some functions for the potential class
 * @author  Takaya Miyamoto
 * @since   Mon Nov  7 16:14:42 JST 2016
 */
//--------------------------------------------------------------------------

#include <potential/potential.h>

//======================== Constructor & Destructor ======================//
POTENTIAL::POTENTIAL() {
   DEBUG_LOG
   
   potential = NULL;
}
POTENTIAL::POTENTIAL(const POTENTIAL& other) {
   DEBUG_LOG
   
   potential = NULL;
   mem_alloc();
   
   for (int n=0; n<(*this).data_size(); n++)
      (*this)(n) = other(n);
}
POTENTIAL::POTENTIAL(const R_CORRELATOR &K_Rcorr, const R_CORRELATOR &Rcorr) {
   DEBUG_LOG
   
   potential = NULL;
   
   set(K_Rcorr, Rcorr);
}
POTENTIAL::~POTENTIAL() {
   DEBUG_LOG
   
   if (potential != NULL) delete [] potential;
}
//============================= For initialize ===========================//
void POTENTIAL::mem_alloc() {
   DEBUG_LOG
   
   if (potential == NULL) potential = new cdouble[(*this).data_size()];
}
void POTENTIAL::set(const R_CORRELATOR &K_Rcorr, const R_CORRELATOR &Rcorr) {
   DEBUG_LOG
   
   mem_alloc();
   input(K_Rcorr, Rcorr);
}
void POTENTIAL::set(const NBS_WAVE &K_NBS, const NBS_WAVE &NBS, const double E) {
   DEBUG_LOG
   
   mem_alloc();
   input(K_NBS, NBS, E);
}
void POTENTIAL::mem_del() {
   DEBUG_LOG
   
   if (potential != NULL) {
      delete [] potential;
      potential = NULL;
   }
}
//============================ Operator define ===========================//
POTENTIAL& POTENTIAL::operator=(const POTENTIAL &rhs) {
   mem_alloc();
   
   for (int n=0; n<(*this).data_size(); n++)
      (*this)(n) = rhs(n);
   
   return *this;
}
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
