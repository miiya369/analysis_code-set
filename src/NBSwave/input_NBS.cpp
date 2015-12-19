//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBS wave function
 * @brief   Function for Input NBS wave function files
 * @author  Takaya Miyamoto
 * @since   Wed Jul 29 02:32:15 JST 2015
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

void NBS_WAVE::input_NBS() {
   
   func_name = "input_NBS_&_spin_proj_";
   analysis::route(class_name, func_name, 1);
    
   if (wave == NULL) wave = new cdouble[NBSwave::xyznSIZE];
   cdouble *tmp = new cdouble[NBSwave::xyzSIZE * 4 * 4];
    
   snprintf(         analysis::data_list[N_TIME]
            , sizeof(analysis::data_list[N_TIME])
            , "%d", time_slice);
   snprintf(         analysis::data_list[NBS_DIRECTORY]
            , sizeof(analysis::data_list[NBS_DIRECTORY])
            , "%s", channel.directory.c_str());
   snprintf(         analysis::data_list[N_CHANNEL]
            , sizeof(analysis::data_list[N_CHANNEL])
            , "%s", channel.number.c_str());
   
   time_t start_t, end_t;
   time( &start_t );
   printf(" @ NBSwave read & spin prj |   0%%");
   for (int conf=0; conf<analysis::Nconf; conf++) {   // Read NBS wave files
            
      string NBSfile_name = analysis::set_path( conf );
      ifstream ifs(NBSfile_name.c_str(), ios::in | ios::binary);
      if (!ifs) analysis::error(2, NBSfile_name.c_str());
      
      ifs.read( (char*)&tmp[0], sizeof(cdouble) * NBSwave::xyzSIZE * 4 * 4 );
      ifs.close();
      
      if (endian_flg) analysis::endian_convert(tmp, NBSwave::xyzSIZE * 4 * 4);
      spin_projection(tmp, conf);   // spin projection
        
      printf("\b\b\b\b%3.0lf%%",double(conf+1)/double(analysis::Nconf)*100);
      fflush(stdout);
   } // conf
   time( &end_t );
   printf("  ( %d second )\n",(int)difftime(end_t,start_t));
   printf(" @ Finished input NBS & spin proj. : %s, spin=%d, spin_z=%d, t=%d\n"
          , channel.name.c_str(), spin, spin_z, time_slice);
   delete [] tmp;
   
   analysis::route(class_name, func_name, 0);
}

//--------------------------------------------------------------------------
/**
 * @brief Input compressed-NBS wave function (ishii-san's format)
 * @brief This function is using yuakwa library
 */
//--------------------------------------------------------------------------
void NBS_WAVE::input_compressed_NBS() {

   func_name = "input_compNBS_&SpinPrj";
   analysis::route(class_name, func_name, 1);
    
   if (wave == NULL) wave = new cdouble[NBSwave::xyznSIZE];
   cdouble *tmp = new cdouble[NBSwave::xyzSIZE * 4 * 4];
   
   snprintf(         analysis::data_list[N_TIME]
            , sizeof(analysis::data_list[N_TIME])
            , "%d", time_slice);
   snprintf(         analysis::data_list[NBS_DIRECTORY]
            , sizeof(analysis::data_list[NBS_DIRECTORY])
            , "%s", channel.directory.c_str());
   snprintf(         analysis::data_list[N_CHANNEL]
            , sizeof(analysis::data_list[N_CHANNEL])
            , "%s", channel.number.c_str());
    
   if (!compress_init_flg) {
      cubic_group::initialize();
      PH1::initialize();
      compress_init_flg = true;
   }
   time_t start_t, end_t;
   time( &start_t );
   printf(" @ NBSwave read & spin prj |   0%%");
   for (int conf = 0; conf < analysis::Nconf; conf++) {   // Read NBS wave files
        
      string NBSfile_name = analysis::set_path( conf );
        
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
#define IDX(x,y,z,a,b) a+4*(x+Xsites*(y+Ysites*(z+Zsites*b)))
                           tmp[IDX(ix,iy,iz,alpha+2*beta,alphaP+2*betaP)]
                            = four(ix,iy,iz,sign)(alpha,beta,alphaP,betaP);
#undef IDX
      spin_projection(tmp, conf);   // spin projection
      
      printf("\b\b\b\b%3.0lf%%",double(conf+1)/double(analysis::Nconf)*100);
      fflush(stdout);
   } // conf
   time( &end_t );
   printf("  ( %d second )\n",(int)difftime(end_t,start_t));
   printf(" @ Finished input NBS & spin proj. : %s, spin=%d, spin_z=%d, t=%d\n"
          , channel.name.c_str(), spin, spin_z, time_slice);
   delete [] tmp;
    
   analysis::route(class_name, func_name, 0);
}
