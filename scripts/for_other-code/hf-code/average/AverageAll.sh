#!/bin/sh

if [ $# -ne 4 ]
then
    echo " usage : sh $0 [conf list] [#.average] [Input directory] [Output directory]"
    echo "       : [Input directory] should be same location with the Tshift0??/ directories"
    exit 1
fi

######### ! CHANGE HERE ! #########
ScriptsBase=$HOME/analysis_code-set/scripts/for_other-code/hf-code/average

MakeHFdir=$ScriptsBase/../MakeResultsDir-HF.sh

AveCorr=$ScriptsBase/AverageCorr.sh
AveWave=$ScriptsBase/AverageWave.sh

CorrPS_SS_List=$ScriptsBase/Corr.PS.SS.list
CorrMulti_List=$ScriptsBase/Corr.multi.list
BBwave_List=$ScriptsBase/BBwave.list
PPwave_List=$ScriptsBase/PPwave.list
################################### 

conf_list=$1
Naverage=$2
ibase=$3
obase=$4

Nave=`printf %02d $Naverage`
OBASE=$obase/org

CorrDir="correlator.PS.dir correlator.SS.dir correlator.multi.dir"
BBwaveDir=`cat $BBwave_List`
PPwaveDir=`cat $PPwave_List`

mkdir -p $obase/log

$MakeHFdir $OBASE $conf_list $BBwaveDir $PPwaveDir $CorrDir

$AveCorr $conf_list $Naverage $ibase $OBASE $CorrPS_SS_List 1 > $obase/log/Ave_Corr.PS.SS.log 2>&1 &
$AveCorr $conf_list $Naverage $ibase $OBASE $CorrMulti_List 2 > $obase/log/Ave_Corr.multi.log 2>&1 &

$AveWave $conf_list $Naverage $ibase $OBASE $BBwave_List 1 > $obase/log/Ave_BBwave.log 2>&1 &
$AveWave $conf_list $Naverage $ibase $OBASE $PPwave_List 2 > $obase/log/Ave_PPwave.log 2>&1 &
