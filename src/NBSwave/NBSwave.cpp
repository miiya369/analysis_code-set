//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBS wave function
 * @brief   Definition of some functions for the NBS wave function
 * @author  Takaya Miyamoto
 * @since   Mon Nov  7 19:51:54 JST 2016
 */
//--------------------------------------------------------------------------

#include <NBSwave/NBSwave.h>

//--------------------------------------------------------------------------
/**
 * @brief Definition of some functions for the original NBS wave function class
 */
//--------------------------------------------------------------------------

//======================== Constructor & Destructor ======================//
NBS_WAVE_ORG::NBS_WAVE_ORG() {
   DEBUG_LOG
   
   wave_org = NULL;
}
NBS_WAVE_ORG::NBS_WAVE_ORG(const NBS_WAVE_ORG& other) {
   DEBUG_LOG
   
   wave_org = NULL;
   mem_alloc();
   
   for (int n=0; n<(*this).data_size(); n++)
      (*this)(n) = other(n);
}
NBS_WAVE_ORG::NBS_WAVE_ORG(const char* PATH) {
   DEBUG_LOG
   
   wave_org = NULL;
   
   set(PATH);
}
NBS_WAVE_ORG::NBS_WAVE_ORG(const CHANNEL_TYPE ch, const int it, const int iconf) {
   DEBUG_LOG
   
   wave_org = NULL;
   
   set(ch, it, iconf);
}
NBS_WAVE_ORG::~NBS_WAVE_ORG() {
   if (wave_org != NULL) delete [] wave_org;
}
//============================= For initialize ===========================//
void NBS_WAVE_ORG::mem_alloc() {
   DEBUG_LOG
   
   if (wave_org == NULL) wave_org = new cdouble[(*this).data_size()];
}
void NBS_WAVE_ORG::set(const char* PATH) {
   DEBUG_LOG
   
   mem_alloc();
   
   input_compressed_FromPath(PATH);
}
void NBS_WAVE_ORG::set(const CHANNEL_TYPE ch, const int it, const int iconf) {
   DEBUG_LOG
   
   mem_alloc();
   
   input_compressed(ch, it, iconf);
}
void NBS_WAVE_ORG::mem_del() {
   DEBUG_LOG
   
   if (wave_org != NULL) {
      delete [] wave_org;
      wave_org = NULL;
   }
}
//============================ Operator define ===========================//
NBS_WAVE_ORG& NBS_WAVE_ORG::operator=(const NBS_WAVE_ORG &rhs) {
   mem_alloc();
   
   for (int n=0; n<(*this).data_size(); n++)
      (*this)(n) = rhs(n);
   
   return *this;
}
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//


//--------------------------------------------------------------------------
/**
 * @brief Definition of some functions for the src-spin projected NBS wave function
*/
//--------------------------------------------------------------------------

//======================== Constructor & Destructor ======================//
NBS_WAVE_SRC_PRJ::NBS_WAVE_SRC_PRJ() {
   DEBUG_LOG
   
   wave_src_prj = NULL;
}
NBS_WAVE_SRC_PRJ::NBS_WAVE_SRC_PRJ(const NBS_WAVE_SRC_PRJ& other) {
   DEBUG_LOG
   
   wave_src_prj = NULL;
   mem_alloc();
   
   for (int n=0; n<(*this).data_size(); n++)
      (*this)(n) = other(n);
}
NBS_WAVE_SRC_PRJ::~NBS_WAVE_SRC_PRJ() {
   DEBUG_LOG
   
   if (wave_src_prj != NULL) delete [] wave_src_prj;
}
//============================= For initialize ===========================//
void NBS_WAVE_SRC_PRJ::mem_alloc() {
   DEBUG_LOG
   
   if (wave_src_prj == NULL) wave_src_prj = new cdouble[(*this).data_size()];
}
void NBS_WAVE_SRC_PRJ::mem_del() {
   DEBUG_LOG
   
   if (wave_src_prj != NULL) {
      delete [] wave_src_prj;
      wave_src_prj = NULL;
   }
}
//============================ Operator define ===========================//
NBS_WAVE_SRC_PRJ& NBS_WAVE_SRC_PRJ::operator=(const NBS_WAVE_SRC_PRJ &rhs) {
   mem_alloc();
   
   for (int n=0; n<(*this).data_size(); n++)
      (*this)(n) = rhs(n);
   
   return *this;
}
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//


//--------------------------------------------------------------------------
/**
 * @brief Definition of some functions for the spin projected NBS wave function
 */
//--------------------------------------------------------------------------

//======================== Constructor & Destructor ======================//
NBS_WAVE::NBS_WAVE() {
   DEBUG_LOG
   
   wave = NULL;
}
NBS_WAVE::NBS_WAVE(const NBS_WAVE& other) {
   DEBUG_LOG
   
   wave = NULL;
   mem_alloc();
   
   for (int n=0; n<(*this).data_size(); n++)
      (*this)(n) = other(n);
}
NBS_WAVE::NBS_WAVE(const char* PATH) {
   DEBUG_LOG
   
   wave = NULL;
   
   set(PATH);
}
NBS_WAVE::NBS_WAVE(const CHANNEL_TYPE ch, const int it, const int iconf) {
   DEBUG_LOG
   
   wave = NULL;
   
   set(ch, it, iconf);
}
NBS_WAVE::~NBS_WAVE() {
   DEBUG_LOG
   
   if (wave != NULL) delete [] wave;
}
//============================= For initialize ===========================//
void NBS_WAVE::mem_alloc() {
   DEBUG_LOG
   
   if (wave == NULL) wave = new cdouble[(*this).data_size()];
}
void NBS_WAVE::set(const char* PATH) {
   DEBUG_LOG
   
   mem_alloc();
   
   input_FromPath(PATH);
}
void NBS_WAVE::set(const CHANNEL_TYPE ch, const int it, const int iconf) {
   DEBUG_LOG
   
   mem_alloc();
   
   input(ch, it, iconf);
}
void NBS_WAVE::mem_del() {
   DEBUG_LOG
   
   if (wave != NULL) {
      delete [] wave;
      wave = NULL;
   }
}
//============================ Operator define ===========================//
NBS_WAVE& NBS_WAVE::operator=(const NBS_WAVE &rhs) {
   mem_alloc();
   
   for (int n=0; n<(*this).data_size(); n++)
      (*this)(n) = rhs(n);
   
   return *this;
}
//============================ Operator helper ===========================//
    
//=========================== Several functions ==========================//
