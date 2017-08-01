//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup R-correlator
 * @brief   Definition of some functions for the R-correlator class
 * @author  Takaya Miyamoto
 * @since   Mon Nov  7 19:51:54 JST 2016
 */
//--------------------------------------------------------------------------

#include <R_correlator/R_correlator.h>

//--------------------------------------------------------------------------
/**
 * @brief Definition of some functions for the src-spin projected R-correlator
 */
//--------------------------------------------------------------------------

//======================== Constructor & Destructor ======================//
R_CORRELATOR_SRC_PRJ::R_CORRELATOR_SRC_PRJ() {
   DEBUG_LOG
   
   Rcorr_src_prj = NULL;
}
R_CORRELATOR_SRC_PRJ::R_CORRELATOR_SRC_PRJ(const R_CORRELATOR_SRC_PRJ& other) {
   DEBUG_LOG
   
   Rcorr_src_prj = NULL;
   mem_alloc();
   
   for (int n=0; n<(*this).data_size(); n++)
      (*this)(n) = other(n);
}
R_CORRELATOR_SRC_PRJ::R_CORRELATOR_SRC_PRJ(  const NBS_WAVE_SRC_PRJ &nbs
                                           , const CORRELATOR &corr1
                                           , const CORRELATOR &corr2
                                           , const int time_slice ) {
   DEBUG_LOG
   
   Rcorr_src_prj = NULL;
   
   set(nbs, corr1, corr2, time_slice);
}
R_CORRELATOR_SRC_PRJ::R_CORRELATOR_SRC_PRJ(  const NBS_WAVE_SRC_PRJ &nbs
                                           , const double HAD1_mass
                                           , const double HAD2_mass
                                           , const int time_slice ) {
   DEBUG_LOG
   
   Rcorr_src_prj = NULL;
   
   set(nbs, HAD1_mass, HAD2_mass, time_slice);
}
R_CORRELATOR_SRC_PRJ::~R_CORRELATOR_SRC_PRJ() {
   DEBUG_LOG
   
   if (Rcorr_src_prj != NULL) delete [] Rcorr_src_prj;
}
//============================= For initialize ===========================//
void R_CORRELATOR_SRC_PRJ::mem_alloc() {
   DEBUG_LOG
   
   if (Rcorr_src_prj == NULL) Rcorr_src_prj = new cdouble[(*this).data_size()];
}
void R_CORRELATOR_SRC_PRJ::set(  const NBS_WAVE_SRC_PRJ &nbs
                               , const CORRELATOR &corr1
                               , const CORRELATOR &corr2, const int time_slice ) {
   DEBUG_LOG
   
   mem_alloc();
   input(nbs, corr1, corr2, time_slice);
}
void R_CORRELATOR_SRC_PRJ::set(  const NBS_WAVE_SRC_PRJ &nbs
                               , const double HAD1_mass
                               , const double HAD2_mass, int time_slice ) {
   DEBUG_LOG
   
   mem_alloc();
   input(nbs, HAD1_mass, HAD2_mass, time_slice);
}
void R_CORRELATOR_SRC_PRJ::mem_del() {
   DEBUG_LOG
   
   if (Rcorr_src_prj != NULL) {
      delete [] Rcorr_src_prj;
      Rcorr_src_prj = NULL;
   }
}
//============================ Operator define ===========================//
R_CORRELATOR_SRC_PRJ& R_CORRELATOR_SRC_PRJ::operator=(const R_CORRELATOR_SRC_PRJ
                                                      &rhs ) {
   mem_alloc();
   
   for (int n=0; n<(*this).data_size(); n++)
      (*this)(n) = rhs(n);
   
   return *this;
}
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//


//--------------------------------------------------------------------------
/**
 * @brief Definition of some functions for the spin projected R-correlator
 */
//--------------------------------------------------------------------------

//======================== Constructor & Destructor ======================//
R_CORRELATOR::R_CORRELATOR() {
   DEBUG_LOG
   
   Rcorr = NULL;
}
R_CORRELATOR::R_CORRELATOR(const R_CORRELATOR& other) {
   DEBUG_LOG
   
   Rcorr = NULL;
   mem_alloc();
   
   for (int n=0; n<(*this).data_size(); n++)
      (*this)(n) = other(n);
}
R_CORRELATOR::R_CORRELATOR(  const NBS_WAVE &nbs, const CORRELATOR &corr1
                           , const CORRELATOR &corr2, const int time_slice ) {
   DEBUG_LOG
   
   Rcorr = NULL;
   
   set(nbs, corr1, corr2, time_slice);
}
R_CORRELATOR::R_CORRELATOR(  const NBS_WAVE &nbs, const double HAD1_mass
                           , const double HAD2_mass, const int time_slice ) {
   DEBUG_LOG
   
   Rcorr = NULL;
   
   set(nbs, HAD1_mass, HAD2_mass, time_slice);
}
R_CORRELATOR::~R_CORRELATOR() {
   DEBUG_LOG
   
   if (Rcorr != NULL) delete [] Rcorr;
}
//============================= For initialize ===========================//
void R_CORRELATOR::mem_alloc() {
   DEBUG_LOG
   
   if (Rcorr == NULL) Rcorr = new cdouble[(*this).data_size()];
}
void R_CORRELATOR::set(  const NBS_WAVE &nbs, const CORRELATOR &corr1
                       , const CORRELATOR &corr2, const int time_slice ) {
   DEBUG_LOG
   
   mem_alloc();
   input(nbs, corr1, corr2, time_slice);
}
void R_CORRELATOR::set(  const NBS_WAVE &nbs, const double HAD1_mass
                       , const double HAD2_mass, const int time_slice ) {
   DEBUG_LOG
   
   mem_alloc();
   input(nbs, HAD1_mass, HAD2_mass, time_slice);
}
void R_CORRELATOR::mem_del() {
   DEBUG_LOG
   
   if (Rcorr != NULL) {
      delete [] Rcorr;
      Rcorr = NULL;
   }
}
//============================ Operator define ===========================//
R_CORRELATOR& R_CORRELATOR::operator=(const R_CORRELATOR &rhs) {
   mem_alloc();
   
   for (int n=0; n<(*this).data_size(); n++)
      (*this)(n) = rhs(n);
   
   return *this;
}
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
