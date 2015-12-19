//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Function for input the ( miyamoto-format ) fit data
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <fitting.h>

void FITTING::input_data( const char *INFILE_NAME, bool endian_flg ){
    
    func_name = "input_data____________";
    route( class_name, func_name, 1 );
    
    if( new_flg_data ) error(1,"Fit data has already inputed !");
    else{
        
        printf(" @ Fit data reading |   0%%");
        ifstream infile( INFILE_NAME, ios::in | ios::binary );
        if( !infile ) error(2, INFILE_NAME);
        
        infile.read( (char*)&fit_type, sizeof(int) );   // read file header
        if( endian_flg ) endian_convert(&fit_type);
        infile.read( (char*)&N_conf, sizeof(int) );
        if( endian_flg ) endian_convert(&N_conf);
        
//=========================== read corr-type data =========================//
        if( fit_type == 1 ){
            dataSIZE = new int[1];
            infile.read( (char*)&dataSIZE[0], sizeof(int) );
            if( endian_flg ) endian_convert(&dataSIZE[0]);
            cood = new double[ dataSIZE[0] ];
            data = new double[ dataSIZE[0] * N_conf ];
            err  = new double[ dataSIZE[0] ];
            new_flg_data = true;
            
            for(int t=0; t<dataSIZE[0]; t++){
                cood[t] = t;
                infile.read( (char*)&err[t], sizeof(double) );
                for( int i=0; i<N_conf; i++)
                    infile.read( (char*)&data[ idx(i,t) ], sizeof(double) );
                
                printf("\b\b\b\b%3.0lf%%",double(t+1)/double(dataSIZE[0])*100);
                fflush(stdout);
            }
            printf("\n");
            if( endian_flg ){
                endian_convert( data, dataSIZE[0] * N_conf );
                endian_convert( err,  dataSIZE[0] );
            }
//========================= read potential-type data ======================//
        }else if( fit_type == 2 ){
            dataSIZE = new int[ N_conf ];
            int tmp_dataSIZE = 0;
            for(int i=0; i<N_conf; i++){
                infile.read( (char*)&dataSIZE[i], sizeof(int) );
                if( endian_flg ) endian_convert(&dataSIZE[i]);
                tmp_dataSIZE += dataSIZE[i];
            }
            cood = new double[ tmp_dataSIZE ];
            data = new double[ tmp_dataSIZE ];
            err  = new double[ tmp_dataSIZE ];
            new_flg_data = true;
            
            for( int i=0; i<N_conf; i++){
                for(int j=0; j<dataSIZE[i]; j++){
                    infile.read( (char*)&cood[ idx(i,j) ], sizeof(double) );
                    infile.read( (char*)&data[ idx(i,j) ], sizeof(double) );
                    infile.read( (char*)&err[  idx(i,j) ], sizeof(double) );
                }
                printf("\b\b\b\b%3.0lf%%",double(i+1)/double(N_conf)*100);
                fflush(stdout);
            }
            printf("\n");
            if( endian_flg ){
                endian_convert( cood, tmp_dataSIZE );
                endian_convert( data, tmp_dataSIZE );
                endian_convert( err,  tmp_dataSIZE );
            }
        }
//===========================================================================//
        else{
            printf("\n @ fit type         = %d\n", fit_type);
            printf(" @ #.confs          = %d\n", N_conf);
            infile.close();
            error(3,"This file is not miyamoto-format fit data");
        }
        printf(" @ fit type         = %d\n", fit_type);
        printf(" @ #.confs          = %d\n", N_conf);
        infile.close();
    }
    route( class_name, func_name, 0 );
}
