//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup gauge
 * @brief   Header file for several functions of gauge link valiable
 * @brief   NOTE: gauge link valiable must be form "ildg format"
 * @author  Takaya Miyamoto
 * @since   Mon Aug 17 00:55:17 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef GAUGE_LINK_H
#define GAUGE_LINK_H

#include <common/matrix.h>

#define idx(mu,x,y,z,t) mu+4*(x+sSIZE*(y+sSIZE*(z+sSIZE*t)))

double plaquette(double *iconf, int sSIZE, int tSIZE) {
   
   MATRIX* conf = (MATRIX*)iconf;
   double  ret  = 0.0;
   
   size_t xyztSIZE =   sSIZE* sSIZE* sSIZE* tSIZE;
   int    size[]   = { sSIZE, sSIZE, sSIZE, tSIZE };   // x, y, z, t
   
#ifdef _OPENMP
#pragma omp parallel for reduction (+: ret)
#endif
   for (size_t site_loop=0; site_loop<xyztSIZE; site_loop++) {
      MATRIX tmp;
      
      int n[4],m[4],l[4];
//===================   Plaquette notation   ====================
//          m
//           x--->---x         mu          thus, m = n + mu
//           |       |         ^                 l = n + nu
//   P(n) =  ^       v         |
//           |       |         x--> nu
//           @---<---x
//          n         l         ( n[0]=x, n[1]=y, n[2]=z, n[3]=t )
//===============================================================
      size_t tmp_site = site_loop;
      for (int xyzt=0; xyzt<4; xyzt++) {
         n[xyzt]  =  tmp_site          % size[xyzt];
         tmp_site = (tmp_site-n[xyzt]) / size[xyzt];
      }
      for (int mu=0; mu<4;  mu++)
         for (int nu=0; nu<mu; nu++) {
            
            for (int loop=0; loop<4; loop++)
               l[loop] = m[loop] = n[loop];
            
            m[mu] = (m[mu]+1) % size[mu];
            l[nu] = (l[nu]+1) % size[nu];
            
            tmp =  conf[idx(mu,n[0],n[1],n[2],n[3])]
                 * conf[idx(nu,m[0],m[1],m[2],m[3])]
                 * conf[idx(mu,l[0],l[1],l[2],l[3])].dagger()
                 * conf[idx(nu,n[0],n[1],n[2],n[3])].dagger();
            
            ret += tmp.trace().real();   // <- sum_n Re tr P(n)
         }
   }
   return ret/double(xyztSIZE*18);
}

void stout_smearing(double *iconf, double param, int sSIZE, int tSIZE) {
   
   MATRIX* conf = (MATRIX*)iconf;
   
   size_t xyztSIZE =   sSIZE* sSIZE* sSIZE* tSIZE;
   int    size[]   = { sSIZE, sSIZE, sSIZE, tSIZE };   // x, y, z, t
   
   MATRIX *conf_tmp = new MATRIX[xyztSIZE*4];
   
#ifdef _OPENMP
#pragma omp parallel for
#endif
   for (size_t site_loop=0; site_loop<xyztSIZE; site_loop++) {
      MATRIX staple, tmp;
      
      int n[4],m1[4],m2[4],l1[4],l2[4];
//====================   Staple notation   ======================
//      l2     m2
//      x-->-- x --<--x         mu          thus, m1 = n + nu
//      |             |         ^                 m2 = n + mu
//      ^             ^         |                 l1 = n - nu
//      |             |         x--> nu           l2 = n - nu + mu
//      x--<-- x -->--x
//      l1     n      m1     ( n[0]=x, n[1]=y, n[2]=z, n[3]=t )
//===============================================================
      size_t tmp_site = site_loop;
      for (int xyzt=0; xyzt<4; xyzt++) {
         n[xyzt]  =  tmp_site          % size[xyzt];
         tmp_site = (tmp_site-n[xyzt]) / size[xyzt];
      }
      for (   int mu=0; mu<4; mu++) {
         staple.init();
         for (int nu=0; nu<4; nu++) {
            if (mu == nu) continue;
            for (int loop=0; loop<4; loop++)
               l2[loop] = l1[loop] = m2[loop] = m1[loop] = n[loop];
            
            m1[nu] = (m1[nu]+1)          % size[nu];
            m2[mu] = (m2[mu]+1)          % size[mu];
            l1[nu] = (l1[nu]-1+size[nu]) % size[nu];
            l2[nu] = (l2[nu]-1+size[nu]) % size[nu];
            l2[mu] = (l2[mu]+1)          % size[mu];
            
            staple += (  conf[idx(nu, n[0], n[1], n[2], n[3])]
                       * conf[idx(mu,m1[0],m1[1],m1[2],m1[3])]
                       * conf[idx(nu,m2[0],m2[1],m2[2],m2[3])].dagger()
                      +  conf[idx(nu,l1[0],l1[1],l1[2],l1[3])].dagger()
                       * conf[idx(mu,l1[0],l1[1],l1[2],l1[3])]
                       * conf[idx(nu,l2[0],l2[1],l2[2],l2[3])]
                       ) * param;
         } // nu
         staple *= conf[idx(mu,n[0],n[1],n[2],n[3])].dagger();
         tmp = (   (staple.dagger() - staple)
                - ((staple.dagger() - staple).trace()/3.0))* cdouble(0.0, 0.5);
         conf_tmp[idx(mu,n[0],n[1],n[2],n[3])]
            = tmp.exp_su3() * conf[idx(mu,n[0],n[1],n[2],n[3])];
      } // mu
   } // site_loop
#ifdef _OPENMP
#pragma omp parallel for
#endif
   for(size_t loop=0; loop<xyztSIZE*4; loop++)
      conf[loop] = conf_tmp[loop];
   
   delete [] conf_tmp;
}

cdouble check_coulomb_gfix(double *iconf, int sSIZE, int tSIZE) {
   
   MATRIX* conf = (MATRIX*)iconf;
   cdouble ret = 0.0;
   
   size_t xyztSIZE =   sSIZE* sSIZE* sSIZE* tSIZE;
   int    size[]   = { sSIZE, sSIZE, sSIZE, tSIZE };   // x, y, z, t
   
#ifdef _OPENMP
#pragma omp parallel for
#endif
   for (size_t site_loop=0; site_loop<xyztSIZE; site_loop++) {
      MATRIX tmp;
      
      int n[4],m[4];
//=========================   notation   ========================
//
//        m       n                 define, m = n - mu
//        x--->---@--->---x
//
//                           ( n[0]=x, n[1]=y, n[2]=z, n[3]=t )
//===============================================================
      size_t tmp_site = site_loop;
      for (int xyzt=0; xyzt<4; xyzt++) {
         n[xyzt]  =  tmp_site          % size[xyzt];
         tmp_site = (tmp_site-n[xyzt]) / size[xyzt];
      }
      for (int mu=0; mu<3;  mu++) {
            
            for (int loop=0; loop<4; loop++)
               m[loop] = n[loop];
            
            m[mu] = (m[mu]-1+size[mu]) % size[mu];
            
            tmp =  conf[idx(mu,n[0],n[1],n[2],n[3])]
                 - conf[idx(mu,m[0],m[1],m[2],m[3])]
                 - conf[idx(mu,n[0],n[1],n[2],n[3])].dagger()
                 + conf[idx(mu,m[0],m[1],m[2],m[3])].dagger();
         
            ret += tmp.sum_all_elements2();
         }
   }
   return ret/double(xyztSIZE*4);
}

#undef idx
#endif
