#!/bin/sh

if [ $# -ne 6 ]
then
    echo " usage : sh $0 [conf list] [#.average] [Input directory] [Output directory] [NBSwave list] [1=BBwave, 2=PPwave]"
    echo "       : [Input directory] should be same location with the Tshift0??/ directories"
    exit 1
fi

######### ! CHANGE HERE ! #########
t_min_BB=6
t_max_BB=18
t_min_PP=6
t_max_PP=31

snk_rela=CG05
XYZshift=000.000.000

WaveAve_NoComp=$HOME/analysis_code-set/main/indep_code/WaveAve_not-compress
WaveAve_Comp=$HOME/analysis_code-set/bin/waveave
################################### 

conf_list=$1
Naverage=$2
ibase=$3
obase=$4
wave_list=$5

Nave=`printf %02d $Naverage`

if [ $6 -eq 1 ]
then
    t_min=$t_min_BB
    t_max=$t_max_BB
    src_rela="CG05 NR"
    NBStype=NUC
    WaveAve=$WaveAve_Comp
elif [ $6 -eq 2 ]
then
    t_min=$t_min_PP
    t_max=$t_max_PP
    src_rela=CG05
    NBStype=PP
    WaveAve=$WaveAve_NoComp
else
    echo "Invalid arguments: Please put 1 or 2 (1=BBwave, 2=PPwave)"
    exit 1
fi

echo "Setup:"
echo " - Current    = `pwd`"
echo " - Conf List  = $conf_list"
echo " - Wave List  = $wave_list"
echo " - Input dir  = $ibase"
echo " - Output dir = $obase"
echo " - time       = $t_min - $t_max"
echo " - #.average  = $Naverage"
echo
echo "Average NBSwave START: `date`"
echo

for CONF in `cat $conf_list`
do
    for Time in `seq $t_min 1 $t_max`
    do
	TIME=`printf %03d $Time`
	for WaveDir in `cat $wave_list`
	do
	    for SRC_RELA in $src_rela
	    do
		for SNK_RELA in $snk_rela
		do
		    ofname=$obase/$WaveDir/$CONF/NBSwave.+$TIME+A$Nave.$XYZshift.$CONF.${NBStype}_$SNK_RELA.${NBStype}_$SRC_RELA
		    $WaveAve $ofname $ibase/Tshift???/$WaveDir/$CONF/NBSwave.+$TIME+???.$XYZshift.$CONF.${NBStype}_$SNK_RELA.${NBStype}_$SRC_RELA
		    if [ $? -ne 0 ]
		    then
			echo "ERROR: Something happened. Exit."
			echo
			echo "Report: conf = $CONF"
			echo "      : time = $Time"
			echo "      : dir  = $WaveDir"
			echo "      : rela = ${SNK_RELA}_${SRC_RELA}"
			exit 1
		    fi
		done
	    done
	done
    done
    echo "End: $CONF: `date`"
done

echo
echo "Average NBSwave  END : `date`"
