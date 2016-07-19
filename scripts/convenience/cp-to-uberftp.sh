#!/bin/sh

if [ $# -ne 3 ]
then
    echo "usage: $0 [File] [Copy To (JLDG:home/...)] [Log name base]"
    exit 1
fi

# For KEK
CLIENT=scjldg01.sc.kek.jp

# For RIKEN
#CLIENT=jldgriken01

Fname=$1
CopyTo=$2
LogBase=$3

if [ ! -e $Fname ]
then
    echo "No such a file '$Fname'"
    exit 1
fi

echo "@@@ COPY TO JLDG START: `date`" > $LogBase.CopyTo-uberftp.log 2>&1

uberftp file:$Fname gsiftp://$CLIENT/gfarm/pacscs/BBILQCD/miyamoto/$CopyTo \
>> $LogBase.CopyTo-uberftp.log 2>&1

ExitCode=$?
if [ $ExitCode -eq 0 ]
then
    echo "@@@ COPY TO JLDG Successful" >> $LogBase.CopyTo-uberftp.log 2>&1
else
    echo "@@@ COPY TO JLDG ERROR: $ExitCode" >> $LogBase.CopyTo-uberftp.log 2>&1
fi
echo "@@@ COPY TO JLDG  END : `date`" >> $LogBase.CopyTo-uberftp.log 2>&1
