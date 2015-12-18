#!/bin/sh

PATHofCONF="$1/*"
for CONF in ${PATHofCONF}
do
    CONF_tmp=${CONF##*/}
    echo ${CONF_tmp%/*}
done