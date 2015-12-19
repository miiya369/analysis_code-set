//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup R-correlator
 * @brief   Header file for R-correlator class
 * @author  Takaya Miyamoto
 * @since   Wed Jul 29 02:09:59 JST 2015
 */
//--------------------------------------------------------------------------

#ifndef R_CORRELATOR_H
#define R_CORRELATOR_H

#include <NBSwave/NBSwave.h>
#include <correlator/correlator.h>

//--------------------------------------------------------------------------
/**
 * @brief The class for R-correlator
 */
//--------------------------------------------------------------------------
class R_CORRELATOR : public NBS_WAVE, public CORRELATOR {
    
private:
   string class_name, func_name;
   
   HADRON_TYPE hadron1;
   HADRON_TYPE hadron2;
   
protected:
   
public:
   cdouble *Rcorr;
//============================ For inner index ===========================//

//============================== For writing =============================//
   cdouble& operator()(int x, int y, int z, int conf) {
      return Rcorr[   x + analysis::xSIZE
                   *( y + analysis::ySIZE
                   *( z + analysis::zSIZE * conf ))];
   }
   cdouble& operator()(size_t index) {
      return Rcorr[index];
   }
//============================== For reading =============================//
   const cdouble& operator()(int x, int y, int z, int conf) const {
      return Rcorr[   x + analysis::xSIZE
                   *( y + analysis::ySIZE
                   *( z + analysis::zSIZE * conf ))];
   }
   const cdouble& operator()(size_t index) const {
      return Rcorr[index];
   }
//======================== Constructor & Destructor ======================//
   R_CORRELATOR() {
      class_name = "R_CORRELATOR:CORRELATOR,NBS_WAVE";
      func_name = "______________________";
      analysis::route(class_name, func_name, 1);
      
      Rcorr = NULL;
   }
   ~R_CORRELATOR() {
      if (Rcorr != NULL) delete [] Rcorr;
      
      func_name = "______________________";
      analysis::route(class_name, func_name, 0);
   }
//============================= For initialize ===========================//
   void set_Rcorr(  CHANNEL_TYPE ch, int it, bool endian_FLG
                  , int SPIN, int SPIN_z, int ANG_MOM, bool compress_FLG ) {
      
      func_name = "set_Rcorr_NBS/readcorr";
      analysis::route(class_name, func_name, 1);
      
      NBSwave::xyzSIZE  = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
      NBSwave::xyznSIZE = NBSwave::xyzSIZE * analysis::Nconf;
      
      channel      = ch;
      hadron1      = channel.hadron1;
      hadron2      = channel.hadron2;
      time_slice   = it;
      endian_flg   = endian_FLG;
      compress_flg = compress_FLG;
      spin         = SPIN;
      spin_z       = SPIN_z;
      ang_mom      = ANG_MOM;
      if (spin == 0) spin_z = 0;
      
      input_Rcorr();
      func_name = "set_Rcorr_NBS/readcorr";
      analysis::route(class_name, func_name, 0);
   }
   
   void set_Rcorr(  CHANNEL_TYPE ch,int it,bool endian_FLG,int SPIN, int SPIN_z
                  , int ANG_MOM, cdouble *corr1, cdouble *corr2
                  , bool compress_FLG ) {
      
      func_name = "set_Rcorr_NBS/corr____";
      analysis::route(class_name, func_name, 1);
      
      NBSwave::xyzSIZE  = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
      NBSwave::xyznSIZE = NBSwave::xyzSIZE * analysis::Nconf;
      
      channel      = ch;
      hadron1      = channel.hadron1;
      hadron2      = channel.hadron2;
      time_slice   = it;
      endian_flg   = endian_FLG;
      compress_flg = compress_FLG;
      spin         = SPIN;
      spin_z       = SPIN_z;
      ang_mom      = ANG_MOM;
      if (spin == 0) spin_z = 0;
      
      input_Rcorr( corr1, corr2 );
      func_name = "set_Rcorr_NBS/corr____";
      analysis::route(class_name, func_name, 0);
   }
   
   void set_Rcorr(  CHANNEL_TYPE ch,int it,bool endian_FLG, int SPIN, int SPIN_z
                  , int ANG_MOM, double HAD1_mass, double HAD2_mass
                  , bool compress_FLG ) {
      
      func_name = "set_Rcorr_NBS/mass____";
      analysis::route(class_name, func_name, 1);
      
      NBSwave::xyzSIZE  = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
      NBSwave::xyznSIZE = NBSwave::xyzSIZE * analysis::Nconf;
      
      channel      = ch;
      hadron1      = channel.hadron1;
      hadron2      = channel.hadron2;
      time_slice   = it;
      endian_flg   = endian_FLG;
      compress_flg = compress_FLG;
      spin         = SPIN;
      spin_z       = SPIN_z;
      ang_mom      = ANG_MOM;
      if (spin == 0) spin_z = 0;
      
      input_Rcorr( HAD1_mass, HAD2_mass );
      func_name = "set_Rcorr_NBS/mass____";
      analysis::route(class_name, func_name, 0);
   }
   
   void delete_Rcorr() {
      
      func_name = "delete_Rcorr__________";
      analysis::route(class_name, func_name, 1);
      
      if (Rcorr != NULL) {
         delete [] Rcorr;   Rcorr = NULL;
      }
      analysis::route(class_name, func_name, 0);
   }
//============================ Operator define ===========================//
   
//============================ Operator helper ===========================//
   
//=========================== Several functions ==========================//
   int  info_time(){ return time_slice; }
   
   void input_Rcorr();
   void input_Rcorr( cdouble*, cdouble* );
   void input_Rcorr( double, double );
   void output_Rcorr_all( const char* );
   void output_Rcorr_err( const char*, bool );
};

#endif
