//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Function for fitting datas
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <fitting.h>

void FITTING::fit_data( int cood_min, int cood_max, double STP_CND ){
    
    func_name = "fit_data______________";
    route( class_name, func_name, 1 );
    
    if( !new_flg_data )         error(1,"Fit data has not inputed yet !");
    else if( !new_flg_param )   error(1,"Fit parameter has not setted yet !");
    else{
        printf(" @ Fitting          |   0%%");
        stp_cnd = STP_CND;
        chisq_mean = 0.0;
        
        double chisq_tmp;
        double chisq_sqr_tmp = 0.0;
        double *param_tmp  = new double[ N_param ];
//=========================== fit corr-type data =========================//
        if( fit_type == 1 ){
            int tmp_dataSIZE = cood_max - cood_min + 1;
            double *cood_tmp = new double[ tmp_dataSIZE ];
            double *data_tmp = new double[ tmp_dataSIZE ];
            double *err_tmp  = new double[ tmp_dataSIZE ];
            
            for( int conf=0; conf<N_conf; conf++ ){
                
                for( int loop=0; loop<tmp_dataSIZE; loop++ ){
                    cood_tmp[loop] = cood[ loop+cood_min ];
                    data_tmp[loop] = data[ idx(conf,loop+cood_min) ];
                    err_tmp[loop]  =  err[ loop+cood_min ];
                }
                
                chisq_tmp
                = fitmrq( cood_tmp, data_tmp, err_tmp, param_tmp, tmp_dataSIZE );
                
                for( int loop=0; loop<N_param; loop++ )
                    param_out[ idx(conf,loop) ] = param_tmp[loop];
                chisq_mean      += chisq_tmp;
                chisq_sqr_tmp   += chisq_tmp * chisq_tmp;
                
                printf("\b\b\b\b%3.0lf%%",double(conf+1)/double(N_conf)*100);
                fflush(stdout);
            }
            delete [] cood_tmp;
            delete [] data_tmp;
            delete [] err_tmp;
//========================= fit potential-type data ======================//
        }else{
            for( int conf=0; conf<N_conf; conf++ ){
                double *cood_tmp = new double[ dataSIZE[conf] ];
                double *data_tmp = new double[ dataSIZE[conf] ];
                double *err_tmp  = new double[ dataSIZE[conf] ];
                
                for( int loop=0; loop<dataSIZE[conf]; loop++ ){
                    cood_tmp[loop] = cood[ idx(conf,loop) ];
                    data_tmp[loop] = data[ idx(conf,loop) ];
                    err_tmp[loop]  =  err[ idx(conf,loop) ];
                }
                
                chisq_tmp
                = fitmrq( cood_tmp, data_tmp, err_tmp, param_tmp, dataSIZE[conf] );
                
                for( int loop=0; loop<N_param; loop++ )
                    param_out[ idx(conf,loop) ] = param_tmp[loop];
                chisq_mean      += chisq_tmp;
                chisq_sqr_tmp   += chisq_tmp * chisq_tmp;
                
                delete [] cood_tmp;
                delete [] data_tmp;
                delete [] err_tmp;
                
                printf("\b\b\b\b%3.0lf%%",double(conf+1)/double(N_conf)*100);
                fflush(stdout);
            }
//===========================================================================//
        }
        delete [] param_tmp;
        printf("\n");
        fitting_flg = true;
        chisq_mean      /= double(N_conf);
        chisq_sqr_tmp   /= double(N_conf);
        chisq_err = sqrt( (chisq_sqr_tmp - pow(chisq_mean,2))/double(N_conf) );
        
        for( int loop=0; loop<N_param; loop++ ){
            param_mean[loop]    = 0.0;
            double sqr_tmp      = 0.0;
            for( int conf=0; conf<N_conf; conf++ ){
                param_mean[loop]+= param_out[ idx(conf,loop) ];
                sqr_tmp         += pow( param_out[ idx(conf,loop) ],2 );
            }
            param_mean[loop]/= double(N_conf);
            sqr_tmp         /= double(N_conf);
            
            param_err[loop]  = sqrt( (sqr_tmp - pow(param_mean[loop],2))
                                    * double(N_conf-1) );
        }
        printf(" @ Finished fitting : chisq/confs = %lf +/- %lf\n",
               chisq_mean, chisq_err);
    }
    route( class_name, func_name, 0 );
}
