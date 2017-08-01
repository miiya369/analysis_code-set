//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBS wave function
 * @brief   Function for Output NBS wave function files
 * @author  Takaya Miyamoto
 * @since   Wed Oct 12 03:51:03 JST 2016
 */
//--------------------------------------------------------------------------

#include <NBSwave/NBSwave.h>

void NBS_WAVE_ORG::output(const CHANNEL_TYPE ch, const int it, const int iconf) {
   DEBUG_LOG
   
   analysis::set_data_list(N_TIME,        "%03d", it                  );
   analysis::set_data_list(NBS_DIRECTORY, "%s",   ch.directory.c_str());
   analysis::set_data_list(OPER_TYPE,     "%s",   ch.OperType.c_str() );
   
   string NBSfile_name = analysis::set_path(iconf);   // Write NBS wave files
   
   ofstream ofs(NBSfile_name.c_str(), ios::out | ios::binary);
   if (!ofs) ERROR_FOPEN(NBSfile_name.c_str());
   
   cdouble tmp_cmp;
   for (int n=0; n<data_size(); n++) {
      tmp_cmp = wave_org[n];
      
      if (analysis::machine_is_little_endian())
         analysis::endian_convert(&tmp_cmp, 1);
      
      ofs.write((char*)&tmp_cmp, sizeof(cdouble));
   }
   ofs.close();
   
   printf("Finished output NBS: %s, t=%d, conf number=%d\n"
          , ch.name.c_str(), it, iconf);
}
