//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBS wave function
 * @brief   Function for Input NBS wave function files
 * @author  Takaya Miyamoto
 * @since   Wed Sep  2 03:19:21 JST 2015
 */
//--------------------------------------------------------------------------

#include <NBSwave/NBSwave.h>

#include <yukawa/compress48.h>
#include <yukawa/PH1.h>

using namespace yukawa;
typedef four_point_base::BC BC;
typedef PH1                 Core;
typedef four_point<Core>    Four_Point;
typedef compress48<Core>    Compress48;

bool NBSwave::compress_init_flg = false;

void NBS_WAVE_ORG::input( CHANNEL_TYPE ch, int it, int iconf, bool endian_FLG ) {
   
   func_name = "input_NBS_____________";
   analysis::route(class_name, func_name, 1);
   
   size_t xyzSIZE = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
    
   snprintf(         analysis::data_list[N_TIME]
            , sizeof(analysis::data_list[N_TIME])
            , "%d", it);
   snprintf(         analysis::data_list[NBS_DIRECTORY]
            , sizeof(analysis::data_list[NBS_DIRECTORY])
            , "%s", ch.directory.c_str());
   snprintf(         analysis::data_list[N_CHANNEL]
            , sizeof(analysis::data_list[N_CHANNEL])
            , "%s", ch.number.c_str());
            
   string NBSfile_name = analysis::set_path( iconf );   // Read NBS wave files
   ifstream ifs(NBSfile_name.c_str(), ios::in | ios::binary);
   if (!ifs) analysis::error(2, NBSfile_name.c_str());
   
   ifs.read( (char*)&wave_org[0], sizeof(cdouble) * xyzSIZE * 4 * 4 );
   ifs.close();
   
   if (endian_FLG) analysis::endian_convert(wave_org, xyzSIZE * 4 * 4);

   printf(" @ Finished input NBS : %s, t=%d, conf number=%d\n"
          , ch.name.c_str(), it, iconf);
   
   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Input compressed-NBS wave function (ishii-san's format)
 * @brief This function is using yuakwa library
 */
//--------------------------------------------------------------------------
void NBS_WAVE_ORG::input_compressed(  CHANNEL_TYPE ch
                                    , int it, int iconf, bool endian_FLG ) {

   func_name = "input_compNBS_________";
   analysis::route(class_name, func_name, 1);
   
   snprintf(         analysis::data_list[N_TIME]
            , sizeof(analysis::data_list[N_TIME])
            , "%d", it);
   snprintf(         analysis::data_list[NBS_DIRECTORY]
            , sizeof(analysis::data_list[NBS_DIRECTORY])
            , "%s", ch.directory.c_str());
   snprintf(         analysis::data_list[N_CHANNEL]
            , sizeof(analysis::data_list[N_CHANNEL])
            , "%s", ch.number.c_str());
   
   if (!NBSwave::compress_init_flg) {
      cubic_group::initialize();
      PH1::initialize();
      NBSwave::compress_init_flg = true;
   }
        
   string NBSfile_name = analysis::set_path( iconf );   // Read NBS wave files
   
   mapping48 map;
   map.read(NBSfile_name.c_str());
   Compress48 comp;
   comp.read(NBSfile_name.c_str(), map);
   BC Xbc = comp.Xbc;
   BC Ybc = comp.Ybc;
   BC Zbc = comp.Zbc;
   map.construct_map_and_phase(Xbc, Ybc, Zbc);
   Four_Point four = comp.decompress48();
   int Xsites = four.Xsites;
   int Ysites = four.Ysites;
   int Zsites = four.Zsites;
   int sign   = 1;
   for(                  int betaP =0; betaP <2;       betaP++)
      for(               int alphaP=0; alphaP<2;       alphaP++)
         for(            int iz    =0; iz    <Zsites;  iz++)
            for(         int iy    =0; iy    <Ysites;  iy++)
               for(      int ix    =0; ix    <Xsites;  ix++)
                  for(   int beta  =0; beta  <2;       beta++)
                     for(int alpha =0; alpha <2;       alpha++)
                        wave_org[idx(ix,iy,iz,alpha+2*beta,alphaP+2*betaP)]
                              = four(ix,iy,iz,sign)(alpha,beta,alphaP,betaP);
   
   printf(" @ Finished input NBS : %s, t=%d, conf number=%d\n"
          , ch.name.c_str(), it, iconf);
    
   analysis::route(class_name, func_name, 0);
}
