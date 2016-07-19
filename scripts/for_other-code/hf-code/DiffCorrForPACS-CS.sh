#!/bin/sh

if [ $# -ne 8 ]
then
    echo "usage: $0 [Dir 1] [Dir 2] [Tshift] [Corr Dir] [EnsN 1] [EnsN 2] [ConfN] [Hadron]"
    echo "     :     Corr Dir = PS, SS, multi"
    echo "     :     EnsN     = 1,2,3, 11=1.gfix, 22=2.gfix, 33=3.gfix"
    exit 1
fi

######### ! CHANGE HERE ! #########

###################################

Dir1=$1
Dir2=$2
DiffTshift=$3
ConfN=$7
Had=$8

DiffTSHIFT=`printf %03d $DiffTshift`
CONFN=`printf %06d $ConfN`

case $4 in
    PS ) DiffDir=correlator.$4.dir;;
    SS ) DiffDir=correlator.$4.dir;;
    multi ) DiffDir=correlator.$4.dir;;
    * ) echo "Invalid Correlator directory name."; exit 1;;
esac

case $5 in
    1 ) CONF1=RC32x64_B1900Kud01370000Ks01364000C1715-b-$CONFN;;
    2 ) CONF1=RC32x64_B1900Kud01372700Ks01364000C1715-b-$CONFN;;
    3 ) CONF1=RC32x64_B1900Kud01375400Ks01364000C1715-b-$CONFN;;
    11 ) CONF1=RC32x64_B1900Kud01370000Ks01364000C1715-b-$CONFN.gfix;;
    22 ) CONF1=RC32x64_B1900Kud01372700Ks01364000C1715-b-$CONFN.gfix;;
    33 ) CONF1=RC32x64_B1900Kud01375400Ks01364000C1715-b-$CONFN.gfix;;
    * ) echo "Invalid Ens Number."; exit 1;;
esac

case $6 in
    1 ) CONF2=RC32x64_B1900Kud01370000Ks01364000C1715-b-$CONFN;;
    2 ) CONF2=RC32x64_B1900Kud01372700Ks01364000C1715-b-$CONFN;;
    3 ) CONF2=RC32x64_B1900Kud01375400Ks01364000C1715-b-$CONFN;;
    11 ) CONF2=RC32x64_B1900Kud01370000Ks01364000C1715-b-$CONFN.gfix;;
    22 ) CONF2=RC32x64_B1900Kud01372700Ks01364000C1715-b-$CONFN.gfix;;
    33 ) CONF2=RC32x64_B1900Kud01375400Ks01364000C1715-b-$CONFN.gfix;;
    * ) echo "Invalid Ens Number."; exit 1;;
esac

DiffCorr=/bgdata4/scnfqcd/miyamoto/script/DiffCorrelators.py
DiffBase1=$DiffDir/$CONF1/${Had}_correlator.+$DiffTSHIFT.000.000.000.$CONF1
DiffBase2=$DiffDir/$CONF2/${Had}_correlator.+$DiffTSHIFT.000.000.000.$CONF2

$DiffCorr $Dir1/$DiffBase1 $Dir2/$DiffBase2
