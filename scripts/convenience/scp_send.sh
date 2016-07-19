#!/bin/sh

if [ $# -ne 4 ]
then
    echo "usage: $0 [file] [DST Server] [Path on DST] [Log file base name]"
    exit 1
fi

Fname=$1
DSTserver=$2
DSTpath=$3
Logfile=$4

if [ ! -e $Fname ]
then
    echo "No such a file '$Fname'"
    exit 1
fi

echo "@@@ SCP START: `date`" > $Logfile.scp_send.log 2>&1
scp $Fname $DSTserver:$DSTpath/$Fname >> $Logfile.scp_send.log 2>&1
ExitCode=$?
if [ $ExitCode -eq 0 ]
then
    echo "@@@ SCP Successful" >> $Logfile.scp_send.log 2>&1
else
    echo "@@@ SCP ERROR: $ExitCode" >> $Logfile.scp_send.log 2>&1
fi
echo "@@@ SCP  END : `date`" >> $Logfile.scp_send.log 2>&1
