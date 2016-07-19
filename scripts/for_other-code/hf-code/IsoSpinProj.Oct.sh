#!/bin/sh

if [ $# -ne 3 ]
then
    echo "usage: $0 [conf list] [input directory] [output directory]"
    exit 1
fi

######### ! CHANGE HERE ! #########
t_min=5
t_max=15

snk_rela=CG05
src_rela=CG05
TXYZshift=A16.000.000.000

AddWave=$HOME/analysis_code-set/main/indep_code/WaveAdd_compress
################################### 

ConfList=$1

ibase=$2
obase=$3

sqrt13=`echo "sqrt(1/3)" | bc -l`
sqrt23=`echo "sqrt(2/3)" | bc -l`
sqrt49=`echo "2/3" | bc -l`
sqrt29=`echo "sqrt(2)/3" | bc -l`
sqrt19=`echo "1/3" | bc -l`

if [ ! -e $obase/BBwave.prj.S1.00 ] # Lambda N - Lambda N
then
    for CONF in `cat $ConfList`
    do
	mkdir -p $obase/BBwave.prj.S1.00/$CONF
    done
fi
if [ ! -e $obase/BBwave.prj.S1.01 ] # Lambda N - Sigma N
then
    for CONF in `cat $ConfList`
    do
	mkdir -p $obase/BBwave.prj.S1.01/$CONF
    done
fi
if [ ! -e $obase/BBwave.prj.S1.02 ] # Sigma N - Lambda N
then
    for CONF in `cat $ConfList`
    do
	mkdir -p $obase/BBwave.prj.S1.02/$CONF
    done
fi
if [ ! -e $obase/BBwave.prj.S1.03 ] # Sigma N - Sigma N
then
    for CONF in `cat $ConfList`
    do
	mkdir -p $obase/BBwave.prj.S1.03/$CONF
    done
fi
if [ ! -e $obase/BBwave.prj.S1.04 ] # Sigma N (IsoSpin = 3/2) - Sigma N (IsoSpin = 3/2)
then
    for CONF in `cat $ConfList`
    do
	mkdir -p $obase/BBwave.prj.S1.04/$CONF
    done
fi
if [ ! -e $obase/BBwave.prj.S1.05 ] # Lambda N - Sigma N (IsoSpin = 3/2)
then
    for CONF in `cat $ConfList`
    do
        mkdir -p $obase/BBwave.prj.S1.05/$CONF
    done
fi
if [ ! -e $obase/BBwave.prj.S1.06 ] # Sigma N (IsoSpin = 3/2) - Lambda N
then
    for CONF in `cat $ConfList`
    do
        mkdir -p $obase/BBwave.prj.S1.06/$CONF
    done
fi

echo "Setup:"
echo " - Current    = `pwd`"
echo " - Conf List  = $ConfList"
echo " - Input dir  = $ibase"
echo " - Output dir = $obase"
echo " - time       = $t_min - $t_max"
echo
echo "IsoSpin Projection (Oct) START: `date`"
echo

for CONF in `cat $ConfList`
do
    for Time in `seq $t_min 1 $t_max`
    do
        TIME=`printf %03d $Time`
	for SRC_RELA in $src_rela
        do
            for SNK_RELA in $snk_rela
            do
		ifile=$ibase/BBwave.dir.S1.00/$CONF/NBSwave.+$TIME+$TXYZshift.$CONF.NUC_$SNK_RELA.NUC_$SRC_RELA

		ofile=$obase/BBwave.prj.S1.00/$CONF/NBSwave.+$TIME+$TXYZshift.$CONF.NUC_$SNK_RELA.NUC_$SRC_RELA
		$AddWave $ofile 1 $ifile
		
		ifile1=$ibase/BBwave.dir.S1.02/$CONF/NBSwave.+$TIME+$TXYZshift.$CONF.NUC_$SNK_RELA.NUC_$SRC_RELA
		ifile2=$ibase/BBwave.dir.S1.01/$CONF/NBSwave.+$TIME+$TXYZshift.$CONF.NUC_$SNK_RELA.NUC_$SRC_RELA

                ofile=$obase/BBwave.prj.S1.01/$CONF/NBSwave.+$TIME+$TXYZshift.$CONF.NUC_$SNK_RELA.NUC_$SRC_RELA
                $AddWave $ofile $sqrt23 $ifile1 -$sqrt13 $ifile2
		ofile=$obase/BBwave.prj.S1.05/$CONF/NBSwave.+$TIME+$TXYZshift.$CONF.NUC_$SNK_RELA.NUC_$SRC_RELA
                $AddWave $ofile $sqrt13 $ifile1 $sqrt23 $ifile2
		
		ifile1=$ibase/BBwave.dir.S1.06/$CONF/NBSwave.+$TIME+$TXYZshift.$CONF.NUC_$SNK_RELA.NUC_$SRC_RELA
                ifile2=$ibase/BBwave.dir.S1.03/$CONF/NBSwave.+$TIME+$TXYZshift.$CONF.NUC_$SNK_RELA.NUC_$SRC_RELA
                
		ofile=$obase/BBwave.prj.S1.02/$CONF/NBSwave.+$TIME+$TXYZshift.$CONF.NUC_$SNK_RELA.NUC_$SRC_RELA
                $AddWave $ofile $sqrt23 $ifile1 -$sqrt13 $ifile2
		ofile=$obase/BBwave.prj.S1.06/$CONF/NBSwave.+$TIME+$TXYZshift.$CONF.NUC_$SNK_RELA.NUC_$SRC_RELA
                $AddWave $ofile $sqrt13 $ifile1 $sqrt23 $ifile2
		
		ifile1=$ibase/BBwave.dir.S1.08/$CONF/NBSwave.+$TIME+$TXYZshift.$CONF.NUC_$SNK_RELA.NUC_$SRC_RELA
                ifile2=$ibase/BBwave.dir.S1.05/$CONF/NBSwave.+$TIME+$TXYZshift.$CONF.NUC_$SNK_RELA.NUC_$SRC_RELA
		ifile3=$ibase/BBwave.dir.S1.07/$CONF/NBSwave.+$TIME+$TXYZshift.$CONF.NUC_$SNK_RELA.NUC_$SRC_RELA
                ifile4=$ibase/BBwave.dir.S1.04/$CONF/NBSwave.+$TIME+$TXYZshift.$CONF.NUC_$SNK_RELA.NUC_$SRC_RELA
                
		ofile=$obase/BBwave.prj.S1.03/$CONF/NBSwave.+$TIME+$TXYZshift.$CONF.NUC_$SNK_RELA.NUC_$SRC_RELA
                $AddWave $ofile $sqrt49 $ifile1 -$sqrt29 $ifile2 -$sqrt29 $ifile3 $sqrt19 $ifile4
                ofile=$obase/BBwave.prj.S1.04/$CONF/NBSwave.+$TIME+$TXYZshift.$CONF.NUC_$SNK_RELA.NUC_$SRC_RELA
                $AddWave $ofile $sqrt19 $ifile1 $sqrt29 $ifile2 $sqrt29 $ifile3 $sqrt49 $ifile4
	    done
	done
    done
    echo "End: $CONF: `date`"
done

echo
echo "IsoSpin Projection (Oct)  END : `date`"
