#!/bin/sh

if [ $# -ne 3 ]
then
    echo "usage: $0 [File in JLDG (from gfarm/...)] [Copy To (Local)] [Log name base]"
    exit 1
fi

# For KEK
CLIENT=scjldg01.sc.kek.jp

# For RIKEN
#CLIENT=jldgriken01

Fname=$1
CopyTo=$2
LogBase=$3

echo "@@@ COPY FROM JLDG START: `date`" > $LogBase.CopyFrom-uberftp.log 2>&1

uberftp gsiftp://$CLIENT/$Fname file:$CopyTo >> $LogBase.CopyFrom-uberftp.log 2>&1

ExitCode=$?
if [ $ExitCode -eq 0 ]
then
    echo "@@@ COPY FROM JLDG Successful" >> $LogBase.CopyFrom-uberftp.log 2>&1
else
    echo "@@@ COPY FROM JLDG ERROR: $ExitCode" >> $LogBase.CopyFrom-uberftp.log 2>&1
fi
echo "@@@ COPY FROM JLDG  END : `date`" >> $LogBase.CopyFrom-uberftp.log 2>&1
