//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBS wave function
 * @brief   Header file for NBS wave function
 * @author  Takaya Miyamoto
 * @since   Mon Nov  7 19:51:54 JST 2016
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
namespace NBSwave
{
   extern int  rot_matrix[384];
   extern bool compress_init_flg;
   
   cdouble Y_0_0 (double, double, double);
   cdouble Y_1_p1(double, double, double);
   cdouble Y_1_0 (double, double, double);
   cdouble Y_1_m1(double, double, double);
   cdouble Y_2_p2(double, double, double);
   cdouble Y_2_p1(double, double, double);
   cdouble Y_2_0 (double, double, double);
   cdouble Y_2_m1(double, double, double);
   cdouble Y_2_m2(double, double, double);
   
   void       rot_matrix_init();
   inline int matrix_idx(int row, int column, int rot_type) {
      return column + 4 * ( row + 4 * rot_type );
   } // rot_type = 0 -> E, 1~6 -> 6C4, 7~9 -> 3C2, 10~17 -> 8C3, 18~23 -> 6C2
}

//--------------------------------------------------------------------------
/**
 * @brief The class for original NBS wave function
 */
//--------------------------------------------------------------------------
class NBS_WAVE_ORG
{
private:
   cdouble *wave_org;
   
protected:
   
public:
//============================ For inner index ===========================//
   int idx(int x,int y,int z,int a,int b) {
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
   cdouble& operator()(int index) {
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
   const cdouble& operator()(int index) const {
      return wave_org[index];
   }
//======================== Constructor & Destructor ======================//
   NBS_WAVE_ORG();
   NBS_WAVE_ORG(const NBS_WAVE_ORG&);
   NBS_WAVE_ORG(const char*);
   NBS_WAVE_ORG(const CHANNEL_TYPE, const int, const int);
   ~NBS_WAVE_ORG();
//============================= For initialize ===========================//
   void mem_alloc();
   void set(const char*);
   void set(const CHANNEL_TYPE, const int, const int);
   void mem_del();
//============================ Operator define ===========================//
   NBS_WAVE_ORG& operator=(const NBS_WAVE_ORG&);
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
   int info_class() { return CLASS_NBS_WAVE_ORG; }
   int data_size () {
      return analysis::xSIZE * analysis::ySIZE * analysis::zSIZE * 4 * 4;
   }
   const int info_class() const { return CLASS_NBS_WAVE_ORG; }
   const int data_size () const {
      return analysis::xSIZE * analysis::ySIZE * analysis::zSIZE * 4 * 4;
   }
   
   void input                    (const CHANNEL_TYPE, const int, const int);
   void input_compressed         (const CHANNEL_TYPE, const int, const int);
   void input_compressed_FromPath(const char*);
   
   void output(const CHANNEL_TYPE, const int, const int);
};

//--------------------------------------------------------------------------
/**
 * @brief The class for NBS wave function spin projected at source part
*/
//--------------------------------------------------------------------------
class NBS_WAVE_SRC_PRJ
{
private:
   cdouble *wave_src_prj;
   
protected:
   
public:
//============================ For inner index ===========================//
   int idx(int x,int y,int z, int a) {
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
   cdouble& operator()(int index) {
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
   const cdouble& operator()(int index) const {
      return wave_src_prj[index];
   }
//======================== Constructor & Destructor ======================//
   NBS_WAVE_SRC_PRJ ();
   NBS_WAVE_SRC_PRJ(const NBS_WAVE_SRC_PRJ&);
   ~NBS_WAVE_SRC_PRJ ();
//============================= For initialize ===========================//
   void mem_alloc ();
   void mem_del ();
//============================ Operator define ===========================//
   NBS_WAVE_SRC_PRJ& operator=(const NBS_WAVE_SRC_PRJ&);
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
   int info_class() { return CLASS_NBS_WAVE_SRC_PRJ; }
   int data_size () {
      return 4 * analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   }
   int info_class() const { return CLASS_NBS_WAVE_SRC_PRJ; }
   int data_size () const {
      return 4 * analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   }
};

//--------------------------------------------------------------------------
/**
 * @brief The class for spin projected NBS wave function
 */
//--------------------------------------------------------------------------
class NBS_WAVE
{
private:
   cdouble *wave;
    
protected:
    
public:
//============================ For inner index ===========================//
   int idx(int x,int y,int z) {
      return x + analysis::xSIZE *( y + analysis::ySIZE * z );
   }
//============================== For writing =============================//
   cdouble& operator()(int x, int y, int z) {
      return wave[ x + analysis::xSIZE *( y + analysis::ySIZE * z ) ];
   }
   cdouble& operator()(int index) {
      return wave[index];
   }
//============================== For reading =============================//
   const cdouble& operator()(int x, int y, int z) const {
      return wave[ x + analysis::xSIZE *( y + analysis::ySIZE * z ) ];
   }
   const cdouble& operator()(int index) const {
      return wave[index];
   }
//======================== Constructor & Destructor ======================//
   NBS_WAVE();
   NBS_WAVE(const NBS_WAVE&);
   NBS_WAVE(const char*);
   NBS_WAVE(const CHANNEL_TYPE, const int, const int);
   ~NBS_WAVE();
//============================= For initialize ===========================//
   void mem_alloc();
   void set(const char*);
   void set(const CHANNEL_TYPE, const int, const int);
   void mem_del();
//============================ Operator define ===========================//
    NBS_WAVE& operator=(const NBS_WAVE&);
//============================ Operator helper ===========================//
    
//=========================== Several functions ==========================//
   int info_class() { return CLASS_NBS_WAVE; }
   int data_size () {
      return analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   }
   int info_class() const { return CLASS_NBS_WAVE; }
   int data_size () const {
      return analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
   }
   
   void input         (const CHANNEL_TYPE, const int, const int);
   void input_FromPath(const char*);
};

namespace NBSwave
{
   void spin_projection  (  const NBS_WAVE_ORG&,     NBS_WAVE_SRC_PRJ&
                          , const SPIN_TYPE );
   void spin_projection  (  const NBS_WAVE_SRC_PRJ&, NBS_WAVE&
                          , const SPIN_TYPE );
   void spin_projection  (  const NBS_WAVE_ORG&,     NBS_WAVE&
                          , const SPIN_TYPE );
   
   void Swave_projection (NBS_WAVE&);
   void Swave_division   (NBS_WAVE&,         NBS_WAVE&);
   void Swave_division   (NBS_WAVE_SRC_PRJ&, NBS_WAVE_SRC_PRJ&);
   
   void parity_projection(NBS_WAVE&);
   void parity_projection(NBS_WAVE_SRC_PRJ&);
   
   void LP_projection    (NBS_WAVE&);
   
   void remove_angular(NBS_WAVE&, const int, const double);
   
   void S12_psi(const NBS_WAVE_SRC_PRJ&, NBS_WAVE&, const SPIN_TYPE);
   void S12_psi(const NBS_WAVE_SRC_PRJ&, NBS_WAVE_SRC_PRJ&);
   
   void mult_YDstar(NBS_WAVE_SRC_PRJ&, const SPIN_TYPE);
}

#endif
