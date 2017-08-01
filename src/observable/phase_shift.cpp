//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup phase shift
 * @brief   Definition of some functions for the phase shift class
 * @author  Takaya Miyamoto
 * @since   Mon Nov  7 16:14:42 JST 2016
 */
//--------------------------------------------------------------------------

#include <observable/phase_shift.h>

//======================== Constructor & Destructor ======================//
PHASE_SHIFT::PHASE_SHIFT() {
   DEBUG_LOG
   
   m_data   = NULL;
   m_Energy = NULL;
   
   m_Ndata = 0;
}
PHASE_SHIFT::PHASE_SHIFT(const PHASE_SHIFT& other) {
   DEBUG_LOG
   
   m_data   = NULL;
   m_Energy = NULL;
   mem_alloc(other.data_size());
   
   for (int n=0; n<(*this).data_size(); n++) {
      (*this) (n) = other  (n);
      m_Energy[n] = other.E(n);
   }
}
PHASE_SHIFT::PHASE_SHIFT(const T_MATRIX& a_Tmat) {
   DEBUG_LOG
   
   m_data   = NULL;
   m_Energy = NULL;
   
   mem_alloc(a_Tmat);
}
PHASE_SHIFT::PHASE_SHIFT(  const double a_Emin, const double a_Emax
                         , const double a_Edev ) {
   DEBUG_LOG
   
   m_data   = NULL;
   m_Energy = NULL;
   
   mem_alloc(a_Emin, a_Emax, a_Edev);
}
PHASE_SHIFT::PHASE_SHIFT(const double* a_Energy, const int a_Ndata) {
   DEBUG_LOG
   
   m_data   = NULL;
   m_Energy = NULL;
   
   mem_alloc(a_Energy, a_Ndata);
}
PHASE_SHIFT::~PHASE_SHIFT() {
   DEBUG_LOG
   
   if (m_data   != NULL) delete [] m_data;
   if (m_Energy != NULL) delete [] m_Energy;
}
//============================= For initialize ===========================//
void PHASE_SHIFT::mem_alloc(const int a_Ndata) {
   DEBUG_LOG
   
   if (m_Ndata != a_Ndata) {
      mem_del();
      m_Ndata = a_Ndata;
   }
   if (m_data   == NULL) m_data   = new double[m_Ndata];
   if (m_Energy == NULL) m_Energy = new double[m_Ndata];
}
void PHASE_SHIFT::mem_alloc(const T_MATRIX& a_Tmat) {
   DEBUG_LOG
   
   if (m_Ndata != a_Tmat.data_size()) {
      mem_del();
      m_Ndata = a_Tmat.data_size();
   }
   if (m_data   == NULL) m_data   = new double[m_Ndata];
   if (m_Energy == NULL) m_Energy = new double[m_Ndata];
   
   for (int n=0; n<(*this).data_size(); n++) m_Energy[n] = a_Tmat.E(n);
}
void PHASE_SHIFT::mem_alloc(  const double a_Emin, const double a_Emax
                            , const double a_Edev ) {
   DEBUG_LOG
   
   int count = 0;
   for (double loop=a_Emin; loop<=a_Emax; loop+=a_Edev) count++;
   
   if (m_Ndata != count) {
      mem_del();
      m_Ndata = count;
   }
   
   if (m_data   == NULL) m_data   = new double[m_Ndata];
   if (m_Energy == NULL) m_Energy = new double[m_Ndata];
   
   double tmp_E = a_Emin;
   for (int n=0; n<(*this).data_size(); n++) {
      m_Energy[n] = tmp_E;
      tmp_E += a_Edev;
   }
}
void PHASE_SHIFT::mem_alloc(const double* a_Energy, const int a_Ndata) {
   DEBUG_LOG
   
   if (m_Ndata != a_Ndata) {
      mem_del();
      m_Ndata = a_Ndata;
   }
   if (m_data   == NULL) m_data   = new double[m_Ndata];
   if (m_Energy == NULL) m_Energy = new double[m_Ndata];
   
   for (int n=0; n<(*this).data_size(); n++) m_Energy[n] = a_Energy[n];
}
void PHASE_SHIFT::mem_del() {
   DEBUG_LOG
   
   if (m_data != NULL) {
      delete [] m_data;
      m_data = NULL;
   }
   if (m_Energy != NULL) {
      delete [] m_Energy;
      m_Energy = NULL;
   }
}
//============================ Operator define ===========================//
PHASE_SHIFT& PHASE_SHIFT::operator=(const PHASE_SHIFT &rhs) {
   if ((*this).data_size() != rhs.data_size())
      ERROR_COMMENTS("Different data sizes. Cannot copy.");
   
   for (int n=0; n<(*this).data_size(); n++) {
      (*this) (n) = rhs  (n);
      m_Energy[n] = rhs.E(n);
   }
   return *this;
}
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
