#!/bin/sh

if test $# -ne 3; then
echo " usage : sh $0 [working dir] [base name of confs] [#.bin]"
exit 1
fi

working_dir=$1
nbase=$2
N_bin=$3

for ibin in `seq 1 1 $N_bin`
do
CONF=${nbase}bin`printf %03d $N_bin`.`printf %03d $ibin`
for CHANNEL in 0 4
do
mkdir -p "$working_dir/BBwave.dir.S${CHANNEL}.00/$CONF"
done

for CHANNEL in `seq 0 1 8`
do
mkdir -p "$working_dir/BBwave.dir.S1.`printf %02d $CHANNEL`/$CONF"
done

for CHANNEL in `seq 0 1 35`
do
mkdir -p "$working_dir/BBwave.dir.S2.`printf %02d $CHANNEL`/$CONF"
done

for CHANNEL in `seq 0 1 8`
do
mkdir -p "$working_dir/BBwave.dir.S3.`printf %02d $CHANNEL`/$CONF"
done

for HADRON in PS SS multi
do
mkdir -p "$working_dir/correlator.${HADRON}.dir/$CONF"
done
done
