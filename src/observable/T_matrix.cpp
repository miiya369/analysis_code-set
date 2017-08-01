//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup observable
 * @brief   Definition for the T-matrix class
 * @author  Takaya Miyamoto
 * @since   Mon Nov  7 16:14:42 JST 2016
 */
//--------------------------------------------------------------------------

#include <observable/Tmatrix.h>

//======================== Constructor & Destructor ======================//
T_MATRIX::T_MATRIX() {
   DEBUG_LOG
   
   m_data   = NULL;
   m_Energy = NULL;
   
   m_Ndata = 0;
   m_Nch   = 0;
}
T_MATRIX::T_MATRIX(const T_MATRIX& other) {
   DEBUG_LOG
   
   m_data   = NULL;
   m_Energy = NULL;
   mem_alloc(other.data_size(), other.Nch());
   
   for (int n=0; n<(*this).data_size(); n++) {
      (*this) (n) = other  (n);
      m_Energy[n] = other.E(n);
   }
}
T_MATRIX::T_MATRIX(  const double a_Emin, const double a_Emax
                   , const double a_Edev, const int a_Nch) {
   DEBUG_LOG
   
   m_data   = NULL;
   m_Energy = NULL;
   
   mem_alloc(a_Emin, a_Emax, a_Edev, a_Nch);
}
T_MATRIX::T_MATRIX(const double* a_Energy, const int a_Ndata, const int a_Nch) {
   DEBUG_LOG
   
   m_data   = NULL;
   m_Energy = NULL;
   
   mem_alloc(a_Energy, a_Ndata, a_Nch);
}
T_MATRIX::~T_MATRIX() {
   DEBUG_LOG
   
   if (m_data   != NULL) delete [] m_data;
   if (m_Energy != NULL) delete [] m_Energy;
}
//============================= For initialize ===========================//
void T_MATRIX::mem_alloc(const int a_Ndata, const int a_Nch) {
   DEBUG_LOG
   
   if (m_Ndata != a_Ndata || m_Nch != a_Nch) {
      mem_del();
      m_Ndata = a_Ndata;
      m_Nch   = a_Nch;
   }
   if (m_data   == NULL) m_data   = new cmatrix[m_Ndata];
   if (m_Energy == NULL) m_Energy = new double [m_Ndata];
   
   for (int n=0; n<(*this).data_size(); n++) m_data[n].init(m_Nch);
}
void T_MATRIX::mem_alloc(  const double a_Emin, const double a_Emax
                         , const double a_Edev, const int a_Nch ) {
   DEBUG_LOG
   
   int count = 0;
   for (double loop=a_Emin; loop<=a_Emax; loop+=a_Edev) count++;
   
   if (m_Ndata != count || m_Nch != a_Nch) {
      mem_del();
      m_Ndata = count;
      m_Nch   = a_Nch;
   }
   
   if (m_data   == NULL) m_data   = new cmatrix[m_Ndata];
   if (m_Energy == NULL) m_Energy = new double [m_Ndata];
   
   double tmp_E = a_Emin;
   for (int n=0; n<(*this).data_size(); n++) {
      m_data[n].init(m_Nch);
      m_Energy[n] = tmp_E;
      tmp_E += a_Edev;
   }
}
void T_MATRIX::mem_alloc(  const double* a_Energy, const int a_Ndata
                         , const int a_Nch ) {
   DEBUG_LOG
   
   if (m_Ndata != a_Ndata || m_Nch != a_Nch) {
      mem_del();
      m_Ndata = a_Ndata;
      m_Nch   = a_Nch;
   }
   if (m_data   == NULL) m_data   = new cmatrix[m_Ndata];
   if (m_Energy == NULL) m_Energy = new double [m_Ndata];
   
   for (int n=0; n<(*this).data_size(); n++) {
      m_data[n].init(m_Nch);
      m_Energy[n] = a_Energy[n];
   }
}
void T_MATRIX::mem_del() {
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
T_MATRIX& T_MATRIX::operator=(const T_MATRIX &rhs) {
   if ((*this).data_size() != rhs.data_size())
      ERROR_COMMENTS("Different data sizes. Cannot copy.");
   if ((*this).Nch() != rhs.Nch())
      ERROR_COMMENTS("Different #.channels. Cannot copy.");
   
   for (int n=0; n<(*this).data_size(); n++) {
      (*this) (n) = rhs  (n);
      m_Energy[n] = rhs.E(n);
   }
   return *this;
}
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
