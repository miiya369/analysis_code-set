#!/bin/sh

if test $# != 2
then
   echo " usage : sh $0 [mass1] [mass2]"
   exit 1
fi

mass1=$1
mass2=$2

echo "scale=6; ($mass1 * $mass2) / ($mass1 + $mass2)" | bc
