#!/bin/sh

if test $# -ne 1
then
   echo " usage : sh $0 [directory]"
   exit -1
fi

PATHofCONF="$1/*"
for CONF in ${PATHofCONF}
do
    CONF_tmp=${CONF##*/}
    echo ${CONF_tmp%/*}
done