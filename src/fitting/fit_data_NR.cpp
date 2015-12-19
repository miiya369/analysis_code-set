//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Function for data fitting, according to Numerical Recipe
 * @author  Takaya Miyamoto
 * @since   Wed Sep  2 23:41:46 JST 2015
 */
//--------------------------------------------------------------------------

#include <fitting/fitting.h>
#include <fitting/fit_NR.h>

//--------------------------------------------------------------------------
/**
 * @brief The fit for CORRELATOR-TYPE data
 */
//--------------------------------------------------------------------------
double FIT::fit_data_NR(  double *cood, double *data, double *err
                        , int cood_min, int cood_max, double STP_CND ) {
   
   int n_data = cood_max - cood_min + 1;
   
   return FIT::fit_data_NR(  &cood[cood_min], &data[cood_min], &err[cood_min]
                           , n_data, STP_CND );
}

//--------------------------------------------------------------------------
/**
 * @brief The fit for POTENTIAL-TYPE data
 */
//--------------------------------------------------------------------------
double FIT::fit_data_NR(  double *cood, double *data, double *err
                        , int Ndata, double STP_CND ) {
    
   func_name = "fit_data_NR___________";
   analysis::route(class_name, func_name, 1);
   
   if (param == NULL) {
      analysis::error(1,"Fit function has not set yet !");
      analysis::route(class_name, func_name, 0);
      return 0.0;
   }
   Doub    stp_cnd = STP_CND;
   VecDoub PARAM(func_type.Nparam);
   VecDoub COOD(Ndata);
   VecDoub DATA(Ndata);
   VecDoub  ERR(Ndata);
   
   for (int n=0; n<func_type.Nparam; n++)
      PARAM[n] = param[n];
   for (int loop=0; loop<Ndata; loop++) {
      COOD[loop] = cood[loop];
      DATA[loop] = data[loop];
       ERR[loop] =  err[loop];
   }
   double chisq = 0.0;
//======================== Selection of fit functions ====================//
      if        (func_type.number == 1 ) {
         Fitmrq fit_NR(COOD, DATA, ERR, PARAM, &fit_funcs::func_const, stp_cnd);
         fit_NR.fit();
         for (int n=0; n<func_type.Nparam; n++)
            param[n] = fit_NR.a[n];
         chisq = fit_NR.chisq / (Ndata-func_type.Nparam);
      
      } else if (func_type.number == 2 ) {
         Fitmrq fit_NR(COOD, DATA, ERR, PARAM, &fit_funcs::func_exp, stp_cnd);
         fit_NR.fit();
         for (int n=0; n<func_type.Nparam; n++)
            param[n] = fit_NR.a[n];
         chisq = fit_NR.chisq / (Ndata-func_type.Nparam);
      
      } else if (func_type.number == 3 ||
                 func_type.number == 4 ||
                 func_type.number == 5 ) {
         Fitmrq fit_NR(COOD, DATA, ERR, PARAM, &fit_funcs::func_gauss, stp_cnd);
         fit_NR.fit();
         for (int n=0; n<func_type.Nparam; n++)
            param[n] = fit_NR.a[n];
         chisq = fit_NR.chisq / (Ndata-func_type.Nparam);
      
      } else if (func_type.number == 6 ||
                 func_type.number == 7 ) {
         Fitmrq fit_NR(COOD, DATA, ERR, PARAM, &fit_funcs::func_sgauss, stp_cnd);
         fit_NR.fit();
         for (int n=0; n<func_type.Nparam; n++)
            param[n] = fit_NR.a[n];
         chisq = fit_NR.chisq / (Ndata-func_type.Nparam);
      }
//========================================================================//
   analysis::route(class_name, func_name, 0);
   return chisq;
}
