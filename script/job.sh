#!/bin/sh

TIME=`date`
echo " @@@ JOB 1 START : ${TIME}"

./pot -f arg/arguments_potential_c.txt > c.log

TIME=`date`
echo " @@@ JOB 1 END   : ${TIME}"
TIME=`date`
echo " @@@ JOB 2 START : ${TIME}"

./pot -f arg/arguments_potential_s.txt > s.log

TIME=`date`
echo " @@@ JOB 2 END   : ${TIME}"
