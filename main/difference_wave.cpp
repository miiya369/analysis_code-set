//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup potential
 * @brief   Main part for difference chack of results of hf-code
 * @author  Takaya Miyamoto
 * @since   Thu Jun  2 00:10:23 JST 2016
 */
//--------------------------------------------------------------------------

#include <NBSwave/NBSwave.h>

//========================================================================//
int main(int argc, char** argv)
{
   if (set_args(argc, argv) == 1) return 0;
   
   time_t start_time, end_time;
   time(&start_time);
   
   NBS_WAVE_ORG Wave1, Wave2;

      
      for (int ich=0; ich<N_ch; ich++)
      {
         if (calc_2had_corr)
         {
            analysis::set_data_list(N_TIME, "-1");
            analysis::set_data_list(  HADRON_NAME, "%s"
                                    , channel_type[ich].prop_name.c_str());
            
            analysis::set_data_list(MAIN_PATH, "%s", input_dir1);
            analysis::set_data_list(MAX_PATH_ARG+conf, "%s", tmp_data_lst1[conf]);
            input_corr_all("multi", conf, Corr1);
            
            analysis::set_data_list(MAIN_PATH, "%s", input_dir2);
            analysis::set_data_list(MAX_PATH_ARG+conf, "%s", tmp_data_lst2[conf]);
            input_corr_all("multi", conf, Corr2);
            
            fdiff_swich = 0;
            for (int i=0; i<10*analysis::tSIZE/2; i++)
            {
               if      (abs(Corr1[i]+Corr2[i]) == 0.0     ) count_z++;
               else if (abs(Corr1[i]+Corr2[i]) <  comp_eps) count_CompEps++;
               else
               {
                  tmp_f = abs(2.0 * (Corr1[i]-Corr2[i]) / (Corr1[i]+Corr2[i]));
                  if      (tmp_f == 0.0    ) count_z++;
                  else if (log10(tmp_f) > 0) count_p[ int(log10(tmp_f))]++;
                  else                       count_m[-int(log10(tmp_f))]++;
                  
                  if (tmp_f > precision && fdiff_swich == 0)
                  {
                     fdiff[fdiff_count] = (  channel_type[ich].prop_name
                                            + ", "
                                            + tmp_data_lst1[conf] );
                     fdiff_swich = 1;
                     fdiff_count++;
                     if (fdiff_count == MAX_N_DATA)
                     {
                        printf("ERROR: Too many difference.\n");
                        return -1;
                     }
                  }
               }
               tot_count++;
            }
         }
         
         for (int it=time_min; it<=time_max; it++)
         {
            analysis::set_data_list(MAIN_PATH, "%s", input_dir1);
            analysis::set_data_list(MAX_PATH_ARG+conf, "%s", tmp_data_lst1[conf]);
            Wave1.set(channel_type[ich], it, conf, endian_flg, read_cmp_flg);
            
            analysis::set_data_list(MAIN_PATH, "%s", input_dir2);
            analysis::set_data_list(MAX_PATH_ARG+conf, "%s", tmp_data_lst2[conf]);
            Wave2.set(channel_type[ich], it, conf, endian_flg, read_cmp_flg);
            
            int fdiff_swich_r = 0;
            int fdiff_swich_i = 0;
            for (size_t i=0; i<n_size; i++)
            {
               if      (abs(Wave1(i).real() + Wave2(i).real()) == 0.0     )
                  count_z++;
               else if (abs(Wave1(i).real() + Wave2(i).real()) <  comp_eps)
                  count_CompEps++;
               else
               {
                  tmp_f = 2.0 * abs(  (Wave1(i).real() - Wave2(i).real())
                                    / (Wave1(i).real() + Wave2(i).real()) );
                  
                  if      (tmp_f == 0.0    ) count_z++;
                  else if (log10(tmp_f) > 0) count_p[ int(log10(tmp_f))]++;
                  else                       count_m[-int(log10(tmp_f))]++;
                  
                  if (tmp_f > precision && fdiff_swich_r == 0)
                  {
                     char tmp_ch[3];
                     snprintf(tmp_ch, sizeof(tmp_ch), "%d", it);
                     fdiff[fdiff_count] = (  channel_type[ich].name
                                           + " (t="
                                           + tmp_ch
                                           + ")"
                                           + " [Real] , "
                                           + tmp_data_lst1[conf] );
                     fdiff_swich_r = 1;
                     fdiff_count++;
                     if (fdiff_count == MAX_N_DATA)
                     {
                        printf("ERROR: Too many difference.\n");
                        return -1;
                     }
                  }
               }
               tot_count++;
               
               if      (abs(Wave1(i).imag() + Wave2(i).imag()) == 0.0     )
                  count_z++;
               else if (abs(Wave1(i).imag() + Wave2(i).imag()) <  comp_eps)
                  count_CompEps++;
               else
               {
                  tmp_f = 2.0 * abs(  (Wave1(i).imag() - Wave2(i).imag())
                                    / (Wave1(i).imag() + Wave2(i).imag()) );
                  
                  if      (tmp_f == 0.0    ) count_z++;
                  else if (log10(tmp_f) > 0) count_p[ int(log10(tmp_f))]++;
                  else                       count_m[-int(log10(tmp_f))]++;
                  
                  if (tmp_f > precision && fdiff_swich_i == 0)
                  {
                     char tmp_ch[3];
                     snprintf(tmp_ch, sizeof(tmp_ch), "%d", it);
                     fdiff[fdiff_count] = (  channel_type[ich].name
                                           + " (t="
                                           + tmp_ch
                                           + ")"
                                           + " [Imag] , "
                                           + tmp_data_lst1[conf] );
                     fdiff_swich_i = 1;
                     fdiff_count++;
                     if (fdiff_count == MAX_N_DATA)
                     {
                        printf("ERROR: Too many difference.\n");
                        return -1;
                     }
                  }
               }
               tot_count++;
            }

   return 0;
}
//========================================================================//

static int set_args(int argc, char** argv)
{
   if (argc == 1)
   {
      analysis::usage(PROJECT);
      return 1;
   }

   return 0;
}
