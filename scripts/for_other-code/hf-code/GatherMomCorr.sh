#!/bin/sh

if [ $# -ne 5 ]
then
    echo "usage: $0 [conf list] [Hadron list] [input directory] [output directory] [mod4 = (0,1,2,3)]"
    exit 1
fi

######### ! CHANGE HERE ! #########
XYZshift=000.000.000
################################### 

ConfList=$1
HadList=$2

idir=$3
odir=$4

mod4=$5

if [ $mod4 -gt 3 ]
then
    echo "Invalid arguments: Please put [0-3]"
    exit 1
fi

Time="`seq $mod4 4 $((60+$mod4))`"

echo "Setup:"
echo " - Conf List  = $ConfList"
echo " - Corr List  = $HadList"
echo " - Input dir  = $idir"
echo " - Output dir = $odir"
echo
echo "Gather momentum for Correlator START: `date`"
echo

for CONF in `cat $ConfList`
do
    for HAD in `cat $HadList`
    do
	for TIME in $Time
	do
	    paste $idir/${HAD}_correlator.amom00.+`printf %03d $TIME`.$CONF \
		$idir/${HAD}_correlator.amom01.+`printf %03d $TIME`.$CONF \
		$idir/${HAD}_correlator.amom02.+`printf %03d $TIME`.$CONF \
		$idir/${HAD}_correlator.amom03.+`printf %03d $TIME`.$CONF \
		$idir/${HAD}_correlator.amom04.+`printf %03d $TIME`.$CONF | \
		awk '{print $1, $2, $3, $5, $6, $8, $9, $11, $12, $14, $15}' > \
		$odir/$CONF/${HAD}_correlator.+`printf %03d $TIME`.$XYZshift.$CONF
	done
    done
    echo "End: $CONF: `date`"
done

echo
echo "Gather momentum for Correlator  END : `date`"