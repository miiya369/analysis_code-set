//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Function for data fitting, according to Numerical Recipe
 * @author  Takaya Miyamoto
 * @since   Sat Nov 12 03:33:54 JST 2016
 */
//--------------------------------------------------------------------------

#include <fitting/fitting.h>
#include <fitting/fit_function_NR.h>

//--------------------------------------------------------------------------
/**
 * @brief The fit for CORRELATOR-TYPE data
 */
//--------------------------------------------------------------------------
double FIT::fit_data_NR(  const double *cood, const double *data, const double *err
                        , const int Ndata, const int cood_min, const int cood_max
                        , const double STP_CND )
{
   int idx_min = -1, idx_max = -1;
   for (int n=0; n<Ndata; n++) {
      if (cood[n] >= cood_min && idx_min == -1) idx_min = n;
      if (cood[n] >= cood_max && idx_max == -1) idx_max = n;
   }
   int n_data = idx_max - idx_min + 1;
   
   return FIT::fit_data_NR(  &cood[idx_min], &data[idx_min], &err[idx_min]
                           , n_data, STP_CND );
}
double FIT::fit_data_NR(  const double *cood, const double *data, const double *err
                        , const int Ndata, const double cood_min
                        , const double cood_max, const double STP_CND )
{
   int idx_min = -999, idx_max = -999;
   for (int n=0; n<Ndata; n++) {
      if (cood[n] >= cood_min && idx_min == -999) idx_min = n;
      if (cood[n] >= cood_max && idx_max == -999) idx_max = n;
   }
   int n_data = idx_max - idx_min + 1;
   
   return FIT::fit_data_NR(  &cood[idx_min], &data[idx_min], &err[idx_min]
                           , n_data, STP_CND );
}

//--------------------------------------------------------------------------
/**
 * @brief The fit for POTENTIAL-TYPE data
 */
//--------------------------------------------------------------------------
double FIT::fit_data_NR(  const double *cood, const double *data, const double *err
                        , const int Ndata, const double STP_CND ) {
   DEBUG_LOG
   
   if (param == NULL)
   {
      WORNING_COMMENTS("Fit function has not set yet !");
      return -999.0;
   }
   Doub    stp_cnd = STP_CND;
   VecDoub PARAM(func_type.Nparam);
   VecDoub COOD(Ndata);
   VecDoub DATA(Ndata);
   VecDoub  ERR(Ndata);
   
   for (int n=0; n<func_type.Nparam; n++)
      PARAM[n] = param[n];
   for (int loop=0; loop<Ndata; loop++)
   {
      COOD[loop] = cood[loop];
      DATA[loop] = data[loop];
       ERR[loop] =  err[loop];
   }
   double chisq = 0.0;
//======================== Selection of fit functions ====================//
      if (func_type.number == 1 )
      {
         Fitmrq fit_NR(COOD, DATA, ERR, PARAM, &fit_funcs::func_const, stp_cnd);
         if (fit_NR.fit() == -1) return -999.0;
         for (int n=0; n<func_type.Nparam; n++)
            param[n] = fit_NR.a[n];
         chisq = fit_NR.chisq / (Ndata-func_type.Nparam);
      }
      else if (func_type.number == 2 )
      {
         Fitmrq fit_NR(COOD, DATA, ERR, PARAM, &fit_funcs::func_exp, stp_cnd);
         if (fit_NR.fit() == -1) return -999.0;
         for (int n=0; n<func_type.Nparam; n++)
            param[n] = fit_NR.a[n];
         chisq = fit_NR.chisq / (Ndata-func_type.Nparam);
      }
      else if (func_type.number == 3 ||
               func_type.number == 4 ||
               func_type.number == 5 ||
               func_type.number == 8 )
      {
         Fitmrq fit_NR(COOD, DATA, ERR, PARAM, &fit_funcs::func_gauss, stp_cnd);
         if (fit_NR.fit() == -1) return -999.0;
         for (int n=0; n<func_type.Nparam; n++)
            param[n] = fit_NR.a[n];
         chisq = fit_NR.chisq / (Ndata-func_type.Nparam);
      }
      else if (func_type.number == 6 ||
               func_type.number == 7 )
      {
         Fitmrq fit_NR(COOD, DATA, ERR, PARAM, &fit_funcs::func_sgauss, stp_cnd);
         if (fit_NR.fit() == -1) return -999.0;
         for (int n=0; n<func_type.Nparam; n++)
            param[n] = fit_NR.a[n];
         chisq = fit_NR.chisq / (Ndata-func_type.Nparam);
      }
      else if (func_type.number == 9)
      {
         Fitmrq fit_NR(COOD, DATA, ERR, PARAM, &fit_funcs::func_1g1y, stp_cnd);
         if (fit_NR.fit() == -1) return -999.0;
         for (int n=0; n<func_type.Nparam; n++)
            param[n] = fit_NR.a[n];
         chisq = fit_NR.chisq / (Ndata-func_type.Nparam);
      }
//========================================================================//
   return chisq;
}
