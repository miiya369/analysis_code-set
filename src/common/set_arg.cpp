//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup All
 * @brief   Function for argument setting
 * @author  Takaya Miyamoto
 */
//--------------------------------------------------------------------------

#include <analysis.h>

int set_arg( int usage_type, int argc, char **argv
            , char  argv_o[MAX_N_ARG][MAX_LEN_PATH]
            , char  list[MAX_N_DATA][MAX_LEN_PATH] ){
    
    bool infile_flg = false;
    int  n_arg,check,count1=0,count2=0;
    char infile_name[MAX_LEN_PATH];
    char tmp_str[MAX_N_DATA][MAX_LEN_PATH];
    char tmp_c1[MAX_LEN_PATH], tmp_c2[MAX_LEN_PATH];
    
//===========================================================================//
//============================ For extract Z-factor =========================//
//===========================================================================//
    if( usage_type == EXTRACT_Z_FACTOR ){
        n_arg = 11;   // <- If you change the number of args, change also here.
        char option0[32][32]
    ={ "ZFAC_Gauge_confs_list","ZFAC_Path_to_input_dir","ZFAC_Path_to_output_dir"
      ,"ZFAC_Size_of_time"    ,"ZFAC_T_shift"          ,"ZFAC_X_shift"
      ,"ZFAC_Y_shift"         ,"ZFAC_Z_shift"          ,"ZFAC_Snk_relativistic"
      ,"ZFAC_Src_relativistic","ZFAC_Calc_hadron_name" };
        // "ZFAC_Calc_hadron_name" must be put at the end of option0 !
        
        for( int loop=1; loop<argc; loop++ )
            if( argv[loop][0] == '-' )
                if(argv[loop][1]=='f'){
                    snprintf(infile_name,sizeof(infile_name),"%s",argv[loop+1]);
                    count1++;   infile_flg = true;
                }
        if( infile_flg ){
            ifstream ifs(infile_name, ios::in );
            if(!ifs){
                printf("\n @@@@@@ the file \"%s\" is not exist\n\n", infile_name );
                return -1;
            }
            while(ifs.getline(tmp_str[count2],sizeof(tmp_str[count2]))) count2++;
            for( int loop1=0; loop1<count2; loop1++ ){
                if(sscanf(tmp_str[loop1]," %s = %s ",tmp_c1,tmp_c2) != 2 )
                    continue;
                for( int loop2=0; loop2<n_arg-1; loop2++ )
                    if( strcmp(tmp_c1,option0[loop2]) == 0 )
                        snprintf(argv_o[loop2],sizeof(argv_o[loop2]),"%s",tmp_c2);
                
                if( strcmp(tmp_c1,option0[n_arg-1]) == 0 ){
                    char *tmp_tok;
                    int   count3 = 0;
                    tmp_tok = strtok(tmp_str[loop1]," \t");
                    for(int iii=0;iii<3;iii++) tmp_tok = strtok(NULL," \t");
                    while( strcmp(tmp_tok,"}") != 0 ){
                        snprintf(argv_o[count3+n_arg],sizeof(argv_o[count3+n_arg])
                                 ,"%s",tmp_tok);
                        if(name_to_hadron_type(argv_o[count3+n_arg]) == -1){
                            printf("\n @@@@@@ invalid hadron name \" %s \"\n\n"
                                   ,argv_o[count3+n_arg]);
                            return -1;
                        }
                        count3++;
                        tmp_tok = strtok(NULL," \t");
                    }
                    snprintf(argv_o[n_arg-1],sizeof(argv_o[n_arg-1]),"%d",count3);
                }
            }
        }
        for( int loop=1; loop<argc; loop++ )
            if( argv[loop][0] == '-' ){
                check = 0;
                //****** You may set additional potion in here ******//
                
                //***************************************************//
                if(argv[loop][1]=='f') check++;
                if(check==0){
                    printf("\n @@@@@@ invalid option -%c\n", argv[loop][1]);
                    usage( usage_type );
                    return -1;
                }
            }
        snprintf(list[MAIN_PATH],sizeof(list[MAIN_PATH]) ,"%s",argv_o[1]);
        snprintf(list[N_T_SHIFT],sizeof(list[N_T_SHIFT]) ,"%s",argv_o[4]);
        snprintf(list[N_X_SHIFT],sizeof(list[N_X_SHIFT]) ,"%s",argv_o[5]);
        snprintf(list[N_Y_SHIFT],sizeof(list[N_Y_SHIFT]) ,"%s",argv_o[6]);
        snprintf(list[N_Z_SHIFT],sizeof(list[N_Z_SHIFT]) ,"%s",argv_o[7]);
        snprintf(list[SNK_RELA],sizeof(list[SNK_RELA]) ,"%s",argv_o[8]);
        snprintf(list[SRC_RELA],sizeof(list[SRC_RELA]) ,"%s",argv_o[9]);
    }
//===========================================================================//
//================ For coupled channel potential calculation ================//
//===========================================================================//
    if( usage_type == COUPLED_CHANNEL_POT ){
        n_arg = 25;   // <- If you change the number of args, change also here.
        char option1[32][32]
        = {  "CCP_Size_of_time"    ,"CCP_Size_of_xyz"     ,"CCP_calc_time_slice"
            ,"CCP_Spin_projection" ,"CCP_Spin_z_component","CCP_Ang_mom_projection"
            ,"CCP_Gauge_confs_list","CCP_Path_to_output_dir","CCP_Channel_00"
            ,"CCP_Channel_01"      ,"CCP_Channel_10"      ,"CCP_Channel_11"
            ,"CCP_Reduced_mass_00" ,"CCP_Reduced_mass_01" ,"CCP_Reduced_mass_10"
            ,"CCP_Reduced_mass_11" ,"CCP_Path_to_input_dir","CCP_T_shift"
            ,"CCP_X_shift"         ,"CCP_Y_shift"         ,"CCP_Z_shift"
            ,"CCP_Endian_convert"  ,"CCP_Calc_fit_data"   ,"CCP_Snk_relativistic"
            ,"CCP_Src_relativistic" };
        
        for( int loop=1; loop<argc; loop++ )
            if( argv[loop][0] == '-' )
                if(argv[loop][1]=='f'){
                    snprintf(infile_name,sizeof(infile_name),"%s",argv[loop+1]);
                    count1++;   infile_flg = true;
                }
        if( infile_flg ){
            ifstream ifs(infile_name, ios::in );
            if(!ifs){
                printf("\n @@@@@@ the file \"%s\" is not exist\n\n", infile_name );
                return -1;
            }
            while(ifs.getline(tmp_str[count2],sizeof(tmp_str[count2]))) count2++;
            for( int loop1=0; loop1<count2; loop1++ ){
                if(sscanf(tmp_str[loop1]," %s = %s ",tmp_c1,tmp_c2) != 2 )
                    continue;
                for( int loop2=0; loop2<n_arg; loop2++ )
                    if( strcmp(tmp_c1,option1[loop2]) == 0 )
                        snprintf(argv_o[loop2],sizeof(argv_o[loop2]),"%s",tmp_c2);
            }
            snprintf(list[MAIN_PATH],sizeof(list[MAIN_PATH]) ,"%s",argv_o[16]);
            snprintf(list[N_T_SHIFT],sizeof(list[N_T_SHIFT]) ,"%s",argv_o[17]);
            snprintf(list[N_X_SHIFT],sizeof(list[N_X_SHIFT]) ,"%s",argv_o[18]);
            snprintf(list[N_Y_SHIFT],sizeof(list[N_Y_SHIFT]) ,"%s",argv_o[19]);
            snprintf(list[N_Z_SHIFT],sizeof(list[N_Z_SHIFT]) ,"%s",argv_o[20]);
            snprintf(list[SNK_RELA],sizeof(list[SNK_RELA]) ,"%s",argv_o[23]);
            snprintf(list[SRC_RELA],sizeof(list[SRC_RELA]) ,"%s",argv_o[24]);
            for(int iii=0;iii<4;iii++)
                if(name_to_channel(argv_o[8+iii]) == -1){
                    printf("\n @@@@@@ invalid channel \" %s \"\n\n",argv_o[8+iii]);
                    return -1;
                }
        }
        for( int loop=1; loop<argc; loop++ )
            if( argv[loop][0] == '-' ){
                check = 0;
                //****** You may set additional potion in here ******//
                if(argv[loop][1]=='t'){
                    snprintf(argv_o[2],sizeof(argv_o[2]),"%s",argv[loop+1]);
                    check++;   count1++;
                }
                if(argv[loop][1]=='^'){
                    snprintf(argv_o[n_arg],sizeof(argv_o[n_arg]),"y");
                    check++;   count1++;
                }
                //***************************************************//
                if(argv[loop][1]=='f') check++;
                if(check==0){
                    printf("\n @@@@@@ invalid option -%c\n", argv[loop][1]);
                    usage( usage_type );
                    return -1;
                }
            }
    }
//===========================================================================//
//====================== For effective mass calculation =====================//
//===========================================================================//
    if( usage_type == EFFECTIVEMASS ){
        n_arg = 15;   // <- If you change the number of args, change also here.
        char option2[32][32]
        = {  "MAS_Size_of_time"   ,"MAS_Gauge_confs_list","MAS_Path_to_output_dir"
            ,"MAS_Fit_range_min"    ,"MAS_Fit_range_max" ,"MAS_Calc_fit_data"
            ,"MAS_Path_to_input_dir","MAS_T_shift"       ,"MAS_X_shift"
            ,"MAS_Y_shift"          ,"MAS_Z_shift"       ,"MAS_Snk_relativistic"
            ,"MAS_Src_relativistic","MAS_Lattice_spacing","MAS_Calc_hadron_name"};
        // "MAS_Calc_hadron_name" must be put at the end of option2 !
        
        for( int loop=1; loop<argc; loop++ )
            if( argv[loop][0] == '-' )
                if(argv[loop][1]=='f'){
                    snprintf(infile_name,sizeof(infile_name),"%s",argv[loop+1]);
                    count1++;   infile_flg = true;
                }
        if( infile_flg ){
            ifstream ifs(infile_name, ios::in );
            if(!ifs){
                printf("\n @@@@@@ the file \"%s\" is not exist\n\n", infile_name );
                return -1;
            }
            while(ifs.getline(tmp_str[count2],sizeof(tmp_str[count2]))) count2++;
            for( int loop1=0; loop1<count2; loop1++ ){
                if(sscanf(tmp_str[loop1]," %s = %s ",tmp_c1,tmp_c2) != 2 )
                    continue;
                for( int loop2=0; loop2<n_arg-1; loop2++ )
                    if( strcmp(tmp_c1,option2[loop2]) == 0 )
                        snprintf(argv_o[loop2],sizeof(argv_o[loop2]),"%s",tmp_c2);
                
                if( strcmp(tmp_c1,option2[n_arg-1]) == 0 ){
                    char *tmp_tok;
                    int   count3 = 0;
                    tmp_tok = strtok(tmp_str[loop1]," \t");
                    for(int iii=0;iii<3;iii++) tmp_tok = strtok(NULL," \t");
                    while( strcmp(tmp_tok,"}") != 0 ){
                        snprintf(argv_o[count3+n_arg],sizeof(argv_o[count3+n_arg])
                                 ,"%s",tmp_tok);
                        if(name_to_hadron_type(argv_o[count3+n_arg]) == -1){
                            printf("\n @@@@@@ invalid hadron name \" %s \"\n\n"
                                   ,argv_o[count3+n_arg]);
                            return -1;
                        }
                        count3++;
                        tmp_tok = strtok(NULL," \t");
                    }
                    snprintf(argv_o[n_arg-1],sizeof(argv_o[n_arg-1]),"%d",count3);
                }
            }
        }
        for( int loop=1; loop<argc; loop++ )
            if( argv[loop][0] == '-' ){
                check = 0;
                //****** You may set additional potion in here ******//
                if(argv[loop][1]=='i'){
                    snprintf(argv_o[6],sizeof(argv_o[6]),"%s",argv[loop+1]);
                    check++;   count1++;
                }
                if(argv[loop][1]=='o'){
                    snprintf(argv_o[2],sizeof(argv_o[2]),"%s",argv[loop+1]);
                    check++;   count1++;
                }
                if(argv[loop][1]=='m'){
                    snprintf(argv_o[5],sizeof(argv_o[5]),"yes");
                    check++;   count1++;
                }
                if(argv[loop][1]=='t'){
                    snprintf(argv_o[3],sizeof(argv_o[3]),"%s",argv[loop+1]);
                    check++;   count1++;
                }
                if(argv[loop][1]=='T'){
                    snprintf(argv_o[4],sizeof(argv_o[4]),"%s",argv[loop+1]);
                    check++;   count1++;
                }
                //***************************************************//
                if(argv[loop][1]=='f') check++;
                if(check==0){
                    printf("\n @@@@@@ invalid option -%c\n", argv[loop][1]);
                    usage( usage_type );
                    return -1;
                }
            }
        snprintf(list[MAIN_PATH],sizeof(list[MAIN_PATH]) ,"%s",argv_o[6]);
        snprintf(list[N_T_SHIFT],sizeof(list[N_T_SHIFT]) ,"%s",argv_o[7]);
        snprintf(list[N_X_SHIFT],sizeof(list[N_X_SHIFT]) ,"%s",argv_o[8]);
        snprintf(list[N_Y_SHIFT],sizeof(list[N_Y_SHIFT]) ,"%s",argv_o[9]);
        snprintf(list[N_Z_SHIFT],sizeof(list[N_Z_SHIFT]) ,"%s",argv_o[10]);
        snprintf(list[SNK_RELA] ,sizeof(list[SNK_RELA])  ,"%s",argv_o[11]);
        snprintf(list[SRC_RELA] ,sizeof(list[SRC_RELA])  ,"%s",argv_o[12]);
    }
//===========================================================================//
//======================== For potential calculation ========================//
//===========================================================================//
    if( usage_type == CALC_POTENTIAL ){
        n_arg = 28;   // <- If you change the number of args, change also here.
        char option3[32][32]
        = {  "POT_Size_of_xyz"     ,"POT_Min_time_slice"  ,"POT_Max_time_slice"
            ,"POT_Channel"         ,"POT_Spin_projection" ,"POT_Spin_z_component"
        ,"POT_Ang_mom_projection","POT_Gauge_confs_list"  ,"POT_Path_to_output_dir"
            ,"POT_Endian_convert"  ,"POT_Out_sum_pot"     ,"POT_Out_laplacian"
            ,"POT_Out_1st_t_diff"  ,"POT_Out_2nd_t_diff"  ,"POT_Out_fit_data"
            ,"POT_Path_to_input_dir","POT_T_shift"        ,"POT_X_shift"
            ,"POT_Y_shift"         ,"POT_Z_shift"         ,"POT_Size_of_time"
            ,"POT_Snk_relativistic","POT_Src_relativistic","POT_Out_NBS_wave"
            ,"POT_Out_R_correlator","POT_Rcorr_NBS/corr"  ,"POT_Had1_mass"
            ,"POT_Had2_mass" };
        
        for( int loop=1; loop<argc; loop++ )
            if( argv[loop][0] == '-' )
                if(argv[loop][1]=='f'){
                    snprintf(infile_name,sizeof(infile_name),"%s",argv[loop+1]);
                    count1++;   infile_flg = true;
                }
        if( infile_flg ){
            ifstream ifs(infile_name, ios::in );
            if(!ifs){
                printf("\n @@@@@@ the file \"%s\" is not exist\n\n", infile_name );
                return -1;
            }
            while(ifs.getline(tmp_str[count2],sizeof(tmp_str[count2]))) count2++;
            for( int loop1=0; loop1<count2; loop1++ ){
                if(sscanf(tmp_str[loop1]," %s = %s ",tmp_c1,tmp_c2) != 2 )
                    continue;
                for( int loop2=0; loop2<n_arg; loop2++ )
                    if( strcmp(tmp_c1,option3[loop2]) == 0 )
                        snprintf(argv_o[loop2],sizeof(argv_o[loop2]),"%s",tmp_c2);
            }
            snprintf(list[MAIN_PATH],sizeof(list[MAIN_PATH]) ,"%s",argv_o[15]);
            snprintf(list[N_T_SHIFT],sizeof(list[N_T_SHIFT]) ,"%s",argv_o[16]);
            snprintf(list[N_X_SHIFT],sizeof(list[N_X_SHIFT]) ,"%s",argv_o[17]);
            snprintf(list[N_Y_SHIFT],sizeof(list[N_Y_SHIFT]) ,"%s",argv_o[18]);
            snprintf(list[N_Z_SHIFT],sizeof(list[N_Z_SHIFT]) ,"%s",argv_o[19]);
            snprintf(list[SNK_RELA] ,sizeof(list[SNK_RELA])  ,"%s",argv_o[21]);
            snprintf(list[SRC_RELA] ,sizeof(list[SRC_RELA])  ,"%s",argv_o[22]);
            if(name_to_channel(argv_o[3]) == -1){
                printf("\n @@@@@@ invalid channel \" %s \"\n\n",argv_o[3]);
                return -1;
            }
        }
        for( int loop=1; loop<argc; loop++ )
            if( argv[loop][0] == '-' ){
                check = 0;
                //****** You may set additional potion in here ******//
                if(argv[loop][1]=='t'){
                    snprintf(argv_o[1],sizeof(argv_o[1]),"%s",argv[loop+1]);
                    check++;   count1++;
                }
                if(argv[loop][1]=='T'){
                    snprintf(argv_o[2],sizeof(argv_o[2]),"%s",argv[loop+1]);
                    check++;   count1++;
                }
                if(argv[loop][1]=='c'){
                    snprintf(argv_o[3],sizeof(argv_o[3]),"%s",argv[loop+1]);
                    check++;   count1++;
                }
                if(argv[loop][1]=='m'){
                    snprintf(argv_o[26],sizeof(argv_o[26]),"%s",argv[loop+1]);
                    check++;   count1++;
                }
                if(argv[loop][1]=='M'){
                    snprintf(argv_o[27],sizeof(argv_o[27]),"%s",argv[loop+1]);
                    check++;   count1++;
                }
                //***************************************************//
                if(argv[loop][1]=='f') check++;
                if(check==0){
                    printf("\n @@@@@@ invalid option -%c\n", argv[loop][1]);
                    usage( usage_type );
                    return -1;
                }
            }
    }
//===========================================================================//
//============================= For data fitting ============================//
//===========================================================================//
    if( usage_type == FITTING_DATA ){
        n_arg = 9;   // <- If you change the number of args, change also here.
        char option4[32][32]
        = {  "FIT_Stopping_condition","FIT_Function_type"  ,"FIT_Fit_range_min"
            ,"FIT_Fit_range_max" ,"FIT_Fit_data_file_name" ,"FIT_Output_file_name"
            ,"FIT_Endian_convert"    ,"FIT_lattice_spacing","FIT_Parameter" };
        // "FIT_Parameter" must be put at the end of option4 !
        
        for( int loop=1; loop<argc; loop++ )
            if( argv[loop][0] == '-' )
                if(argv[loop][1]=='f'){
                    snprintf(infile_name,sizeof(infile_name),"%s",argv[loop+1]);
                    count1++;   infile_flg = true;
                }
        if( infile_flg ){
            ifstream ifs(infile_name, ios::in );
            if(!ifs){
                printf("\n @@@@@@ the file \"%s\" is not exist\n\n", infile_name );
                return -1;
            }
            while(ifs.getline(tmp_str[count2],sizeof(tmp_str[count2]))) count2++;
            for( int loop1=0; loop1<count2; loop1++ ){
                if(sscanf(tmp_str[loop1]," %s = %s ",tmp_c1,tmp_c2) != 2 )
                    continue;
                for( int loop2=0; loop2<n_arg-1; loop2++ )
                    if( strcmp(tmp_c1,option4[loop2]) == 0 )
                        snprintf(argv_o[loop2],sizeof(argv_o[loop2]),"%s",tmp_c2);
                
                if( strcmp(tmp_c1,option4[n_arg-1]) == 0 ){
                    char *tmp_tok;
                    int   count3 = 0;
                    tmp_tok = strtok(tmp_str[loop1]," \t");
                    for(int iii=0;iii<3;iii++) tmp_tok = strtok(NULL," \t");
                    while( strcmp(tmp_tok,"}") != 0 ){
                        snprintf(argv_o[count3+n_arg],sizeof(argv_o[count3+n_arg])
                                 ,"%s",tmp_tok);
                        count3++;
                        tmp_tok = strtok(NULL," \t");
                    }
                    snprintf(argv_o[n_arg-1],sizeof(argv_o[n_arg-1]),"%d",count3);
                }
            }
        }
        for( int loop=1; loop<argc; loop++ )
            if( argv[loop][0] == '-' ){
                check = 0;
                //****** You may set additional potion in here ******//
                if(argv[loop][1]=='i'){
                    snprintf(argv_o[4],sizeof(argv_o[4]),"%s",argv[loop+1]);
                    check++;   count1++;
                }
                if(argv[loop][1]=='o'){
                    snprintf(argv_o[5],sizeof(argv_o[5]),"%s",argv[loop+1]);
                    check++;   count1++;
                }
                if(argv[loop][1]=='r'){
                    snprintf(argv_o[2],sizeof(argv_o[2]),"%s",argv[loop+1]);
                    check++;   count1++;
                }
                if(argv[loop][1]=='R'){
                    snprintf(argv_o[3],sizeof(argv_o[3]),"%s",argv[loop+1]);
                    check++;   count1++;
                }
                if(argv[loop][1]=='t'){
                    snprintf(argv_o[1],sizeof(argv_o[1]),"%s",argv[loop+1]);
                    check++;   count1++;
                }
                if(argv[loop][1]=='p'){
                    int count4 = 0;
                    do{
                        if( loop == argc-1 ){
                            printf(
                            "\n @@@@@@ Need \"@\" at the end of -p option\n\n");
                            return -1;
                        }
                        snprintf(argv_o[count4+n_arg],sizeof(argv_o[count4+n_arg])
                                 ,"%s",argv[loop+1]);
                        count4++;
                        loop++;
                    }while( strcmp(argv_o[count4+n_arg-1],"@") != 0 );
                    snprintf(argv_o[n_arg-1],sizeof(argv_o[n_arg-1])
                             ,"%d",count4-1);
                    check++;   count1++;
                }
                //***************************************************//
                if(argv[loop][1]=='f') check++;
                if(check==0){
                    printf("\n @@@@@@ invalid option -%c\n", argv[loop][1]);
                    usage( usage_type );
                    return -1;
                }
            }
        if(name_to_N_func(argv_o[1]) == -1){
            printf("\n @@@@@@ invalid function type \" %s \"\n\n",argv_o[1]);
            return -1;
        }
    }
//===========================================================================//
//========================= For observable calculation ======================//
//===========================================================================//
    if( usage_type == CALC_OBSERVABLE ){
        n_arg = 14;   // <- If you change the number of args, change also here.
        char option5[32][32]
        = {  "OBS_Lattice_spacing"  ,"OBS_Reduced_mass"    ,"OBS_Angular_momentum"
            ,"OBS_Calc_energy_min"  ,"OBS_Calc_energy_max" ,"OBS_Runge_kutta_delta"
            ,"OBS_Runge_kutta_max_r","OBS_Squ_wall_pot_V0" ,"OBS_Squ_wall_pot_r0"
            ,"OBS_Parameter_list"   ,"OBS_Output_file_name","OBS_Endian_convert"
            ,"OBS_Calc_squ_wall_pot","OBS_Calc_scatt_length" };
        
        for( int loop=1; loop<argc; loop++ )
            if( argv[loop][0] == '-' )
                if(argv[loop][1]=='f'){
                    snprintf(infile_name,sizeof(infile_name),"%s",argv[loop+1]);
                    count1++;   infile_flg = true;
                }
        if( infile_flg ){
            ifstream ifs(infile_name, ios::in );
            if(!ifs){
                printf("\n @@@@@@ the file \"%s\" is not exist\n\n", infile_name );
                return -1;
            }
            while(ifs.getline(tmp_str[count2],sizeof(tmp_str[count2]))) count2++;
            for( int loop1=0; loop1<count2; loop1++ ){
                if(sscanf(tmp_str[loop1]," %s = %s ",tmp_c1,tmp_c2) != 2 )
                    continue;
                for( int loop2=0; loop2<n_arg; loop2++ )
                    if( strcmp(tmp_c1,option5[loop2]) == 0 )
                        snprintf(argv_o[loop2],sizeof(argv_o[loop2]),"%s",tmp_c2);
            }
        }
        for( int loop=1; loop<argc; loop++ )
            if( argv[loop][0] == '-' ){
                check = 0;
                //****** You may set additional potion in here ******//
                if(argv[loop][1]=='i'){
                    snprintf(argv_o[9],sizeof(argv_o[9]),"%s",argv[loop+1]);
                    check++;   count1++;
                }
                if(argv[loop][1]=='o'){
                    snprintf(argv_o[10],sizeof(argv_o[10]),"%s",argv[loop+1]);
                    check++;   count1++;
                }
                if(argv[loop][1]=='m'){
                    snprintf(argv_o[1],sizeof(argv_o[1]),"%s",argv[loop+1]);
                    check++;   count1++;
                }
                if(argv[loop][1]=='t'){
                    snprintf(argv_o[12],sizeof(argv_o[12]),"yes");
                    check++;   count1++;
                }
                if(argv[loop][1]=='e'){
                    snprintf(argv_o[7],sizeof(argv_o[7]),"%s",argv[loop+1]);
                    check++;   count1++;
                }
                if(argv[loop][1]=='r'){
                    snprintf(argv_o[8],sizeof(argv_o[8]),"%s",argv[loop+1]);
                    check++;   count1++;
                }
                //***************************************************//
                if(argv[loop][1]=='f') check++;
                if(check==0){
                    printf("\n @@@@@@ invalid option -%c\n", argv[loop][1]);
                    usage( usage_type );
                    return -1;
                }
            }
    }
//===========================================================================//
//========================== For isospin projection =========================//
//===========================================================================//
    if( usage_type == ISOSPIN_PROJECTION ){
        n_arg = 12;   // <- If you change the number of args, change also here.
        char option6[32][32]
        = {  "ISO_Size_of_xyz"     ,"ISO_Min_time_slice","ISO_Max_time_slice"
            ,"ISO_Gauge_confs_list","ISO_Endian_convert","ISO_Path_to_directory"
            ,"ISO_T_shift"         ,"ISO_X_shift"       ,"ISO_Y_shift"
            ,"ISO_Z_shift"       ,"ISO_Snk_relativistic","ISO_Src_relativistic" };
        
        for( int loop=1; loop<argc; loop++ )
            if( argv[loop][0] == '-' )
                if(argv[loop][1]=='f'){
                    snprintf(infile_name,sizeof(infile_name),"%s",argv[loop+1]);
                    count1++;   infile_flg = true;
                }
        if( infile_flg ){
            ifstream ifs(infile_name, ios::in );
            if(!ifs){
                printf("\n @@@@@@ the file \"%s\" is not exist\n\n", infile_name );
                return -1;
            }
            while(ifs.getline(tmp_str[count2],sizeof(tmp_str[count2]))) count2++;
            for( int loop1=0; loop1<count2; loop1++ ){
                if(sscanf(tmp_str[loop1]," %s = %s ",tmp_c1,tmp_c2) != 2 )
                    continue;
                for( int loop2=0; loop2<n_arg; loop2++ )
                    if( strcmp(tmp_c1,option6[loop2]) == 0 )
                        snprintf(argv_o[loop2],sizeof(argv_o[loop2]),"%s",tmp_c2);
            }
            snprintf(list[MAIN_PATH],sizeof(list[MAIN_PATH]) ,"%s",argv_o[5]);
            snprintf(list[N_T_SHIFT],sizeof(list[N_T_SHIFT]) ,"%s",argv_o[6]);
            snprintf(list[N_X_SHIFT],sizeof(list[N_X_SHIFT]) ,"%s",argv_o[7]);
            snprintf(list[N_Y_SHIFT],sizeof(list[N_Y_SHIFT]) ,"%s",argv_o[8]);
            snprintf(list[N_Z_SHIFT],sizeof(list[N_Z_SHIFT]) ,"%s",argv_o[9]);
            snprintf(list[SNK_RELA],sizeof(list[SNK_RELA]) ,"%s",argv_o[10]);
            snprintf(list[SRC_RELA],sizeof(list[SRC_RELA]) ,"%s",argv_o[11]);
        }
        for( int loop=1; loop<argc; loop++ )
            if( argv[loop][0] == '-' ){
                check = 0;
                //****** You may set additional potion in here ******//
                
                //***************************************************//
                if(argv[loop][1]=='f') check++;
                if(check==0){
                    printf("\n @@@@@@ invalid option -%c\n", argv[loop][1]);
                    usage( usage_type );
                    return -1;
                }
            }
    }
//===========================================================================//
//===========================================================================//
//===========================================================================//
    if( count1 == 0 ){ printf("\n @@@@@@ invalid arguments\n\n");   return -1; }
    if( !infile_flg ){
        printf("\n @@@@@@ no input arguments file\n");
        usage( usage_type );
        return -1;
    }
    return count1;
}
