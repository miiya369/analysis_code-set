#!/bin/sh

if [ $# -ne 1 ]
then
    echo "usage: $0 [file.tar]"
    exit 1
fi

Fname=$1

if [ ! -e $Fname ]
then
    echo "No such a file '$Fname'"
    exit 1
fi

echo "@@@ TAR START: `date`" > $Fname.expand.log 2>&1

tar xvf $Fname >> $Fname.expand.log 2>&1

ExitCode=$?
if [ $ExitCode -eq 0 ]
then
    echo "@@@ TAR Successful" >> $Fname.expand.log 2>&1
else
    echo "@@@ TAR ERROR: $ExitCode" >> $Fname.expand.log 2>&1
fi
echo "@@@ TAR  END : `date`" >> $Fname.expand.log 2>&1
