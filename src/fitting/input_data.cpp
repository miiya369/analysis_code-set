//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup fitting
 * @brief   Function for input the ( miyamoto-format ) fit data
 * @author  Takaya Miyamoto
 * @since   Wed Jul 22 20:58:00 JST 2015
 */
//--------------------------------------------------------------------------

#include <fitting/fitting.h>

void FIT::input_data( const char *infile_name ) {
   
   func_name = "input_data____________";
   analysis::route(class_name, func_name, 1);
   
   if (new_flg_data){
      delete_data();
      func_name = "input_data____________";
   }
   printf(" @ Fit data reading |   0%%");
   ifstream ifs(infile_name, ios::in | ios::binary);
   if (!ifs) analysis::error(2, infile_name);
   
//======================== miyamoto-format notation =======================//
//
//                        !! ALWAYS LITTLE ENDIAN !!
//
//      1) fit type (int) -> 1 = corr type, 2 = pot type
//      2) #.conf   (int)
//      3) #.data   (int)
//
//      4) data     (double float)
//         -> for i = 0 to #.data {    |   for i = 0 to #.data {
//               err[i]                |      cood[i]
//               for n = 0 to #.conf   |       err[i]
//                  data[n+#.conf*i]   |      for n = 0 to #.conf
//            }                        |         data[n+#.conf*i]
//                                     |   }
//                 (at fit type = 1)   |        (at fit type = 2)
//
//=========================================================================//
   ifs.read((char*)&fit_type       , sizeof(int));
   ifs.read((char*)&analysis::Nconf, sizeof(int));   // read file header
   ifs.read((char*)&Ndata          , sizeof(int));
   
   if (!analysis::machine_is_little_endian()) {
      analysis::endian_convert(&fit_type, 1);
      analysis::endian_convert(&analysis::Nconf, 1);
      analysis::endian_convert(&Ndata, 1);
   }
//=========================== read corr-type data =========================//
   if(fit_type == 1) {
      cood = new double[Ndata];
      data = new double[Ndata * analysis::Nconf];
      err  = new double[Ndata];
      new_flg_data = true;
      
      for (int t=0; t<Ndata; t++) {
         cood[t] = t;
         ifs.read((char*)&err[t], sizeof(double));
         for (int i=0; i<analysis::Nconf; i++)
            ifs.read((char*)&data[idx(i,t)], sizeof(double));
         
         printf("\b\b\b\b%3.0lf%%",double(t+1)/double(Ndata)*100);
         fflush(stdout);
      }
      printf("\n");
      if (!analysis::machine_is_little_endian()) {
         analysis::endian_convert(data, Ndata * analysis::Nconf);
         analysis::endian_convert(err,  Ndata);
      }
//========================= read potential-type data ======================//
   }else if(fit_type == 2) {
      cood = new double[Ndata];
      data = new double[Ndata * analysis::Nconf];
      err  = new double[Ndata];
      new_flg_data = true;
      
      for (int r=0; r<Ndata; r++) {
         ifs.read((char*)&cood[r], sizeof(double));
         ifs.read((char*)&err[r] , sizeof(double));
         for (int i=0; i<analysis::Nconf; i++)
            ifs.read((char*)&data[idx(i,r)], sizeof(double));
         
         printf("\b\b\b\b%3.0lf%%",double(r+1)/double(Ndata)*100);
         fflush(stdout);
      }
      printf("\n");
      if(!analysis::machine_is_little_endian()){
         analysis::endian_convert(cood, Ndata);
         analysis::endian_convert(data, Ndata * analysis::Nconf);
         analysis::endian_convert(err,  Ndata);
      }
   }
//========================================================================//
   else{
      printf("\n @ fit type         = %d\n", fit_type);
      printf(" @ #.confs          = %d\n", analysis::Nconf);
      printf(" @ #.data size      = %d\n", Ndata);
      ifs.close();
      analysis::error(3,"This file is not miyamoto-format fit data");
   }
   printf(" @ fit type         = %d\n", fit_type);
   printf(" @ #.confs          = %d\n", analysis::Nconf);
   printf(" @ #.data size      = %d\n", Ndata);
   ifs.close();
   
   analysis::route(class_name, func_name, 0);
}
