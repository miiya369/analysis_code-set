#!/bin/sh

if test $# -ne 3; then
echo " usage : sh $0 [working dir] [conf list dir] [#. average Tshift]"
exit 1
fi

working_dir=$1
conf_list=$2
Nave=$3

confs=`ls -U $conf_list`

for CONF in $confs
do
   for CHANNEL in 0 4
   do
      mkdir -p "$working_dir/t_shift_ave/${Nave}src/BBwave.dir.S${CHANNEL}.00/$CONF"
   done

   for CHANNEL in `seq 0 1 8`
   do
      mkdir -p "$working_dir/t_shift_ave/${Nave}src/BBwave.dir.S1.`printf %02d $CHANNEL`/$CONF"
   done

   for CHANNEL in `seq 0 1 35`
   do
      mkdir -p "$working_dir/t_shift_ave/${Nave}src/BBwave.dir.S2.`printf %02d $CHANNEL`/$CONF"
   done

   for CHANNEL in `seq 0 1 8`
   do
      mkdir -p "$working_dir/t_shift_ave/${Nave}src/BBwave.dir.S3.`printf %02d $CHANNEL`/$CONF"
   done

   for HADRON in PS SS multi
   do
      mkdir -p "$working_dir/t_shift_ave/${Nave}src/correlator.${HADRON}.dir/$CONF"
   done
done
