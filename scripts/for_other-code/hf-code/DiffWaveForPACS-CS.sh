#!/bin/sh

if [ $# -ne 9 ]
then
    echo "usage: $0 [Dir 1] [Dir 2] [Spin 0 or 1] [Time] [Tshift] [BBdir(Sx.xx)] [EnsN 1] [EnsN 2] [ConfN]"
    echo "     :     EnsN = 1,2,3, 11=1.gfix, 22=2.gfix, 33=3.gfix"
    exit 1
fi

######### ! CHANGE HERE ! #########
SpaceSize=32
###################################

Dir1=$1
Dir2=$2
DiffTime=$4
DiffTshift=$5
DiffDir=BBwave.dir.S$6
ConfN=$9

DiffTIME=`printf %03d $DiffTime`
DiffTSHIFT=`printf %03d $DiffTshift`
CONFN=`printf %06d $ConfN`

case $3 in
    0 ) Spin=S0_Sz0;;
    1 ) Spin=S1_Sz0;;
    * ) echo "Invalid Spin"; exit 1;;
esac

case $7 in
    1 ) CONF1=RC32x64_B1900Kud01370000Ks01364000C1715-b-$CONFN;;
    2 ) CONF1=RC32x64_B1900Kud01372700Ks01364000C1715-b-$CONFN;;
    3 ) CONF1=RC32x64_B1900Kud01375400Ks01364000C1715-b-$CONFN;;
    11 ) CONF1=RC32x64_B1900Kud01370000Ks01364000C1715-b-$CONFN.gfix;;
    22 ) CONF1=RC32x64_B1900Kud01372700Ks01364000C1715-b-$CONFN.gfix;;
    33 ) CONF1=RC32x64_B1900Kud01375400Ks01364000C1715-b-$CONFN.gfix;;
    * ) echo "Invalid Ens Number."; exit 1;;
esac

case $8 in
    1 ) CONF2=RC32x64_B1900Kud01370000Ks01364000C1715-b-$CONFN;;
    2 ) CONF2=RC32x64_B1900Kud01372700Ks01364000C1715-b-$CONFN;;
    3 ) CONF2=RC32x64_B1900Kud01375400Ks01364000C1715-b-$CONFN;;
    11 ) CONF2=RC32x64_B1900Kud01370000Ks01364000C1715-b-$CONFN.gfix;;
    22 ) CONF2=RC32x64_B1900Kud01372700Ks01364000C1715-b-$CONFN.gfix;;
    33 ) CONF2=RC32x64_B1900Kud01375400Ks01364000C1715-b-$CONFN.gfix;;
    * ) echo "Invalid Ens Number."; exit 1;;
esac

DiffWave=/bgdata4/scnfqcd/miyamoto/script/diffwave
DiffBase1=$DiffDir/$CONF1/NBSwave.+$DiffTIME+$DiffTSHIFT.000.000.000.$CONF1.NUC_CG05.NUC_CG05
DiffBase2=$DiffDir/$CONF2/NBSwave.+$DiffTIME+$DiffTSHIFT.000.000.000.$CONF2.NUC_CG05.NUC_CG05

$DiffWave $Dir1/$DiffBase1 $Dir2/$DiffBase2 $Spin $SpaceSize
