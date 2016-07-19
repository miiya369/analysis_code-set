#!/bin/sh

if [ $# -ne 1 ]
then
    echo "usage: $0 [Directory]"
    exit 1
fi

DirName=$1

if [ ! -e $DirName ]
then
    echo "No such a directoty '$DirName'"
    exit 1
fi

echo "@@@ TAR START: `date`" > $DirName.tar_c.log 2>&1

tar cvf $DirName.tar $DirName >> $DirName.tar_c.log 2>&1

ExitCode=$?
if [ $ExitCode -eq 0 ]
then
    echo "@@@ TAR Successful" >> $DirName.tar_c.log 2>&1
else
    echo "@@@ TAR ERROR: $ExitCode" >> $DirName.tar_c.log 2>&1
fi
echo "@@@ TAR  END : `date`" >> $DirName.tar_c.log 2>&1
