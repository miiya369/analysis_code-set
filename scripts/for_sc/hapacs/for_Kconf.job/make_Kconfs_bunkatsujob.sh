#!/bin/sh

if test $# -ne 6
then
    echo " usage : sh $0 [job name] [out file name] [idir] [odir] \"confs\" [set]"
    exit 0
fi

JOB_NAME=$1
OUT_FILE_NAME=$2
SET_INPUT_PATH=$3
SET_OUTPUT_PATH=$4
SET_CONFS=$5
SET_SET=$6

echo '#PBS -S /bin/bash'
echo "#PBS -N $JOB_NAME"
echo '#PBS -A HALQCD'
echo '#PBS -q comq'
echo '#PBS -l select=1:ncpus=16:mpiprocs=1:ompthreads=16'
echo '#PBS -l walltime=24:00:00'
echo '#PBS -j oe'
echo "#PBS -o $OUT_FILE_NAME"
echo ''
echo '. /opt/Modules/default/init/bash'
echo ''
echo 'module load gnu/4.4.7 cuda/6.5.14'
echo 'module load mvapich2/1.8.1_gnu_cuda-6.0.37'
echo 'module load boost/1.55_gnu-4.4.7'
echo ''
echo 'cd $PBS_O_WORKDIR'
echo ''
echo 'echo $PBS_NODEFILE'
echo 'cat $PBS_NODEFILE'
echo ''
echo '### This job script is made by T.Miyamoto (2015.06.26)'
echo '###    read DDHMC -> stout smear -> rotate -> write bunkatsu MDP'
echo '### Using, konv96'
echo ''
echo '#########  Parameter Setting #########'
echo '#                                    #'
echo "INPUT_PATH=$SET_INPUT_PATH"
echo "OUTPUT_PATH=$SET_OUTPUT_PATH"
echo ''
echo "CALC_CONF=\"$SET_CONFS\""
echo "SET=$SET_SET"
echo ''
echo 'XYZSIZE=96'
echo 'TSIZE=96'
echo ''
echo '## You need change format "config" ! #'
echo '#                                    #'
echo '######################################'
echo 'DIV_TSIZE=$((TSIZE/8+1))'
echo ''
echo 'for CONF in $CALC_CONF'
echo 'do'
echo ''
echo 'TIME=`date`'
echo 'echo " @@@ DDHMC->MDP START   : conf=$CONF : $TIME"'
echo ''
echo '#########   DDHMC -> stout smear -> rotate -> bunkatsu MDP   #########'
echo '#                                                                    #'
echo 'mkdir -p $OUTPUT_PATH/tmp.mdp'
echo './konv96 \'
echo '-sites $XYZSIZE $XYZSIZE $XYZSIZE $TSIZE \'
echo '-ddhmc::NDIM 8 8 8 4 \'
echo '-ddhmc::read $INPUT_PATH/$CONF/config \'
echo '-plaq \'
echo '-smearing::stout 0.1 6 \'
echo '-mdp::bunkatsu_write 8 $OUTPUT_PATH/tmp.mdp/t$CONF \'
echo '-rotate 1 \'
echo '-mdp::bunkatsu_write 8 $OUTPUT_PATH/tmp.mdp/x$CONF \'
echo '-rotate 1 \'
echo '-mdp::bunkatsu_write 8 $OUTPUT_PATH/tmp.mdp/y$CONF \'
echo '-rotate 1 \'
echo '-mdp::bunkatsu_write 8 $OUTPUT_PATH/tmp.mdp/z$CONF'
echo 'case "$?" in'
echo '     0) echo "good";;'
echo '     *) echo "@@@@@@ FATAL ERROR !"; exit 1;;'
echo 'esac'
echo '#                                                                    #'
echo '######################################################################'
echo ''
echo 'TIME=`date`'
echo 'echo " @@@ DDHMC->MDP END     : conf=$CONF : $TIME"'
echo ''
echo 'echo " @@@SIZE@@@ t${CONF}_t00 = `du $OUTPUT_PATH/tmp.mdp/t${CONF}_t00.mdp`"'
echo 'echo " @@@SIZE@@@ t${CONF}_t01 = `du $OUTPUT_PATH/tmp.mdp/t${CONF}_t01.mdp`"'
echo 'echo " @@@SIZE@@@ t${CONF}_t02 = `du $OUTPUT_PATH/tmp.mdp/t${CONF}_t02.mdp`"'
echo 'echo " @@@SIZE@@@ t${CONF}_t03 = `du $OUTPUT_PATH/tmp.mdp/t${CONF}_t03.mdp`"'
echo 'echo " @@@SIZE@@@ t${CONF}_t04 = `du $OUTPUT_PATH/tmp.mdp/t${CONF}_t04.mdp`"'
echo 'echo " @@@SIZE@@@ t${CONF}_t05 = `du $OUTPUT_PATH/tmp.mdp/t${CONF}_t05.mdp`"'
echo 'echo " @@@SIZE@@@ t${CONF}_t06 = `du $OUTPUT_PATH/tmp.mdp/t${CONF}_t06.mdp`"'
echo 'echo " @@@SIZE@@@ t${CONF}_t07 = `du $OUTPUT_PATH/tmp.mdp/t${CONF}_t07.mdp`"'
echo 'echo " @@@SIZE@@@ x${CONF}_t00 = `du $OUTPUT_PATH/tmp.mdp/x${CONF}_t00.mdp`"'
echo 'echo " @@@SIZE@@@ x${CONF}_t01 = `du $OUTPUT_PATH/tmp.mdp/x${CONF}_t01.mdp`"'
echo 'echo " @@@SIZE@@@ x${CONF}_t02 = `du $OUTPUT_PATH/tmp.mdp/x${CONF}_t02.mdp`"'
echo 'echo " @@@SIZE@@@ x${CONF}_t03 = `du $OUTPUT_PATH/tmp.mdp/x${CONF}_t03.mdp`"'
echo 'echo " @@@SIZE@@@ x${CONF}_t04 = `du $OUTPUT_PATH/tmp.mdp/x${CONF}_t04.mdp`"'
echo 'echo " @@@SIZE@@@ x${CONF}_t05 = `du $OUTPUT_PATH/tmp.mdp/x${CONF}_t05.mdp`"'
echo 'echo " @@@SIZE@@@ x${CONF}_t06 = `du $OUTPUT_PATH/tmp.mdp/x${CONF}_t06.mdp`"'
echo 'echo " @@@SIZE@@@ x${CONF}_t07 = `du $OUTPUT_PATH/tmp.mdp/x${CONF}_t07.mdp`"'
echo 'echo " @@@SIZE@@@ y${CONF}_t00 = `du $OUTPUT_PATH/tmp.mdp/y${CONF}_t00.mdp`"'
echo 'echo " @@@SIZE@@@ y${CONF}_t01 = `du $OUTPUT_PATH/tmp.mdp/y${CONF}_t01.mdp`"'
echo 'echo " @@@SIZE@@@ y${CONF}_t02 = `du $OUTPUT_PATH/tmp.mdp/y${CONF}_t02.mdp`"'
echo 'echo " @@@SIZE@@@ y${CONF}_t03 = `du $OUTPUT_PATH/tmp.mdp/y${CONF}_t03.mdp`"'
echo 'echo " @@@SIZE@@@ y${CONF}_t04 = `du $OUTPUT_PATH/tmp.mdp/y${CONF}_t04.mdp`"'
echo 'echo " @@@SIZE@@@ y${CONF}_t05 = `du $OUTPUT_PATH/tmp.mdp/y${CONF}_t05.mdp`"'
echo 'echo " @@@SIZE@@@ y${CONF}_t06 = `du $OUTPUT_PATH/tmp.mdp/y${CONF}_t06.mdp`"'
echo 'echo " @@@SIZE@@@ y${CONF}_t07 = `du $OUTPUT_PATH/tmp.mdp/y${CONF}_t07.mdp`"'
echo 'echo " @@@SIZE@@@ z${CONF}_t00 = `du $OUTPUT_PATH/tmp.mdp/z${CONF}_t00.mdp`"'
echo 'echo " @@@SIZE@@@ z${CONF}_t01 = `du $OUTPUT_PATH/tmp.mdp/z${CONF}_t01.mdp`"'
echo 'echo " @@@SIZE@@@ z${CONF}_t02 = `du $OUTPUT_PATH/tmp.mdp/z${CONF}_t02.mdp`"'
echo 'echo " @@@SIZE@@@ z${CONF}_t03 = `du $OUTPUT_PATH/tmp.mdp/z${CONF}_t03.mdp`"'
echo 'echo " @@@SIZE@@@ z${CONF}_t04 = `du $OUTPUT_PATH/tmp.mdp/z${CONF}_t04.mdp`"'
echo 'echo " @@@SIZE@@@ z${CONF}_t05 = `du $OUTPUT_PATH/tmp.mdp/z${CONF}_t05.mdp`"'
echo 'echo " @@@SIZE@@@ z${CONF}_t06 = `du $OUTPUT_PATH/tmp.mdp/z${CONF}_t06.mdp`"'
echo 'echo " @@@SIZE@@@ z${CONF}_t07 = `du $OUTPUT_PATH/tmp.mdp/z${CONF}_t07.mdp`"'
echo ''
echo 'done'