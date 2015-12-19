//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup phase shift
 * @brief   Function for calculate phase shift
 * @author  Takaya Miyamoto
 * @since   Fri Nov 13 01:44:42 JST 2015
 */
//--------------------------------------------------------------------------

#include <observable/phase_shift.h>
#include <stdlib.h>
#include <time.h>   // for random initialize
#include <gauge/matrix_func.h>

void observable::calc_phase_shift_dif(  PHASE_SHIFT *phase, double *aa, double *mu
                                      , double *mass_threshold
                                      , int Nchannel, int Nparam, int func_type
                                      , double lat_space, double del
                                      , double r_max ) {
   
   string class_name = "________________________________";
   string func_name  = "calc_phase_shift______";
   analysis::route(class_name, func_name, 1);
   
   srand((unsigned int)time(NULL));   // for random initialize
   
   double hc_api = hbar_c / lat_space;
   
   double  *E        = new  double[Nchannel];
   double  *delta_E  = new  double[Nchannel];
   double  *v        = new  double[Nchannel*Nchannel];
   cdouble *kk       = new cdouble[Nchannel];
   cdouble *phi_diff = new cdouble[Nchannel*Nchannel];
   cdouble *Coe      = new cdouble[Nchannel*Nchannel];
   cdouble *XY       = new cdouble[Nchannel*Nchannel];
   cdouble *Smat     = new cdouble[Nchannel*Nchannel];
   cdouble *phi[3];
   for (int i=0; i<3; i++) phi[i] = new cdouble[Nchannel*Nchannel];
   
   for (int ch=1; ch<Nchannel; ch++)
      delta_E[ch] = mass_threshold[ch] - mass_threshold[0];
   delta_E[0] = 0.0;
   
#define mat(R,C)  (C + Nchannel * R)
   for (size_t iE=0; iE<phase[0].data_size(); iE++) {
      for (int ch=0; ch<Nchannel; ch++) E[ch] = phase[0].E(iE) - delta_E[ch];
      for (int ch=0; ch<Nchannel; ch++)
         kk[ch] = sqrt( cdouble(2.0*mu[ch]*E[ch]) ) / hbar_c;
      
      for (int ch=0; ch<Nchannel; ch++) for (int src=0; src<Nchannel; src++) {
         phi[0][mat(ch,src)] = COMP_ZERO;   // initialize of wave function
//         if (ch==src) phi[1][mat(ch,src)] = cdouble(del,del);
//         else         phi[1][mat(ch,src)] = COMP_ZERO;
         phi[1][mat(ch,src)]   // for random initialize
         = cdouble(del*rand()/(double)RAND_MAX, del*rand()/(double)RAND_MAX);
      }
      for (double r=del; r<=r_max; r+=del) {
         
         for (int i=0; i<Nchannel*Nchannel; i++)
            v[i] = V(r/lat_space, &aa[i*Nparam], Nparam, func_type) * hc_api;
         
         for (int ch=0; ch<Nchannel; ch++) for (int src=0; src<Nchannel; src++) {
            cdouble Vphi;
            for (int k=0; k<Nchannel; k++) Vphi += v[mat(ch,k)]*phi[1][mat(k,src)];
            phi[2][mat(ch,src)] = ( (2*mu[ch]) * pow(del/hbar_c,2)
                                  * ( Vphi - E[ch] * phi[1][mat(ch,src)] )
                                  + phi[1][mat(ch,src)]*2.0-phi[0][mat(ch,src)] );
         }
         for (int i=0; i<Nchannel*Nchannel; i++) {
            phi_diff[i] = (phi[2][i]-phi[0][i])/(2.0*del);
            phi[0][i]   =  phi[1][i];
            phi[1][i]   =  phi[2][i];
         }
         
//         printf("%lf %lf %lf %lf %lf\n", r   // For Debug of wave functions
//                , phi[0][mat(0,0)].real(), phi[0][mat(0,0)].imag()
//                , phi[0][mat(1,0)].real(), phi[0][mat(1,0)].imag() );
         
//         for( int ch=0; ch<Nchannel; ch++) for (int src=0; src<Nchannel; src++) {
//            Coe[mat(ch,src)] = ( (  phi_diff[mat(ch,src)]
//                                  - phi[0][  mat(ch,src)] * kk[ch] * COMP_IMAG )
//                                / 2.0 * exp(COMP_IMAG*kk[ch]*r) );
//            XY[mat(ch,src)]  = ( (  phi_diff[mat(ch,src)]
//                                  + phi[0][  mat(ch,src)] * kk[ch] * COMP_IMAG )
//                                / 2.0 * exp(-COMP_IMAG*kk[ch]*r) );
//         }
//         printf("%lf %lf %lf %lf %lf %lf %lf %lf %lf\n", r
//                , Coe[mat(0,0)].real(), Coe[mat(0,0)].imag()   // For Debug
//                , Coe[mat(1,0)].real(), Coe[mat(1,0)].imag()   // of Coe & XY
//                , XY[mat(0,0)].real(), XY[mat(0,0)].imag()
//                , XY[mat(1,0)].real(), XY[mat(1,0)].imag() );
      } // r
      
      for (int ch=0; ch<Nchannel; ch++) for (int src=0; src<Nchannel; src++) {
         Coe[mat(ch,src)] = ( (  phi_diff[mat(ch,src)]
                               - phi[0][  mat(ch,src)] * kk[ch] * COMP_IMAG )
                             / 2.0 * exp(COMP_IMAG*kk[ch]*r_max) );
         XY[mat(ch,src)]  = ( (  phi_diff[mat(ch,src)]
                               + phi[0][  mat(ch,src)] * kk[ch] * COMP_IMAG )
                             / 2.0 * exp(-COMP_IMAG*kk[ch]*r_max) );
      }
///////////////////////////   1 channel case   ///////////////////////////
      if (Nchannel==1) {
         Smat[0] = XY[0] / Coe[0];
         phase[0](iE) = log(Smat[0]) / (2.0*COMP_IMAG) * 180.0/PI;
      }
///////////////////////////   2 channel case   ///////////////////////////
      else if (Nchannel==2) {
         cdouble det = (  Coe[mat(0,0)] * Coe[mat(1,1)]
                        - Coe[mat(0,1)] * Coe[mat(1,0)] );
         cdouble Coe_inv[4];
         Coe_inv[mat(0,0)] =  Coe[mat(1,1)] / det;
         Coe_inv[mat(1,1)] =  Coe[mat(0,0)] / det;
         Coe_inv[mat(0,1)] = -Coe[mat(0,1)] / det;
         Coe_inv[mat(1,0)] = -Coe[mat(1,0)] / det;
         
         for (int i=0; i<Nchannel; i++) for (int j=0; j<Nchannel; j++) {
            Smat[mat(i,j)] = COMP_ZERO;
            for (int k=0; k<Nchannel; k++)
               Smat[mat(i,j)] += XY[mat(i,k)] * Coe_inv[mat(k,j)];
         }
         
//         cdouble Sdet = (  Smat[mat(0,0)] * Smat[mat(1,1)]
//                         - Smat[mat(0,1)] * Smat[mat(1,0)] );
//         cdouble Smat_inv[4],Smat_dag[4],Coe_tmp[4];
//         Smat_inv[mat(0,0)] =  Smat[mat(1,1)] / Sdet;
//         Smat_inv[mat(1,1)] =  Smat[mat(0,0)] / Sdet;
//         Smat_inv[mat(0,1)] = -Smat[mat(0,1)] / Sdet;   // For Debug of S matrix
//         Smat_inv[mat(1,0)] = -Smat[mat(1,0)] / Sdet;
//         Smat_dag[mat(0,0)] =  conj(Smat[mat(0,0)]);
//         Smat_dag[mat(0,1)] =  conj(Smat[mat(0,1)]);
//         Smat_dag[mat(1,0)] =  conj(Smat[mat(1,0)]);
//         Smat_dag[mat(1,1)] =  conj(Smat[mat(1,1)]);
//         for (int i=0; i<Nchannel; i++) for (int j=0; j<Nchannel; j++) {
//            for (int k=0; k<Nchannel; k++)
//               Coe_tmp[mat(i,j)] += Smat[mat(i,k)] * Smat_dag[mat(k,j)];
//         }
//         printf("(%lf %lf) (%lf %lf)\n",Coe_tmp[0].real(),Coe_tmp[0].imag()
//                ,Coe_tmp[1].real(),Coe_tmp[1].imag());
//         printf("(%lf %lf) (%lf %lf)\n\n",Coe_tmp[2].real(),Coe_tmp[2].imag()
//                ,Coe_tmp[3].real(),Coe_tmp[3].imag());
      
         
//         cdouble tan2_BB = (  (Smat[mat(1,0)] + Smat[mat(0,1)])
//                            / (Smat[mat(0,0)] - Smat[mat(1,1)]) );   // For BB
//         cdouble sin2_BB = tan2_BB / sqrt(1.0+tan2_BB*tan2_BB);
//         cdouble A_BB    =  Smat[mat(0,0)] + Smat[mat(1,1)];
//         cdouble B_BB    = (Smat[mat(1,0)] + Smat[mat(0,1)]) / sin2_BB;
//         cdouble del1_BB = log( (A_BB+B_BB)/2.0 ) / (2.0*COMP_IMAG) * 180.0/PI;
//         cdouble del2_BB = log( (A_BB-B_BB)/2.0 ) / (2.0*COMP_IMAG) * 180.0/PI;
         
         cdouble tan2_SYM = (- (Smat[mat(1,0)] * Smat[mat(0,1)])
                             / (Smat[mat(0,0)] * Smat[mat(1,1)]) );   // For SYM
         cdouble cos2_SYM = 1.0/sqrt(1.0+tan2_SYM);
         cdouble del1_SYM = log(Smat[mat(0,0)]/cos2_SYM)/(2.0*COMP_IMAG)*180.0/PI;
         cdouble del2_SYM = log(Smat[mat(1,1)]/cos2_SYM)/(2.0*COMP_IMAG)*180.0/PI;
         
//         printf("%lf %lf %lf %lf %lf\n",phase[0].E(iE)   // For Debug
//                , tan2_BB.real(), tan2_BB.imag()
//                , tan2_SYM.real(),tan2_SYM.imag());
         
//         double A_SYMtoBB = (del1_SYM+del2_SYM).real() * PI/180.0;
//         double B_SYMtoBB = acos(  cos2_SYM.real()
//                                 * cos((del1_SYM-del2_SYM).real() * PI/180.0) );
//         double del1_SYMtoBB = (A_SYMtoBB + B_SYMtoBB) * 0.5 * 180.0/PI;
//         double del2_SYMtoBB = (A_SYMtoBB - B_SYMtoBB) * 0.5 * 180.0/PI;
         
         phase[0](iE) = del1_SYM;
         phase[1](iE) = del2_SYM;
      }
//////////////////////////////////////////////////////////////////////////
   } // iE
#undef mat
   
   delete [] E;
   delete [] delta_E;
   delete [] kk;
   delete [] v;
   for (int loop=0; loop<3; loop++) delete [] phi[loop];
   delete [] phi_diff;
   delete [] Coe;
   delete [] XY;
   delete [] Smat;
   
   analysis::route(class_name, func_name, 0);//exit(0);
}

