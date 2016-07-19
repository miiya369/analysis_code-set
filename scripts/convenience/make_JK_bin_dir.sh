#!/bin/sh

if [ $# -lt 5 ]
then
   echo "usage: sh $0 [working dir] [base name of confs] [#.org conf] [bin size] [dir name 1] [dir name 2] ..."
   exit 1
fi

working_dir=$1
directories=($@)
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
    
    for i in `seq 5 1 $#`
    do
        mkdir -p "$working_dir/${directories[i-1]}/$CONF"
    done
done
