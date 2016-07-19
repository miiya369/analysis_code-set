#!/bin/sh

if [ $# -ne 4 ]
then
    echo "usage: $0 [conf list] [Hadron list] [input directory] [output directory]"
    exit 1
fi

######### ! CHANGE HERE ! #########
XYZshift=000.000.000

Time="`seq 0 4 60`"
################################### 

ConfList=$1
HadList=$2

idir=$3
odir=$4

echo "Setup:"
echo " - Conf List  = $ConfList"
echo " - Corr List  = $HadList"
echo " - Input dir  = $idir"
echo " - Output dir = $odir"
echo
echo "Copy correlator.multi START: `date`"
echo

for CONF in `cat $ConfList`
do
    for HAD in `cat $HadList`
    do
	for TIME in $Time
	do
	    ifile=$idir/${HAD}_correlator.+`printf %03d $TIME`.$CONF
	    ofile=$odir/$CONF/${HAD}_correlator.+`printf %03d $TIME`.$XYZshift.$CONF
	    
	    cp $ifile $ofile
	done
    done
    echo "End: $CONF: `date`"
done

echo
echo "Copy correlator.multi  END : `date`"