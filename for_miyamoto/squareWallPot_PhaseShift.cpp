#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;

//////////////////////////////////////////   inline part   /////////////////////////////////////////////

inline double j_0( double x ){   // spherical Bessel functions (l=0)

    return sin(x)/x;
}

inline double n_0( double x ){   // spherical Neumann functions (l=0)
    
    return -cos(x)/x;
}

inline double j_0_diff( double x ){   // 1st differentation of spherical Bessel functions (l=0)
    
    return (x*cos(x)-sin(x))/pow(x,2);
}

inline double n_0_diff( double x ){   // 1st differentation of spherical Neumann functions (l=0)
    
    return (x*sin(x)+cos(x))/pow(x,2);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////     MAIN PART  ( START )     ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////


int main( int argc, char** argv ){
    
    if( argc == 1 ){ printf("\n FILE ERROR : Please select the input file.\n\n"); return 0; }
    if( argc >  5 ){ printf("\n ARGUMENT ERROR : Too many arguments.\n\n");       return 0; }
    
    double m    = atof(argv[2]);
    double V_0  = atof(argv[3]);
    double a    = atof(argv[4]);
    double k,kk,deno,nume;
    double hber_c = 197.0;
    double PI     = 4.0*atan(1);
    
    char outfile_name[1024];
    snprintf( outfile_name, sizeof(outfile_name),
             "%s/squareWallPot_PhaseShift_m%s_V%s_a%s",argv[1],argv[2],argv[3],argv[4]);
    
    ofstream ofs( outfile_name, ios::out );
    
    for(int E=0; E<=600; E++){
        k    = sqrt(2*m*E/pow(hber_c,2));
        kk   = sqrt(2*m*(E+V_0)/pow(hber_c,2));
        nume = kk*j_0_diff(a*kk)*j_0(a*k)-k*j_0_diff(a*k)*j_0(a*kk);
        deno = kk*j_0_diff(a*kk)*n_0(a*k)-k*n_0_diff(a*k)*j_0(a*kk);
        
        ofs << k << " " << -(nume/deno) /k << endl;
    }
    ofs.close();
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////      MAIN PART  ( END )      /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
