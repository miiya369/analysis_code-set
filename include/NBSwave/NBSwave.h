//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBS wave function
 * @brief   Header file for NBS wave function
 * @author  Takaya Miyamoto
 * @since   Fri Dec 18 00:17:52 JST 2015
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
   
   extern int  rot_matrix[384];
   extern bool compress_init_flg;
   
   cdouble Y_0_0 ( double, double, double );
   cdouble Y_1_p1( double, double, double );
   cdouble Y_1_0 ( double, double, double );
   cdouble Y_1_m1( double, double, double );
   cdouble Y_2_p2( double, double, double );
   cdouble Y_2_p1( double, double, double );
   cdouble Y_2_0 ( double, double, double );
   cdouble Y_2_m1( double, double, double );
   cdouble Y_2_m2( double, double, double );
   
   void       rot_matrix_init();
   inline int matrix_idx(int row, int column, int rot_type) {
      return column + 4 * ( row + 4 * rot_type );
   }   // rot_type = 0 -> E, 1~6 -> 6C4, 7~9 -> 3C2, 10~17 -> 8C3, 18~23 -> 6C2
}

//--------------------------------------------------------------------------
/**
 * @brief The class for original NBS wave function
 */
//--------------------------------------------------------------------------
class NBS_WAVE_ORG {
   
private:
   string class_name, func_name;
   
   cdouble *wave_org;
   
protected:
   
public:
//============================ For inner index ===========================//
   size_t idx(int x,int y,int z,int a,int b) {
      return    a + 4
             *( x + analysis::xSIZE
             *( y + analysis::ySIZE
             *( z + analysis::zSIZE * b )));
   }
//============================== For writing =============================//
   cdouble& operator()(int x,int y,int z, int a,int b) {
      return wave_org[   a + 4
                      *( x + analysis::xSIZE
                      *( y + analysis::ySIZE
                      *( z + analysis::zSIZE * b ))) ];
   }
   cdouble& operator()(int space,int a,int b) {
      return wave_org[ a + 4
            *( space + analysis::xSIZE*analysis::ySIZE*analysis::zSIZE * b ) ];
   }
   cdouble& operator()(size_t index) {
      return wave_org[index];
   }
//============================== For reading =============================//
   const cdouble& operator()(int x,int y,int z, int a,int b) const {
      return wave_org[   a + 4
                      *( x + analysis::xSIZE
                      *( y + analysis::ySIZE
                      *( z + analysis::zSIZE * b ))) ];
   }
   const cdouble& operator()(int space,int a,int b) const {
      return wave_org[ a + 4
            *( space + analysis::xSIZE*analysis::ySIZE*analysis::zSIZE * b ) ];
   }
   const cdouble& operator()(size_t index) const {
      return wave_org[index];
   }
//======================== Constructor & Destructor ======================//
   NBS_WAVE_ORG() {
      class_name = "NBS_WAVE_ORG____________________";
      func_name  = "______________________";
      analysis::route(class_name, func_name, 1);

      wave_org   = NULL;
   }
   NBS_WAVE_ORG(  CHANNEL_TYPE ch, int it, int iconf
                , bool endian_FLG, bool compress_FLG ) {
      class_name = "NBS_WAVE_ORG____________________";
      func_name  = "______________________";
      analysis::route(class_name, func_name, 1);
      
      wave_org = NULL;
      
      set( ch, it, iconf, endian_FLG, compress_FLG );
   }
   ~NBS_WAVE_ORG() {
      if (wave_org != NULL) delete [] wave_org;
      
      func_name = "______________________";
      analysis::route( class_name, func_name, 0 );
   }
//============================= For initialize ===========================//
   void mem_alloc() {
      
      func_name = "mem_alloc_NBS_ORG_____";
      analysis::route( class_name, func_name, 1 );
      
      if (wave_org == NULL) {
         size_t xyzSIZE = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
         wave_org       = new cdouble[xyzSIZE * 4 * 4];
      }
      analysis::route(class_name, func_name, 0);
   }
   void set(  CHANNEL_TYPE ch, int it, int iconf
            , bool endian_FLG, bool compress_FLG ) {
      
      func_name = "set_NBS_ORG___________";
      analysis::route( class_name, func_name, 1 );
      
      mem_alloc();
      
      if (compress_FLG) input_compressed(ch, it, iconf, endian_FLG);
      else              input(ch, it, iconf, endian_FLG);
      func_name = "set_NBS_ORG___________";
      analysis::route(class_name, func_name, 0);
   }
   void mem_del() {
      
      func_name = "mem_delete_NBS_ORG____";
      analysis::route(class_name, func_name, 1);
      
      if (wave_org != NULL) {
         delete [] wave_org;   wave_org = NULL;
      }
      analysis::route( class_name, func_name, 0 );
   }
//============================ Operator define ===========================//
   
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
   int info_class() { return CLASS_NBS_WAVE_ORG; }
   int data_size () {
      return analysis::xSIZE * analysis::ySIZE * analysis::zSIZE * 4 * 4;
   }
   
   void input( CHANNEL_TYPE, int, int, bool );
   void input_compressed( CHANNEL_TYPE, int, int, bool );
};

//--------------------------------------------------------------------------
/**
 * @brief The class for NBS wave function spin projected at source part
*/
//--------------------------------------------------------------------------
class NBS_WAVE_SRC_PRJ {
   
private:
   string class_name, func_name;
   
