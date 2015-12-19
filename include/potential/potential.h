//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Header file for potential class
 * @author  Takaya Miyamoto
 * @since   Thu Jul 23 11:59:40 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef POTENTIAL_H
#define POTENTIAL_H

#include <R_correlator/R_correlator.h>

//--------------------------------------------------------------------------
/**
 * @brief The class for potential
 */
//--------------------------------------------------------------------------
class POTENTIAL {
    
private:
   string class_name, func_name;
   
   string potential_type;
   int time_slice, spin, ang_mom;
   int Rcorr_t[3];
   double reduced_mass;
   HADRON_TYPE hadron1;
   HADRON_TYPE hadron2;
   CHANNEL_TYPE channel;
   
   bool endian_flg, compress_flg;
   bool Rcorr_reread_flg[3], read_time_slice_flg[3];
   
   cdouble      *corr1;
   cdouble      *corr2;
   
protected:
   
public:
   R_CORRELATOR *Rcorr;
   cdouble      *potential;
//============================ For inner index ===========================//
   size_t idx(int x,int y,int z,int n) {
      return    x + analysis::xSIZE
             *( y + analysis::ySIZE
             *( z + analysis::zSIZE * n ));
   }
   size_t xyz(int x,int y,int z) {
      return    x + analysis::xSIZE
             *( y + analysis::ySIZE * z );
   }
//============================== For writing =============================//
   cdouble& operator()(int x, int y, int z, int conf) {
      return potential[   x + analysis::xSIZE
                       *( y + analysis::ySIZE
                       *( z + analysis::zSIZE * conf ))];
   }
   cdouble& operator()(size_t index) {
      return potential[index];
   }
//============================== For reading =============================//
   const cdouble& operator()(int x, int y, int z, int conf) const {
      return potential[   x + analysis::xSIZE
                       *( y + analysis::ySIZE
                       *( z + analysis::zSIZE * conf ))];
   }
   const cdouble& operator()(size_t index) const {
      return potential[index];
   }
//======================== Constructor & Destructor ======================//
   POTENTIAL() {
      class_name = "POTENTIAL_______________________";
      func_name = "______________________";
      analysis::route(class_name, func_name, 1);
      
      endian_flg = false;
      potential  = NULL;
      Rcorr      = NULL;
      corr1      = NULL;
      corr2      = NULL;
   }
   ~POTENTIAL() {
      if (potential != NULL) delete [] potential;
      if (Rcorr     != NULL) delete [] Rcorr;
      if (corr1     != NULL) {
         delete [] corr1;
         delete [] corr2;
      }
      func_name = "______________________";
      analysis::route(class_name, func_name, 0);
   }
//============================= For initialize ===========================//
   void set_pot(  CHANNEL_TYPE ch, int it, bool endian_FLG, bool compress_FLG
                , int SPIN, int ANGMOM, double REDUCED_MASS ) {
      
      func_name = "set_pot_Rcorr_NBS/corr";
      analysis::route(class_name, func_name, 1);
      
      NBSwave::xyzSIZE  = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
      NBSwave::NBSSIZE  =  NBSwave::xyzSIZE * 4 * 4;
      NBSwave::xyznSIZE =  NBSwave::xyzSIZE * analysis::Nconf;
      NBSwave::NBSnSIZE =  NBSwave::NBSSIZE * analysis::Nconf;
      
      channel      = ch;
      hadron1      = channel.hadron1;
      hadron2      = channel.hadron2;
      time_slice   = it;
      spin         = SPIN;
      ang_mom      = ANGMOM;
      endian_flg   = endian_FLG;
      compress_flg = compress_FLG;
      reduced_mass = REDUCED_MASS;
      
      input_pot();
      func_name = "set_pot_Rcorr_NBS/corr";
      analysis::route(class_name, func_name, 0);
   }
   
   void set_pot(  CHANNEL_TYPE ch, int it, bool endian_FLG, bool compress_FLG
                , int SPIN, int ANGMOM, double HAD1_mass, double HAD2_mass ) {
      
      func_name = "set_pot_Rcorr_NBS/mass";
      analysis::route(class_name, func_name, 1);
      
      channel      = ch;
      hadron1      = channel.hadron1;
      hadron2      = channel.hadron2;
      time_slice   = it;
      spin         = SPIN;
      ang_mom      = ANGMOM;
      endian_flg   = endian_FLG;
      compress_flg = compress_FLG;
      reduced_mass = HAD1_mass*HAD2_mass / (HAD1_mass+HAD2_mass);
      
      input_pot(HAD1_mass, HAD2_mass);
      func_name = "set_pot_Rcorr_NBS/mass";
      analysis::route(class_name, func_name, 0);
   }
   
   void delete_pot() {
      
      func_name = "delete_pot____________";
      analysis::route(class_name, func_name, 1);
      
      if (Rcorr != NULL) {
         delete [] Rcorr;
         Rcorr = NULL;
      }
      if (corr1 != NULL) {
         delete [] corr1;
         delete [] corr2;
         corr1 = NULL;
         corr2 = NULL;
      }
      analysis::route(class_name, func_name, 0);
   }
   
   void delete_pot_corr() {
      
      func_name = "delete_pot_corr_______";
      analysis::route(class_name, func_name, 1);
      
      if (corr1 != NULL) {
         delete [] corr1;
         delete [] corr2;
         corr1 = NULL;
         corr2 = NULL;
      }
      analysis::route(class_name, func_name, 0);
   }
//============================ Operator define ===========================//
   
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
   void calc_laplacian();
   void calc_1st_timediff();
   void calc_2nd_timediff();
   void calc_pot_kernel();
   void input_pot();
   void input_pot( double, double );
   void set_pot_from_binary( const char* );
   void output_single_pot_all( const char* );
   void output_single_pot_err( const char* );
   void output_single_pot_fit( const char* );
   void output_single_pot_fit_old( const char* );
   void output_couple_pot_all( const char* );
   void output_couple_pot_err( const char* );
   void output_couple_pot_fit( const char* );
};

#endif
