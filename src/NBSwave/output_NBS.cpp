//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBS wave function
 * @brief   Function for Output NBS wave function files
 * @author  Takaya Miyamoto
 * @since   Thu Jun  2 13:23:37 JST 2016
 */
//--------------------------------------------------------------------------

#include <NBSwave/NBSwave.h>

/*
#include <yukawa/compress48.h>
#include <yukawa/PH1.h>


using namespace yukawa;
typedef four_point_base::BC BC;
typedef PH1                 Core;
typedef four_point<Core>    Four_Point;
typedef compress48<Core>    Compress48;
*/

void NBS_WAVE_ORG::output(  const CHANNEL_TYPE ch, const int it, const int iconf
                          , const bool endian_FLG )
{
   func_name = "output_NBS____________";
   analysis::route(class_name, func_name, 1);
   
   size_t xyzSIZE = analysis::xSIZE * analysis::ySIZE * analysis::zSIZE;
    
   snprintf(         analysis::data_list[N_TIME]
            , sizeof(analysis::data_list[N_TIME])
            , "%03d", it);
   snprintf(         analysis::data_list[NBS_DIRECTORY]
            , sizeof(analysis::data_list[NBS_DIRECTORY])
            , "%s", ch.directory.c_str());
   snprintf(         analysis::data_list[N_CHANNEL]
            , sizeof(analysis::data_list[N_CHANNEL])
            , "%s", ch.number.c_str());
            
   string NBSfile_name = analysis::set_path( iconf );   // Read NBS wave files
   ofstream ofs(NBSfile_name.c_str(), ios::out | ios::binary);
   if (!ofs) analysis::error(2, NBSfile_name.c_str());
   
   cdouble tmp_cf;
   for (size_t n=0; n<xyzSIZE * 4 * 4; n++)
   {
      tmp_cf = wave_org[n];
      if (endian_FLG) analysis::endian_convert(&tmp_cf, 1);
   
      ofs.write((char*)&tmp_cf, sizeof(cdouble));
   }
   ofs.close();

   printf("Finished output NBS : %s, t=%d, conf number=%d\n"
          , ch.name.c_str(), it, iconf);
   
   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Output compressed-NBS wave function (ishii-san's format)
 * @brief This function is using yuakwa library
 */
//--------------------------------------------------------------------------
/*
void NBS_WAVE_ORG::output_compress(  const CHANNEL_TYPE ch, const int it
                                   , const int iconf, const bool endian_FLG )
{
   func_name = "output_compNBS________";
   analysis::route(class_name, func_name, 1);
   
   snprintf(         analysis::data_list[N_TIME]
            , sizeof(analysis::data_list[N_TIME])
            , "%03d", it);
   snprintf(         analysis::data_list[NBS_DIRECTORY]
            , sizeof(analysis::data_list[NBS_DIRECTORY])
            , "%s", ch.directory.c_str());
   snprintf(         analysis::data_list[N_CHANNEL]
            , sizeof(analysis::data_list[N_CHANNEL])
            , "%s", ch.number.c_str());
   
   if (!NBSwave::compress_init_flg)
   {
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
                        four(ix,iy,iz,sign)(alpha,beta,alphaP,betaP)
                        = wave_org[idx(ix,iy,iz,alpha+2*beta,alphaP+2*betaP)];
   
   printf("Finished output NBS : %s, t=%d, conf number=%d\n"
          , ch.name.c_str(), it, iconf);
    
   analysis::route(class_name, func_name, 0);
}
*/
