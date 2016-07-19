#!/bin/sh

if [ $# -ne 6 ]
then
    echo " usage : sh $0 [conf list] [#.average] [Input directory] [Output directory] [Corr list] [1=PS/SS, 2=multi]"
    echo "       : [Input directory] should be same location with the Tshift0??/ directories"
    exit 1
fi

######### ! CHANGE HERE ! #########
XYZshift=000.000.000

CorrAve=$HOME/analysis_code-set/scripts/for_other-code/hf-code/average/corrave.py
################################### 

conf_list=$1
Naverage=$2
ibase=$3
obase=$4
corr_list=$5

Nave=`printf %02d $Naverage`

if [ $6 -eq 1 ]
then
    CorrDir="PS SS"
elif [ $6 -eq 2 ]
then
    CorrDir=multi
else
    echo "Invalid arguments: Please put 1 or 2 (1=PS/SS, 2=multi)"
    exit 1
fi

echo "Setup:"
echo " - Current    = `pwd`"
echo " - Conf List  = $conf_list"
echo " - Corr List  = $corr_list"
echo " - Input dir  = $ibase"
echo " - Output dir = $obase"
echo " - #.average  = $Naverage"
echo
echo "Average Correlator START: `date`"
echo

for CONF in `cat $conf_list`
do
    for Dir in $CorrDir
    do
	for CorrName in `cat $corr_list`
	do
	    ofname=$obase/correlator.$Dir.dir/$CONF/${CorrName}_correlator.+A$Nave.$XYZshift.$CONF
	    $CorrAve $ofname $ibase/Tshift???/correlator.$Dir.dir/$CONF/${CorrName}_correlator.+???.$XYZshift.$CONF
	    if [ $? -ne 0 ]
	    then
		echo "ERROR: Something happened. Exit."
		echo ""
		echo "Report: conf = $CONF"
		echo "      : dir  = $Dir"
		echo "      : Corr = $CorrName"
		exit 1
	    fi
	done
    done
    echo "End: $CONF: `date`"
done

echo
echo "Average Correlator  END : `date`"