void observable::calc_phase_shift_int(  PHASE_SHIFT *phase, double *aa, double *mu
                                      , double *mm_th
                                      , int Nchannel, int Nparam, int func_type
                                      , double lat_a, int Ndev ) {
   
   string class_name = "________________________________";
   string func_name  = "calc_phase_shift_int__";
   analysis::route(class_name, func_name, 1);
   
   double *x     = new double[Ndev];
   double *p     = new double[Ndev+1];
   double *s     = new double[Ndev];
   double *k_sqr = new double[Nchannel];
   double *fac   = new double[Nchannel*Nchannel];
   double  p_cut, dev = 2.0/double(Ndev);
   
#define idx(a,b) (b+Nchannel*(a))
   for (int ch1=0; ch1<Nchannel; ch1++) for (int ch2=0; ch2<Nchannel; ch2++)
      fac[idx(ch1,ch2)] = 2.0*mu[ch1] * lat_a/hbar_c;
   
   cdouble *Aij     = new cdouble[Nchannel*Nchannel*(Ndev+1)*(Ndev+1)];
   cdouble *Aij_inv = new cdouble[Nchannel*Nchannel*(Ndev+1)*(Ndev+1)];
   cdouble *ul      = new cdouble[Nchannel];
   cdouble *un      = new cdouble[Nchannel];
   cdouble *Tmat    = new cdouble[Nchannel*Nchannel*(Ndev+1)*(Ndev+1)];
   cdouble *Vmat    = new cdouble[Nchannel*Nchannel*(Ndev+1)*(Ndev+1)];
   
#define mat(a,b,i,j) (b+Nchannel*(j+(Ndev+1)*(a+Nchannel*(i))))
   for (size_t iE=0; iE<phase[0].data_size(); iE++) {
      
      for (int ch=0; ch<Nchannel; ch++)
         k_sqr[ch] = (  2.0*mu[ch]*((phase[0].E(iE)+10e-12)-(mm_th[0]-mm_th[ch]))
                      * pow(lat_a/hbar_c,2) );
      p[Ndev] = sqrt(k_sqr[0]);   // p_{N+1} = on-shell momentum
      
      p_cut = p[Ndev] / tan((PI/4.0)*dev*((Ndev/2)+0.5));
      for (int i=0; i<Ndev; i++) {   // p_cut < q < p_cut * tan[pi/4 * (1+dev)]
         x[i] = i * dev;
         p[i] = p_cut * tan((PI/4.0)*x[i]);
         s[i] = p_cut * (PI/4.0) * dev / pow(cos((PI/4.0)*x[i]),2);
      }
      for (int ch=0; ch<Nchannel; ch++)
         k_sqr[ch] >= 0 ? un[ch] = COMP_IMAG*PI*sqrt(k_sqr[ch]) / 2.0
                        : un[ch] = COMP_ZERO;
      
      for (int i=0; i<Ndev+1;   i++)
      for (int j=0; j<Ndev;     j++)
      for (int a=0; a<Nchannel; a++)
      for (int b=0; b<Nchannel; b++) {
         Vmat[mat(a,b,i,j)]
         = Vp(p[i], p[j], &aa[idx(a,b)*Nparam], Nparam, func_type)*fac[idx(a,b)];
         Aij[mat(a,b,i,j)]
         = (  (a==b && i==j ? 1.0 : 0.0)
            + Vmat[mat(a,b,i,j)] * pow(p[j],2)*s[j]/(pow(p[j],2)-k_sqr[b]) );
      }
      for (int i=0; i<Ndev+1;   i++)
      for (int a=0; a<Nchannel; a++)
      for (int b=0; b<Nchannel; b++) {
         Vmat[mat(a,b,i,Ndev)]
         = Vp(p[i],p[Ndev],&aa[idx(a,b)*Nparam],Nparam,func_type)*fac[idx(a,b)];
         Aij[mat(a,b,i,Ndev)] = (  (a==b && i==Ndev ? 1.0 : 0.0)
                                 + Vmat[mat(a,b,i,Ndev)] * un[b] );
      }
      solve_mat_gauss(Aij, Tmat, Vmat, Nchannel*(Ndev+1));
      
//      inv_mat_gauss(Aij, Aij_inv, Nchannel*(Ndev+1));   // Using inv. matrix
//      for (int i=0; i<Nchannel*Nchannel*(Ndev+1)*(Ndev+1); i++)
//         Tmat[i] = COMP_ZERO;
//      for (int a=0; a<Nchannel;          a++)
//      for (int b=0; b<Nchannel;          b++)
//      for (int c=0; c<Nchannel;          c++)
//      for (int l=0; l<Nchannel*(Ndev+1); l++)
//         Tmat[b+Nchannel*a] += Aij_inv[mat(a,c,Ndev,l)]*V_p(b+Nchannel*c,l,Ndev);
      
///////////////////////////   1 channel case   ///////////////////////////
      if (Nchannel==1) {
         phase[0](iE) = atan(  Tmat[mat(0,0,Ndev,Ndev)].imag()
                             / Tmat[mat(0,0,Ndev,Ndev)].real() ) * 180/PI;
         
//         cdouble tmpT = (  exp(COMP_IMAG*phase[0](iE)*PI/180.0)   // For Debug
//                         * sin(phase[0](iE).real()*PI/180.0) );
//         cdouble tmpS = 1.0 - COMP_IMAG * Tmat[mat(0,0,Ndev,Ndev)];
//         cdouble tmp_phase = log(tmpS) / (2.0*COMP_IMAG);
//         phase[0](iE) = tmp_phase * 180.0/PI;
//         if (iE==100)
//         printf("%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n"
//                , phase[0].E(iE), p[Ndev]
//                , Tmat[mat(0,0,Ndev,Ndev)].real()
//                , Tmat[mat(0,0,Ndev,Ndev)].imag()
//                , tmpT.real(), tmpT.imag(), tmpS.real(), tmpS.imag()
//                , abs(tmpS), tmp_phase.real(), tmp_phase.imag() );
      }
///////////////////////////   2 channel case   ///////////////////////////
      else if (Nchannel==2) {
         cdouble Smat11 = 1.0 - COMP_IMAG * Tmat[mat(0,0,Ndev,Ndev)];
         cdouble Smat21 = 0.0 - COMP_IMAG * Tmat[mat(1,0,Ndev,Ndev)];
         cdouble Smat12 = 0.0 - COMP_IMAG * Tmat[mat(0,1,Ndev,Ndev)];
         cdouble Smat22 = 1.0 - COMP_IMAG * Tmat[mat(1,1,Ndev,Ndev)];
         
         cdouble tan2_SYM = (- (Smat12 * Smat21) / (Smat11 * Smat22) );
         cdouble cos2_SYM = 1.0/sqrt(1.0+tan2_SYM);
         cdouble del1_SYM = log(Smat11/cos2_SYM) / (2.0*COMP_IMAG) * 180.0/PI;
         cdouble del2_SYM = log(Smat22/cos2_SYM) / (2.0*COMP_IMAG) * 180.0/PI;
         
//         printf("%lf %lf %lf\n", phase[0].E(iE)
//                , tan2_SYM.real(), tan2_SYM.imag());   // For Debug

         phase[0](iE) = del1_SYM;
         phase[1](iE) = del2_SYM;
      }
//////////////////////////////////////////////////////////////////////////
   } // iE
#undef idx
#undef mat
   delete [] x;
   delete [] p;
   delete [] s;
   delete [] k_sqr;
   delete [] fac;
   delete [] Aij;
   delete [] Aij_inv;
   delete [] ul;
   delete [] un;
   delete [] Tmat;
   delete [] Vmat;

   analysis::route(class_name, func_name, 0);//exit(0);
}
