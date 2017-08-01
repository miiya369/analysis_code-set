//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup phase shift
 * @brief   Function for calculate scattering length
 * @author  Takaya Miyamoto
 * @since   Tue Jul  5 04:55:58 JST 2016
 */
//--------------------------------------------------------------------------

#include <observable/Tmatrix.h>

double observable::calc_scatt_length(  const double *a_param, const int a_Nparam
                                     , const int a_func_type, const double a_energy
                                     , const double a_lat_space, const double a_mu
                                     , const int a_ang_mom
                                     , const double a_del, const double a_r_max ) {
   T_MATRIX Tmat(a_energy, a_energy, 1, 1);
   double tmp = 0.0;
   
   Tmat.calc_Tmat_dif(  a_param, &a_mu, &tmp, &a_Nparam, &a_func_type
                      , a_lat_space, a_ang_mom, a_del, a_r_max );
   
   cdouble phase = log(Tmat.Smat(0)(0)) / (2.0*COMP_IMAG) * 180.0/PI;
   
   return -tan(phase.real()*PI/180.0) * hbar_c / sqrt(2.0*a_energy*a_mu);
}

double observable::calc_scatt_length_fpot(  const DENSITY_TYPE_BASE &a_density
                                          , const double *a_param
                                          , const int a_Nparam
                                          , const int a_func_type
                                          , const double a_energy
                                          , const double a_lat_space
                                          , const double a_mu
                                          , const double a_del
                                          , const double a_r_max
                                          , const double a_Ndiv ) {
   T_MATRIX Tmat(a_energy, a_energy, 1, 1);
   
   Tmat.calc_Tmat_fpot(  a_density, a_param, a_mu, a_Nparam, a_func_type
                       , a_lat_space, a_del, a_r_max, a_Ndiv );
   
   cdouble phase = log(Tmat.Smat(0)(0)) / (2.0*COMP_IMAG) * 180.0/PI;
   
   return -tan(phase.real()*PI/180.0) * hbar_c / sqrt(2.0*a_energy*a_mu);
}