   cdouble *wave_src_prj;
   
protected:
   
public:
//============================ For inner index ===========================//
   size_t idx(int x,int y,int z, int a) {
      return a + 4 *( x + analysis::xSIZE *( y + analysis::ySIZE * z ));
   }
//============================== For writing =============================//
   cdouble& operator()(int x, int y, int z, int a) {
      return wave_src_prj[   a + 4
                          *( x + analysis::xSIZE *( y + analysis::ySIZE * z )) ];
   }
   cdouble& operator()(int space,int a) {
      return wave_src_prj[ a + 4 * space ];
   }
   cdouble& operator()(size_t index) {
      return wave_src_prj[index];
   }
//============================== For reading =============================//
   const cdouble& operator()(int x, int y, int z, int a) const {
      return wave_src_prj[   a + 4
                          *( x + analysis::xSIZE *( y + analysis::ySIZE * z )) ];
   }
   const cdouble& operator()(int space,int a) const {
      return wave_src_prj[ a + 4 * space ];
   }
   const cdouble& operator()(size_t index) const {
      return wave_src_prj[index];
   }
//======================== Constructor & Destructor ======================//
   NBS_WAVE_SRC_PRJ () {
      class_name = "NBS_WAVE_SRC_PRJ________________";
      func_name  = "______________________";
      analysis::route(class_name, func_name, 1);
      
      wave_src_prj = NULL;
   }
   ~NBS_WAVE_SRC_PRJ () {
      if (wave_src_prj != NULL) delete [] wave_src_prj;
      
      func_name = "______________________";
      analysis::route( class_name, func_name, 0 );
   }
//============================= For initialize ===========================//
   void mem_alloc () {
      
      func_name = "mem_alloc_NBS_SRC_PRJ_";
      analysis::route( class_name, func_name, 1 );
      
      if (wave_src_prj == NULL) {
         size_t axyzSIZE = 4 * analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
         wave_src_prj = new cdouble[axyzSIZE];
      }
      analysis::route(class_name, func_name, 0);
   }
   void mem_del () {
      
      func_name = "mem_delete_NBS_SRC_PRJ";
      analysis::route(class_name, func_name, 1);
      
      if (wave_src_prj != NULL) {
         delete [] wave_src_prj;   wave_src_prj = NULL;
      }
      analysis::route( class_name, func_name, 0 );
   }
//============================ Operator define ===========================//
   
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
   int info_class() { return CLASS_NBS_WAVE_SRC_PRJ; }
   int data_size () {
      return 4 * analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   }
};

//--------------------------------------------------------------------------
/**
 * @brief The class for spin projected NBS wave function
 */
//--------------------------------------------------------------------------
class NBS_WAVE {
    
private:
   string class_name, func_name;
   
   cdouble *wave;
    
protected:
    
public:
//============================ For inner index ===========================//
   size_t idx(int x,int y,int z) {
      return x + analysis::xSIZE *( y + analysis::ySIZE * z );
   }
//============================== For writing =============================//
   cdouble& operator()(int x, int y, int z) {
      return wave[ x + analysis::xSIZE *( y + analysis::ySIZE * z ) ];
   }
   cdouble& operator()(size_t index) {
      return wave[index];
   }
//============================== For reading =============================//
   const cdouble& operator()(int x, int y, int z) const {
      return wave[ x + analysis::xSIZE *( y + analysis::ySIZE * z ) ];
   }
   const cdouble& operator()(size_t index) const {
      return wave[index];
   }
//======================== Constructor & Destructor ======================//
   NBS_WAVE() {
      class_name = "NBS_WAVE________________________";
      func_name  = "______________________";
      analysis::route(class_name, func_name, 1);
      
      wave       = NULL;
   }
   ~NBS_WAVE() {
      if (wave != NULL) delete [] wave;
        
      func_name = "______________________";
      analysis::route( class_name, func_name, 0 );
   }
//============================= For initialize ===========================//
   void mem_alloc() {
      
      func_name = "mem_alloc_NBS_________";
      analysis::route( class_name, func_name, 1 );
      
      if (wave == NULL) {
         size_t xyzSIZE = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
         wave = new cdouble[xyzSIZE];
      }
      analysis::route(class_name, func_name, 0);
   }
   void mem_del() {
      
      func_name = "mem_delete_NBS________";
      analysis::route(class_name, func_name, 1);
        
      if (wave != NULL) {
         delete [] wave;   wave = NULL;
      }
      analysis::route( class_name, func_name, 0 );
   }
//============================ Operator define ===========================//
    
//============================ Operator helper ===========================//
    
//=========================== Several functions ==========================//
   int info_class() { return CLASS_NBS_WAVE; }
   int data_size () { return analysis::xSIZE * analysis::ySIZE * analysis::zSIZE; }
};

namespace NBSwave {
   
   void spin_projection  ( NBS_WAVE_ORG&,     NBS_WAVE_SRC_PRJ&, SPIN_TYPE );
   void spin_projection  ( NBS_WAVE_SRC_PRJ&, NBS_WAVE&        , SPIN_TYPE );
   void spin_projection  ( NBS_WAVE_ORG&,     NBS_WAVE&        , SPIN_TYPE );
   void Swave_projection ( NBS_WAVE& );
   void Swave_division   ( NBS_WAVE&, NBS_WAVE& );
   void parity_projection( NBS_WAVE& );
   void LP_projection    ( NBS_WAVE& );
   
   void remove_angular( NBS_WAVE&, int, double );
   void S12_psi( NBS_WAVE_SRC_PRJ&, NBS_WAVE&, SPIN_TYPE );
}

#endif
