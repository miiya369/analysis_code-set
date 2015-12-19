#!/bin/sh

filename=$1

set_r="5 10 15 20"
set_t="6 7 8 9 10 11"

for RRR in $set_r
do
    for TIME in $set_t
    do
	while read LINE
	do
	    args=(${LINE})
	    if test ${args[3]} = $RRR
	    then
		echo "$TIME ${args[0]} ${args[1]} ${args[2]} ${args[3]} ${args[4]} ${args[5]}"
	    fi
	done < ${filename}${TIME}
    done
    echo; echo
done
