#!/bin/sh

if test $# -ne 4
then
   echo "usage: sh $0 [working dir] [base name of confs] [#.conf] [bin size]"
   exit 1
fi

working_dir=$1
nbase=$2
Nconf=$3
bin_size=$4

if test $((Nconf%bin_size)) -ne 0
then
   echo "ERROR: #.conf cannot be divided by bin size."
   exit 1
fi

N_bin=$((Nconf/bin_size))

for ibin in `seq 1 1 $N_bin`
do
   CONF=${nbase}bin`printf %03d $bin_size`.`printf %03d $ibin`

   for CHANNEL in 0 4
   do
      mkdir -p "$working_dir/bin`printf %03d $bin_size`/BBwave.dir.S${CHANNEL}.00/$CONF"
   done

   for CHANNEL in `seq 0 1 8`
   do
      mkdir -p "$working_dir/bin`printf %03d $bin_size`/BBwave.dir.S1.`printf %02d $CHANNEL`/$CONF"
   done

   for CHANNEL in `seq 0 1 35`
   do
      mkdir -p "$working_dir/bin`printf %03d $bin_size`/BBwave.dir.S2.`printf %02d $CHANNEL`/$CONF"
   done

   for CHANNEL in `seq 0 1 8`
   do
      mkdir -p "$working_dir/bin`printf %03d $bin_size`/BBwave.dir.S3.`printf %02d $CHANNEL`/$CONF"
   done

   for HADRON in PS SS multi
   do
      mkdir -p "$working_dir/bin`printf %03d $bin_size`/correlator.${HADRON}.dir/$CONF"
   done
done
