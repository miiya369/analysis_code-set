//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Definition of the function for the fitting class
 * @author  Takaya Miyamoto
 * @since   Mon Nov  7 17:49:17 JST 2016
 */
//--------------------------------------------------------------------------

#include <fitting/fitting.h>

//======================== Constructor & Destructor ======================//
FIT::FIT() {
   DEBUG_LOG
   
   param = NULL;
}
FIT::FIT(const FIT& other) {
   DEBUG_LOG
   
   func_type.set(other.info_func_num());
   param = new double[func_type.Nparam];
   
   for (int n=0; n<func_type.Nparam; n++)
      (*this)(n) = other(n);
}
FIT::FIT(const char* FUNC_NAME, const double *PARAM) {
   DEBUG_LOG
   
   param = NULL;
   set_func(FUNC_NAME, PARAM);
}
FIT::FIT(const int FUNC_NUMBER, const double *PARAM) {
   DEBUG_LOG
   
   param = NULL;
   set_func(FUNC_NUMBER, PARAM);
}
FIT::~FIT() {
   DEBUG_LOG
   
   if (param != NULL) delete [] param;
}
//============================= For initialize ===========================//
   void FIT::set_func(const char* FUNC_NAME, const double *PARAM) {
      DEBUG_LOG
      
      func_type.set(FUNC_NAME);
      
      if (param != NULL) delete [] param;
      param = new double[func_type.Nparam];
      
//      printf(" @ fit function = %s\n", func_type.name.c_str());
      
      for (int loop=0; loop<func_type.Nparam; loop++)
         (*this)(loop) = PARAM[loop];
   }
   void FIT::set_func(const int FUNC_NUMBER, const double *PARAM) {
      char FUNC_NAME[16];
      snprintf(FUNC_NAME, sizeof(FUNC_NAME), "%d", FUNC_NUMBER);
      set_func(FUNC_NAME, PARAM);
   }
   void FIT::mem_del() {
      DEBUG_LOG
      
      if (param != NULL) {
         delete [] param;
         param = NULL;
      }
   }
//============================ Operator define ===========================//

//============================ Operator helper ===========================//

//=========================== Several functions ==========================//
