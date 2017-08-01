//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup observable
 * @brief   Function for calculate phase shift
 * @author  Takaya Miyamoto
 * @since   Fri Nov 13 01:44:42 JST 2015
 */
//--------------------------------------------------------------------------

#include <observable/Tmatrix.h>

void T_MATRIX::calc_Tmat_int(  ) {
   
   ERROR_COMMENTS("Not implemented yet.");
   
//   DEBUG_LOG
//   
//   double *x     = new double[Ndev];
//   double *p     = new double[Ndev+1];
//   double *s     = new double[Ndev];
//   double *k_sqr = new double[Nchannel];
//   double *fac   = new double[Nchannel*Nchannel];
//   double  p_cut, dev = 2.0/double(Ndev);
//   
//#define idx(a,b) (b+Nchannel*(a))
//   for (int ch1=0; ch1<Nchannel; ch1++) for (int ch2=0; ch2<Nchannel; ch2++)
//      fac[idx(ch1,ch2)] = 2.0*mu[ch1] * lat_a/hbar_c;
//   
//   cdouble *Aij     = new cdouble[Nchannel*Nchannel*(Ndev+1)*(Ndev+1)];
//   cdouble *Aij_inv = new cdouble[Nchannel*Nchannel*(Ndev+1)*(Ndev+1)];
//   cdouble *ul      = new cdouble[Nchannel];
//   cdouble *un      = new cdouble[Nchannel];
//   cdouble *Tmat    = new cdouble[Nchannel*Nchannel*(Ndev+1)*(Ndev+1)];
//   cdouble *Vmat    = new cdouble[Nchannel*Nchannel*(Ndev+1)*(Ndev+1)];
//   
//#define mat(a,b,i,j) (b+Nchannel*(j+(Ndev+1)*(a+Nchannel*(i))))
//   for (int iE=0; iE<phase[0].data_size(); iE++) {
//      
//      for (int ch=0; ch<Nchannel; ch++)
//         k_sqr[ch] = (  2.0*mu[ch]*((phase[0].E(iE)+10e-12)-(mm_th[0]-mm_th[ch]))
//                      * pow(lat_a/hbar_c,2) );
//      p[Ndev] = sqrt(k_sqr[0]);   // p_{N+1} = on-shell momentum
//      
//      p_cut = p[Ndev] / tan((PI/4.0)*dev*((Ndev/2)+0.5));
//      for (int i=0; i<Ndev; i++) {   // p_cut < q < p_cut * tan[pi/4 * (1+dev)]
//         x[i] = i * dev;
//         p[i] = p_cut * tan((PI/4.0)*x[i]);
//         s[i] = p_cut * (PI/4.0) * dev / pow(cos((PI/4.0)*x[i]),2);
//      }
//      for (int ch=0; ch<Nchannel; ch++)
//         k_sqr[ch] >= 0 ? un[ch] = COMP_IMAG*PI*sqrt(k_sqr[ch]) / 2.0
//                        : un[ch] = COMP_ZERO;
//      
//      for (int i=0; i<Ndev+1;   i++)
//      for (int j=0; j<Ndev;     j++)
//      for (int a=0; a<Nchannel; a++)
//      for (int b=0; b<Nchannel; b++) {
//         Vmat[mat(a,b,i,j)]
//         = Vp(p[i], p[j], &aa[idx(a,b)*Nparam], Nparam, func_type)*fac[idx(a,b)];
//         Aij[mat(a,b,i,j)]
//         = (  (a==b && i==j ? 1.0 : 0.0)
//            + Vmat[mat(a,b,i,j)] * pow(p[j],2)*s[j]/(pow(p[j],2)-k_sqr[b]) );
//      }
//      for (int i=0; i<Ndev+1;   i++)
//      for (int a=0; a<Nchannel; a++)
//      for (int b=0; b<Nchannel; b++) {
//         Vmat[mat(a,b,i,Ndev)]
//         = Vp(p[i],p[Ndev],&aa[idx(a,b)*Nparam],Nparam,func_type)*fac[idx(a,b)];
//         Aij[mat(a,b,i,Ndev)] = (  (a==b && i==Ndev ? 1.0 : 0.0)
//                                 + Vmat[mat(a,b,i,Ndev)] * un[b] );
//      }
//      solve_mat_gauss(Aij, Tmat, Vmat, Nchannel*(Ndev+1));
//      
////      inv_mat_gauss(Aij, Aij_inv, Nchannel*(Ndev+1));   // Using inv. matrix
////      for (int i=0; i<Nchannel*Nchannel*(Ndev+1)*(Ndev+1); i++)
////         Tmat[i] = COMP_ZERO;
////      for (int a=0; a<Nchannel;          a++)
////      for (int b=0; b<Nchannel;          b++)
////      for (int c=0; c<Nchannel;          c++)
////      for (int l=0; l<Nchannel*(Ndev+1); l++)
////         Tmat[b+Nchannel*a] += Aij_inv[mat(a,c,Ndev,l)]*V_p(b+Nchannel*c,l,Ndev);
//      
/////////////////////////////   1 channel case   ///////////////////////////
//      if (Nchannel==1) {
//         phase[0](iE) = atan(  Tmat[mat(0,0,Ndev,Ndev)].imag()
//                             / Tmat[mat(0,0,Ndev,Ndev)].real() ) * 180/PI;
//         
////         cdouble tmpT = (  exp(COMP_IMAG*phase[0](iE)*PI/180.0)   // For Debug
////                         * sin(phase[0](iE).real()*PI/180.0) );
////         cdouble tmpS = 1.0 - COMP_IMAG * Tmat[mat(0,0,Ndev,Ndev)];
////         cdouble tmp_phase = log(tmpS) / (2.0*COMP_IMAG);
////         phase[0](iE) = tmp_phase * 180.0/PI;
////         if (iE==100)
////         printf("%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n"
////                , phase[0].E(iE), p[Ndev]
////                , Tmat[mat(0,0,Ndev,Ndev)].real()
////                , Tmat[mat(0,0,Ndev,Ndev)].imag()
////                , tmpT.real(), tmpT.imag(), tmpS.real(), tmpS.imag()
////                , abs(tmpS), tmp_phase.real(), tmp_phase.imag() );
//      }
/////////////////////////////   2 channel case   ///////////////////////////
//      else if (Nchannel==2) {
//         cdouble Smat11 = 1.0 - COMP_IMAG * Tmat[mat(0,0,Ndev,Ndev)];
//         cdouble Smat21 = 0.0 - COMP_IMAG * Tmat[mat(1,0,Ndev,Ndev)];
//         cdouble Smat12 = 0.0 - COMP_IMAG * Tmat[mat(0,1,Ndev,Ndev)];
//         cdouble Smat22 = 1.0 - COMP_IMAG * Tmat[mat(1,1,Ndev,Ndev)];
//         
//         cdouble tan2_SYM = (- (Smat12 * Smat21) / (Smat11 * Smat22) );
//         cdouble cos2_SYM = 1.0/sqrt(1.0+tan2_SYM);
//         cdouble del1_SYM = log(Smat11/cos2_SYM) / (2.0*COMP_IMAG) * 180.0/PI;
//         cdouble del2_SYM = log(Smat22/cos2_SYM) / (2.0*COMP_IMAG) * 180.0/PI;
//         
////         printf("%lf %lf %lf\n", phase[0].E(iE)
////                , tan2_SYM.real(), tan2_SYM.imag());   // For Debug
//
//         phase[0](iE) = del1_SYM;
//         phase[1](iE) = del2_SYM;
//      }
////////////////////////////////////////////////////////////////////////////
//   } // iE
//#undef idx
//#undef mat
//   delete [] x;
//   delete [] p;
//   delete [] s;
//   delete [] k_sqr;
//   delete [] fac;
//   delete [] Aij;
//   delete [] Aij_inv;
//   delete [] ul;
//   delete [] un;
//   delete [] Tmat;
//   delete [] Vmat;
}
