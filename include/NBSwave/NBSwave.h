//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBS wave function
 * @brief   Header file for NBS wave function
 * @author  Takaya Miyamoto
 * @since   Wed Jul 29 02:08:07 JST 2015
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
   extern size_t xyznSIZE;
   
   int reduced_Ndata();
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
   int  time_slice, spin, spin_z, ang_mom;
   CHANNEL_TYPE channel;
   
   cdouble *wave;
    
public:
//============================ For inner index ===========================//
   size_t idx(int x,int y,int z,int n) {
      return    x + analysis::xSIZE
             *( y + analysis::ySIZE
             *( z + analysis::zSIZE * n ));
   }
   size_t NBSidx(int x,int y,int z,int a,int b) {
      return    a + 4
             *( x + analysis::xSIZE
             *( y + analysis::ySIZE
             *( z + analysis::zSIZE * b )));
   }
   int matrix_idx(int row, int column, int rot_type) {
      return column + 4 * ( row + 4 * rot_type );
   }   // rot_type = 0 -> E, 1~6 -> 6C4, 7~9 -> 3C2, 10~17 -> 8C3, 18~23 -> 6C2
//============================== For writing =============================//
   cdouble& operator()(int x, int y, int z, int conf) {
      return wave[   x + analysis::xSIZE
                     *( y + analysis::ySIZE
                     *( z + analysis::zSIZE * conf ))];
   }
   cdouble& operator()(size_t index) {
      return wave[index];
   }
//============================== For reading =============================//
   const cdouble& operator()(int x, int y, int z, int conf) const {
      return wave[   x + analysis::xSIZE
                     *( y + analysis::ySIZE
                     *( z + analysis::zSIZE * conf ))];
   }
   const cdouble& operator()(size_t index) const {
      return wave[index];
   }
//======================== Constructor & Destructor ======================//
   NBS_WAVE() {
      class_name = "NBS_WAVE________________________";
      func_name  = "______________________";
      analysis::route(class_name, func_name, 1);
        
      endian_flg        = false;
      compress_init_flg = false;
      wave              = NULL;
   }
   ~NBS_WAVE() {
      if (wave != NULL) delete [] wave;
        
      func_name = "______________________";
      analysis::route( class_name, func_name, 0 );
   }
//============================= For initialize ===========================//
   void set_NBS(CHANNEL_TYPE ch, int it, bool endian_FLG
                , int SPIN, int SPIN_z, int ANG_MOM, bool compress_FLG) {
        
      func_name = "set_NBS_______________";
      analysis::route( class_name, func_name, 1 );
      
      NBSwave::xyzSIZE  = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
      NBSwave::xyznSIZE = NBSwave::xyzSIZE * analysis::Nconf;
      
      channel      = ch;
      time_slice   = it;
      endian_flg   = endian_FLG;
      compress_flg = compress_FLG;
      spin         = SPIN;
      spin_z       = SPIN_z;
      ang_mom      = ANG_MOM;
      if (spin == 0) spin_z = 0;
        
      if (compress_flg) input_compressed_NBS();
      else              input_NBS();
      func_name = "set_NBS_______________";
      analysis::route(class_name, func_name, 0);
   }
    
   void delete_NBS() {
      
      func_name = "delete_NBS____________";
      analysis::route(class_name, func_name, 1);
        
      if (wave != NULL) {
         delete [] wave;   wave = NULL;
      }
      analysis::route( class_name, func_name, 0 );
   }
//============================ Operator define ===========================//
    
//============================ Operator helper ===========================//
    
//=========================== Several functions ==========================//
   void input_NBS();
   void input_compressed_NBS();
   void make_JK_sample_NBS( int );
   void spin_projection( cdouble*, int );
   void ang_mom_projection();
   void parity_projection();
   void projection();
   void set_rot_matrix();
   void output_NBS_all( const char* );
   void output_NBS_err( const char*, bool );
};

#endif
