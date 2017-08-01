//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Common header file for configuration template class
 * @author  Takaya Miyamoto
 * @since   Mon Nov  7 16:14:42 JST 2016
 */
//--------------------------------------------------------------------------

#ifndef CONFIG_TMP_H
#define CONFIG_TMP_H

#include <common/common_inc.h>

//--------------------------------------------------------------------------
/**
 * @brief The template class for configuration
 */
//--------------------------------------------------------------------------
template <class X> class CONFIG
{   
private:
   X   *m_data;
   int  m_Nconf;
   
protected:
   
public:
//============================ For inner index ===========================//
   
//============================== For writing =============================//
   X& operator()(int index) {
      return m_data[index];
   }
//============================== For reading =============================//
   const X& operator()(int index) const {
      return m_data[index];
   }
//======================== Constructor & Destructor ======================//
   CONFIG() {
      DEBUG_LOG
      
      m_data  = NULL;
      m_Nconf = analysis::Nconf;
      mem_alloc();
   }
   CONFIG(const CONFIG& other) {
      DEBUG_LOG
      
      m_data  = NULL;
      m_Nconf = other.Nconf();
      mem_alloc();
      
      for (int n=0; n<(*this).Nconf(); n++)
         (*this)(n) = other(n);
   }
   CONFIG(int a_Nconf) {
      DEBUG_LOG
      
      m_data  = NULL;
      m_Nconf = a_Nconf;
      mem_alloc();
   }
   ~CONFIG() {
      DEBUG_LOG
      
      if (m_data != NULL) delete [] m_data;
   }
//============================= For initialize ===========================//
   void mem_alloc() {
      DEBUG_LOG
      
      if (m_data == NULL) m_data = new X[m_Nconf];
   }
   void mem_alloc(int a_Nconf) {
      DEBUG_LOG
      
      if (m_Nconf != a_Nconf) {
         mem_del();
         m_Nconf = a_Nconf;
      }
      if (m_data == NULL) m_data = new X[m_Nconf];
   }
   void mem_del() {
      DEBUG_LOG
      
      if (m_data != NULL) {
         delete [] m_data;
         m_data = NULL;
      }
   }
//============================ Operator define ===========================//
   CONFIG& operator=(const CONFIG &rhs) {
      if ((*this).Nconf() != rhs.Nconf())
         ERROR_COMMENTS("Different data sizes. Cannot copy.");
      
      for (int n=0; n<(*this).Nconf(); n++)
         (*this)(n) = rhs(n);
      
      return *this;
   }
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
   int Nconf(){ return m_Nconf; }
   const int Nconf() const { return m_Nconf; }
   
   void make_JK_sample();
   void make_JK_sample(CONFIG<X>&);
   void make_mean_err( double*,  double*, bool);
   void make_mean_err(cdouble*, cdouble*, bool);
};

namespace analysis {
   //! The function for data output
   template <class X> void output_data_all(CONFIG<X>&, const char*);
   template <class X> void output_data_err(CONFIG<X>&, const char*, bool);
   template <class X> void output_data_fit(CONFIG<X>&, const char*, bool);
}

#include <common/make_jack_knife_sample.h>
#include <common/output_data.h>

#endif
