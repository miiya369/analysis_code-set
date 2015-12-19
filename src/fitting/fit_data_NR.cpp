//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Function for data fitting, according to Numerical Recipe
 * @author  Takaya Miyamoto
 * @since   Thu Jul 23 19:39:30 JST 2015
 */
//--------------------------------------------------------------------------

#include <fitting/fitting.h>
#include <fitting/fit_NR.h>

void FIT::fit_data_NR( int cood_min, int cood_max, double STP_CND ) {
    
   func_name = "fit_data_NR___________";
   analysis::route(class_name, func_name, 1);
   
   if      (!new_flg_data) {
      analysis::error(1,"Fit data has not input yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   else if (!new_flg_param) {
      analysis::error(1,"Fit parameter has not set yet !");
      analysis::route(class_name, func_name, 0);
      return;
   }
   chisq_mean           = 0.0;
   double chisq_sqr_tmp = 0.0;
   int tmp_Ndata        = 0;
   int data_shift       = 0;
   
   Doub stp_cnd = STP_CND;
   VecDoub PARAM(func_type.Nparam);
   for (int n=0; n<func_type.Nparam; n++)
      PARAM[n] = param_ini[n];
//=========================== fit corr-type data =========================//
   if (fit_type == 1) {
      tmp_Ndata  = cood_max - cood_min + 1;
      data_shift = cood_min;
//========================= fit potential-type data ======================//
   } else if (fit_type == 2) {
      tmp_Ndata  = Ndata;
      data_shift = 0;
//========================================================================//
   }
   VecDoub COOD(tmp_Ndata);
   VecDoub DATA(tmp_Ndata);
   VecDoub  ERR(tmp_Ndata);
   
   printf(" @ Fitting          |   0%%");
   for (int conf=0; conf<analysis::Nconf; conf++) {
      
      for (int loop=0; loop<tmp_Ndata; loop++) {
         COOD[loop] = cood[loop+data_shift];
         DATA[loop] = data[idx(conf,loop+data_shift)];
          ERR[loop] =  err[loop+data_shift];
      }
//======================== Selection of fit functions ====================//
      if      (func_type.number == 1 ) {
         Fitmrq fit_NR(COOD, DATA, ERR, PARAM, &fit_funcs::func_const, stp_cnd);
         fit_NR.fit();
         for (int loop=0; loop<func_type.Nparam; loop++)
            param_out[idx(conf,loop)] = fit_NR.a[loop];
         chisq_mean      +=     fit_NR.chisq / (tmp_Ndata-func_type.Nparam);
         chisq_sqr_tmp   += pow(fit_NR.chisq / (tmp_Ndata-func_type.Nparam), 2);
      
      } else if (func_type.number == 2 ) {
         Fitmrq fit_NR(COOD, DATA, ERR, PARAM, &fit_funcs::func_exp, stp_cnd);
         fit_NR.fit();
         for (int loop=0; loop<func_type.Nparam; loop++)
            param_out[idx(conf,loop)] = fit_NR.a[loop];
         chisq_mean      +=     fit_NR.chisq / (tmp_Ndata-func_type.Nparam);
         chisq_sqr_tmp   += pow(fit_NR.chisq / (tmp_Ndata-func_type.Nparam), 2);
      
      } else if (func_type.number == 3 ||
                 func_type.number == 4 ||
                 func_type.number == 5 ) {
         Fitmrq fit_NR(COOD, DATA, ERR, PARAM, &fit_funcs::func_gauss, stp_cnd);
         fit_NR.fit();
         for (int loop=0; loop<func_type.Nparam; loop++)
            param_out[idx(conf,loop)] = fit_NR.a[loop];
         chisq_mean      +=     fit_NR.chisq / (tmp_Ndata-func_type.Nparam);
         chisq_sqr_tmp   += pow(fit_NR.chisq / (tmp_Ndata-func_type.Nparam), 2);
      
      } else if (func_type.number == 6 ||
                 func_type.number == 7 ) {
         Fitmrq fit_NR(COOD, DATA, ERR, PARAM, &fit_funcs::func_sgauss, stp_cnd);
         fit_NR.fit();
         for (int loop=0; loop<func_type.Nparam; loop++)
            param_out[idx(conf,loop)] = fit_NR.a[loop];
         chisq_mean      +=     fit_NR.chisq / (tmp_Ndata-func_type.Nparam);
         chisq_sqr_tmp   += pow(fit_NR.chisq / (tmp_Ndata-func_type.Nparam), 2);
      }
//========================================================================//
      printf("\b\b\b\b%3.0lf%%",double(conf+1)/double(analysis::Nconf)*100);
      fflush(stdout);
   }
   printf("\n");
   fitting_flg = true;
   chisq_mean    /= double(analysis::Nconf);
   chisq_sqr_tmp /= double(analysis::Nconf);
   chisq_err = sqrt((chisq_sqr_tmp - pow(chisq_mean,2))/double(analysis::Nconf));
   
   for (int loop=0; loop<func_type.Nparam; loop++) {
      param_mean[loop] = 0.0;
      double sqr_tmp   = 0.0;
      for (int conf=0; conf<analysis::Nconf; conf++) {
         param_mean[loop] += param_out[idx(conf,loop)];
         sqr_tmp          += pow(param_out[idx(conf,loop)], 2);
      }
      param_mean[loop] /= double(analysis::Nconf);
      sqr_tmp          /= double(analysis::Nconf);
      param_err[loop] = sqrt( (sqr_tmp - pow(param_mean[loop], 2))
                             * double(analysis::Nconf-1) );
   }
   printf(" @ Finished fitting : chisq/confs = %lf +/- %lf\n",
          chisq_mean, chisq_err);
   
   analysis::route(class_name, func_name, 0);
}
