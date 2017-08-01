//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup scattering length
 * @brief   Function for input phase shift data
 * @author  Takaya Miyamoto
 * @since   Wed Sep 23 18:42:31 JST 2015
 */
//--------------------------------------------------------------------------

#include <observable/phase_shift.h>

//--------------------------------------------------------------------------
/**
 * @brief The function for input binary-phase shift data (for header)
 */
//--------------------------------------------------------------------------
void phase_shift::input_phase_shift(  const char* infile_name
                                   , int &n_conf, int &n_data ) {
   DEBUG_LOG
   
   ifstream ifs(infile_name, ios::in | ios::binary);
   if(!ifs) ERROR_FOPEN(infile_name);
   
   int tmp_Nconf, tmp_Ndata, itmp;
   
   ifs.read((char*)&tmp_Nconf, sizeof(int));
   ifs.read((char*)&tmp_Ndata, sizeof(int));   // read file header
   ifs.read((char*)&itmp     , sizeof(int));
   
   if (!analysis::machine_is_little_endian()) {
      analysis::endian_convert(&tmp_Nconf, 1);
      analysis::endian_convert(&tmp_Ndata, 1);
      analysis::endian_convert(&itmp     , 1);
   }
   printf(" @ #.confs = %d\n", tmp_Nconf);
   printf(" @ #.datas = %d\n", tmp_Ndata);
   ifs.close();
   
   if (itmp != sizeof(double))
      ERROR_COMMENTS("May be this is not correct "
                     "(Miyamoto-format) phase-shift file.");
   n_conf = tmp_Nconf;
   n_data = tmp_Ndata;
}

//--------------------------------------------------------------------------
/**
 * @brief The function for input binary-phase shift data (for body)
 */
//--------------------------------------------------------------------------
void phase_shift::input_phase_shift(  const char* infile_name
                                   , double *energy, double *phase ) {
   DEBUG_LOG
   
   ifstream ifs(infile_name, ios::in | ios::binary);
   if(!ifs) ERROR_FOPEN(infile_name);
   
   int tmp_Nconf, tmp_Ndata, itmp;
   double dtmp;
   
   ifs.read((char*)&tmp_Nconf, sizeof(int));
   ifs.read((char*)&tmp_Ndata, sizeof(int));   // read file header
   ifs.read((char*)&itmp     , sizeof(int));
   
   if (!analysis::machine_is_little_endian()) {
      analysis::endian_convert(&tmp_Nconf, 1);
      analysis::endian_convert(&tmp_Ndata, 1);
      analysis::endian_convert(&itmp     , 1);
   }
   if (itmp != sizeof(double))
      ERROR_COMMENTS("May be this is not correct "
                     "(Miyamoto-format) phase-shift file.");
   
   for (int n=0; n<tmp_Ndata; n++) {
      ifs.read((char*)&dtmp, sizeof(double));
      if (!analysis::machine_is_little_endian())
         analysis::endian_convert(&dtmp, 1);
      energy[n] = dtmp;
   }
   for (int i=0; i<tmp_Nconf; i++)
      for (int n=0; n<tmp_Ndata; n++) {
         ifs.read((char*)&dtmp, sizeof(double));
         if (!analysis::machine_is_little_endian())
            analysis::endian_convert(&dtmp, 1);
         phase[n+tmp_Ndata*i] = dtmp;
      }
   ifs.close();
}
