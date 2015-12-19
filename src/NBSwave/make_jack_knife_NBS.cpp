//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBS wave function
 * @brief   Make jack-knife samples
 * @author  Takaya Miyamoto
 * @since   Mon Jul 20 12:49:18 JST 2015
 */
//--------------------------------------------------------------------------

#include <NBSwave/NBSwave.h>

//--------------------------------------------------------------------------
/**
 * @brief Make Jack-Knife sample for original (before spin proj.) NBS wave func.
 */
//--------------------------------------------------------------------------
void NBS_WAVE::make_JK_sample_orgNBS( int bin ) {
    
   func_name = "make_JK_sample_orgNBS_";
   analysis::route(class_name, func_name, 1);
    
   if (orgNBS == NULL) {
      analysis::error(1, "NBS wave function has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   cdouble *tmp;   tmp = new cdouble[NBSwave::NBSnSIZE];
   
#ifdef _OPENMP
#pragma omp parallel for
#endif
   for (size_t loop=0; loop<NBSwave::NBSSIZE; loop++) {
      cdouble sum;
        
      for (int i=0; i<analysis::Nconf; i++)
         sum += orgNBS[loop + NBSwave::NBSSIZE * i];
      for (int i=0; i<analysis::Nconf; i++)
         tmp[loop + NBSwave::NBSSIZE * i]
            = ( sum - orgNBS[loop + NBSwave::NBSSIZE * i] )
               / double(analysis::Nconf-1);
   }
   for (size_t n=0; n<NBSwave::NBSnSIZE; n++) orgNBS[n]=tmp[n];
   delete [] tmp;
    
   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Make Jack-Knife sample for spin projected NBS wave func.
 */
//--------------------------------------------------------------------------
void NBS_WAVE::make_JK_sample_projNBS( int bin ) {
   
   func_name = "make_JK_sample_projNBS";
   analysis::route(class_name, func_name, 1);
   
   if (projNBS == NULL) {
      analysis::error(1, "NBS wave function has not projected yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   cdouble *tmp;   tmp = new cdouble[NBSwave::xyznSIZE];
   
#ifdef _OPENMP
#pragma omp parallel for
#endif
   for (size_t loop=0; loop<NBSwave::xyzSIZE; loop++) {
      cdouble sum;
      
      for (int i=0; i<analysis::Nconf; i++)
         sum += projNBS[loop + NBSwave::xyzSIZE * i];
      for (int i=0; i<analysis::Nconf; i++)
         tmp[loop + NBSwave::xyzSIZE * i]
         = ( sum - projNBS[loop + NBSwave::xyzSIZE * i] )
         / double(analysis::Nconf-1);
   }
   for (size_t n=0; n<NBSwave::xyznSIZE; n++) projNBS[n]=tmp[n];
   delete [] tmp;
   
   analysis::route(class_name, func_name, 0);
}
