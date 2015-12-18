//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   The core of fitting
 * @brief   This code is mimic of Numerical Recipes
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <fitting.h>

double FITTING::fitmrq( double *xx, double *yy, double *ssig, double *param, int data_size ){
    
    int done=0, NDONE = 4;
    double alamda=0.001,ichisq,ochisq,tol=stp_cnd;
    
    double *aa = new double[N_param];
    
    double *atry  = new double[N_param];
    double *beta  = new double[N_param];
    double *da    = new double[N_param];
    double *oneda = new double[N_param];
    
    double *alpha = new double[N_param*N_param];
    double *covar = new double[N_param*N_param];
    double *temp  = new double[N_param*N_param];
    
    for( int j=0; j<N_param; j++ ) aa[j] = param_ini[j];
    
    ichisq=mrqcof( xx, yy, ssig, aa, alpha, beta, data_size );
    
    for( int j=0; j<N_param; j++ ) atry[j] = aa[j];
    ochisq=ichisq;
    for( int iter=0; iter<1000000; iter++ ){
        
        if(done==NDONE) alamda=0.0;
        
        for( int j=0; j<N_param; j++ ){
            for( int k=0; k<N_param; k++ ) covar[prm_idx(j,k)]=alpha[prm_idx(j,k)];
            covar[prm_idx(j,j)]=alpha[prm_idx(j,j)]*(1.0+alamda);
            for( int k=0; k<N_param; k++ ) temp[prm_idx(j,k)]=covar[prm_idx(j,k)];
            oneda[j]=beta[j];
        }
        gaussj(temp,oneda);
        
        for( int j=0; j<N_param; j++ ){
            for( int k=0; k<N_param; k++) covar[prm_idx(j,k)]=temp[prm_idx(j,k)];
            da[j]=oneda[j];
        }
        if(done==NDONE){
            covsrt(covar);
            covsrt(alpha);
            
            for(int loop=0; loop < N_param; loop++ )
                param[loop] = aa[loop];
            
            delete [] aa;
            delete [] atry;
            delete [] beta;
            delete [] da;
            delete [] oneda;
            delete [] alpha;
            delete [] covar;
            delete [] temp;
            
            return ichisq / data_size;
        }
        for( int j=0,l=0; l<N_param; l++ )
            atry[l]=aa[l]+da[j++];
        
        ichisq=mrqcof( xx, yy, ssig, atry, covar, da, data_size );
        
        if( abs(ichisq-ochisq) < MAX(tol, tol*ichisq) ) done++;
        if( ichisq < ochisq ){
            alamda *= 0.1;
            ochisq=ichisq;
            for( int j=0; j<N_param; j++ ){
                for( int k=0; k<N_param; k++ ) alpha[prm_idx(j,k)]=covar[prm_idx(j,k)];
                beta[j]=da[j];
            }
            for( int l=0; l<N_param; l++ ) aa[l]=atry[l];
        }else{
            alamda *= 10.0;
            ichisq=ochisq;
        }
    }
    delete [] aa;
    delete [] atry;
    delete [] beta;
    delete [] da;
    delete [] oneda;
    delete [] alpha;
    delete [] covar;
    delete [] temp;
    throw("Fitmrq too many iterations");
}


double FITTING::mrqcof( double *x, double *y, double *sig, double *a,
                        double *alpha, double *beta, int datasize ){
    
    double chisq_tmp,ymod,wt,sig2i,dy;
    double *dyda = new double[N_param];
    
    for( int j=0; j<N_param; j++ ){
        for( int k=0; k<=j; k++ ) alpha[prm_idx(j,k)]=0.0;
        beta[j]=0.0;
    }
    chisq_tmp=0.0;
    
    for( int i=0; i<datasize; i++ ){
        
        ymod = fit_functions(x[i],a,dyda);
        
        sig2i=1.0/(sig[i]*sig[i]);
        dy=y[i]-ymod;
        for( int j=0,l=0; l<N_param; l++){
            wt=dyda[l]*sig2i;
            for( int k=0,m=0; m<l+1; m++)
                if( m<N_param ) alpha[prm_idx(j,k++)] += wt*dyda[m];
            beta[j++] += dy*wt;
        }
        chisq_tmp += dy*dy*sig2i;
    }
    for( int j=1; j<N_param; j++ )
        for( int k=0; k<j; k++ ) alpha[prm_idx(k,j)]=alpha[prm_idx(j,k)];
    
    delete [] dyda;
    
    return chisq_tmp;
}

void FITTING::gaussj( double *a, double *b )
{
    int icol=0,irow=0,n=N_param;
    double big,dum,pivinv;
    int *indxc = new int[N_param];
    int *indxr = new int[N_param];
    int *ipiv  = new int[N_param];
    for( int j=0; j<n; j++) ipiv[j]=0;
    for( int i=0; i<n; i++){
        big=0.0;
        for( int j=0; j<n; j++ )
            if( ipiv[j] != 1 )
                for( int k=0; k<n; k++ ){
                    if( ipiv[k] == 0 ){
                        if( abs(a[prm_idx(j,k)]) >= big ){
                            big=abs(a[prm_idx(j,k)]);
                            irow=j;
                            icol=k;
                        }
                    }
                }
        ++(ipiv[icol]);
        if( irow != icol ){
            for( int l=0; l<n; l++ ) SWAP( a[prm_idx(irow,l)], a[prm_idx(icol,l)] );
            SWAP( b[irow], b[icol] );
        }
        indxr[i]=irow;
        indxc[i]=icol;
        if( a[prm_idx(icol,icol)] == 0.0 ){
            printf("\n");
            error(3,"gaussj: Singular Matrix");
        }
        pivinv=1.0/a[prm_idx(icol,icol)];
        a[prm_idx(icol,icol)]=1.0;
        for( int l=0; l<n; l++) a[prm_idx(icol,l)] *= pivinv;
        b[icol] *= pivinv;
        for( int ll=0; ll<n; ll++)
            if( ll != icol ){
                dum=a[prm_idx(ll,icol)];
                a[prm_idx(ll,icol)]=0.0;
                for( int l=0; l<n; l++ ) a[prm_idx(ll,l)] -= a[prm_idx(icol,l)]*dum;
                b[ll] -= b[icol]*dum;
            }
    }
    for( int l=n-1; l>=0; l--){
        if( indxr[l] != indxc[l] )
            for( int k=0; k<n; k++ )
                SWAP( a[prm_idx(k,indxr[l])], a[prm_idx(k,indxc[l])] );
    }
    delete [] indxc;
    delete [] indxr;
    delete [] ipiv;
}

void FITTING::covsrt(double *covar){
    for( int i=N_param; i<N_param; i++)
        for( int j=0; j<i+1; j++ ) covar[prm_idx(i,j)]=covar[prm_idx(j,i)]=0.0;
    int k=N_param-1;
    for( int j=N_param-1; j>=0; j-- ){
        for(int i=0;i<N_param;i++) SWAP(covar[prm_idx(i,k)],covar[prm_idx(i,j)]);
        for(int i=0;i<N_param;i++) SWAP(covar[prm_idx(k,i)],covar[prm_idx(j,i)]);
        k--;
    }
}
