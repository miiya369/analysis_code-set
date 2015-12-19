//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBS wave function
 * @brief   Header file for NBS wave function
 * @author  Takaya Miyamoto
 * @since   Wed Jul 22 03:05:59 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef NBS_WAVE_FUNCTION_H
#define NBS_WAVE_FUNCTION_H

#include <common/analysis.h>

//--------------------------------------------------------------------------
/**
 * @brief The namespace for NBS wave function
 */
//--------------------------------------------------------------------------
namespace NBSwave {

   extern size_t xyzSIZE;
   extern size_t NBSSIZE;
   extern size_t xyznSIZE;
   extern size_t NBSnSIZE;
   
   size_t data_reduction(size_t);
   size_t data_reduction_16(size_t);
   size_t data_reduction_32(size_t);
}

//--------------------------------------------------------------------------
/**
 * @brief The class for NBS wave function
 */
//--------------------------------------------------------------------------
class NBS_WAVE {
    
private:
   string class_name, func_name;
   
   int    rot_matrix[384];
    
protected:
   bool endian_flg, compress_flg, compress_init_flg;
   int  time_slice;
   CHANNEL_TYPE channel;
   
   cdouble *orgNBS;
   cdouble *projNBS;
    
public:
//============================ For inner index ===========================//
   size_t idx(int x,int y,int z,int n) {
      return    x + analysis::xSIZE
             *( y + analysis::ySIZE
             *( z + analysis::zSIZE * n ));
   }
   size_t NBSidx(int x,int y,int z,int a,int b,int n) {
      return    a + 4
             *( x + analysis::xSIZE
             *( y + analysis::ySIZE
             *( z + analysis::zSIZE
             *( b + 4 * n ))));
   }
   int matrix_idx(int row, int column, int rot_type) {
      return column + 4 * ( row + 4 * rot_type );
   }   // rot_type = 0 -> E, 1~6 -> 6C4, 7~9 -> 3C2, 10~17 -> 8C3, 18~23 -> 6C2
//============================== For writing =============================//
   cdouble& operator()(int x, int y, int z, int conf) {
      return projNBS[   x + analysis::xSIZE
                     *( y + analysis::ySIZE
                     *( z + analysis::zSIZE * conf ))];
   }
   cdouble& operator()(size_t index) {
      return projNBS[index];
   }
//============================== For reading =============================//
   const cdouble& operator()(int x, int y, int z, int conf) const {
      return projNBS[   x + analysis::xSIZE
                     *( y + analysis::ySIZE
                     *( z + analysis::zSIZE * conf ))];
   }
   const cdouble& operator()(size_t index) const {
      return projNBS[index];
   }
//======================== Constructor & Destructor ======================//
   NBS_WAVE() {
      class_name = "NBS_WAVE________________________";
      func_name  = "______________________";
      analysis::route(class_name, func_name, 1);
        
      endian_flg        = false;
      compress_init_flg = false;
      orgNBS            = NULL;
      projNBS           = NULL;
   }
   ~NBS_WAVE() {
      if ( orgNBS != NULL) delete [] orgNBS;
      if (projNBS != NULL) delete [] projNBS;
        
      func_name = "______________________";
      analysis::route( class_name, func_name, 0 );
   }
//============================= For initialize ===========================//
   void set_NBS(CHANNEL_TYPE ch, int it, bool endian_FLG, bool compress_FLG) {
        
      func_name = "set_NBS_______________";
      analysis::route( class_name, func_name, 1 );
      
      NBSwave::xyzSIZE  = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
      NBSwave::NBSSIZE  =  NBSwave::xyzSIZE * 4 * 4;
      NBSwave::xyznSIZE =  NBSwave::xyzSIZE * analysis::Nconf;
      NBSwave::NBSnSIZE =  NBSwave::NBSSIZE * analysis::Nconf;
      
      channel      = ch;
      time_slice   = it;
      endian_flg   = endian_FLG;
      compress_flg = compress_FLG;
        
      if (compress_flg) input_compressed_NBS();
      else              input_NBS();
      func_name = "set_NBS_______________";
      analysis::route(class_name, func_name, 0);
   }
    
   void delete_NBS() {
      
      func_name = "delete_NBS____________";
      analysis::route(class_name, func_name, 1);
        
      if (orgNBS  != NULL) {
         delete [] orgNBS;   orgNBS  = NULL;
      }
      if (projNBS != NULL) {
         delete [] projNBS;  projNBS = NULL;
      }
      analysis::route( class_name, func_name, 0 );
   }
//============================ Operator define ===========================//
    
//============================ Operator helper ===========================//
    
//=========================== Several functions ==========================//
   void input_NBS();
   void input_compressed_NBS();
   void make_JK_sample_orgNBS( int );
   void make_JK_sample_projNBS( int );
   void spin_projection( int );
   void ang_mom_projection( int );
   void parity_projection();
   void projection( int, int );
   void set_rot_matrix();
   void output_NBS_all( const char* );
   void output_NBS_err( const char* );
};

#endif
