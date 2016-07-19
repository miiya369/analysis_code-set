#!/bin/sh

if [ $# -ne 2 ]
then
    echo "usage: $0 [ipath(absolute)] [opath(absolute)]"
    exit 1
fi

ibase=$1
obase=$2

if [ ! -e $ibase/BBwave.dir.S0.00 ]
then
    echo "ERROR: There is no file '$ibase/BBwave.dir.S0.00'."
    exit 1
fi

results=(`ls $ibase`)
confs=(`ls $ibase/BBwave.dir.S0.00`)

for Res in ${results[@]}
do
    if [ $Res == "Proj.DDwave.dir.S6.00" ]
    then
	iRes="Proj.DDwave.dir.S6.00/spin0z+0.0z+0"
    else
	iRes=$Res
    fi
    for Conf in ${confs[@]}
    do
	cd $ibase/$iRes/$Conf
	md5sum * > $obase/$Res/$Conf.md5sum
    done
done
