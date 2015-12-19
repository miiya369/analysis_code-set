#!/bin/sh

# For S = -2 Octet baryons

#channel="Lam-Lam Pro-XiM Neu-XiZ SigP-SigM SigZ-SigZ SigZ-Lam"
#counter=0
#
#for snk in $channel
#do
#    for src in $channel
#    do
#	snksrc="${snk}_${src}"
#	case $snk in
#	    "Lam-Lam") had1="Lambda"; had2="Lambda";;
#	    "Pro-XiM") had1="proton"; had2="Xi";;
#	    "Neu-XiZ") had1="proton"; had2="Xi";;
#	    "SigP-SigM") had1="Sigma"; had2="Sigma";;
#	    "SigZ-SigZ") had1="Sigma"; had2="Sigma";;
#	    "SigZ-Lam") had1="Sigma"; had2="Lambda";;
#	esac
#	num=`printf "%02d" ${counter}`
#	echo "else if (channel_name == \"${snksrc}\") {"
#	echo "name      = channel_name;"
#	echo "directory = \"BBwave.dir\";"
#	echo "number    = \"2.${num}\";"
#	echo "hadron1.set(\"${had1}\");"
#	echo "hadron2.set(\"${had2}\");"
#	echo "}"
#	counter=$((counter+1))
#    done
#done

# For S = -3 Octet baryons

channel="XiZ-Lam XiZ-SigZ XiM-SigP"
counter=0

for snk in $channel
do
    for src in $channel
    do
	snksrc="${snk}_${src}"
	case $snk in
	    "XiZ-Lam") had1="Xi"; had2="Lambda";;
	    "XiZ-SigZ") had1="Xi"; had2="Sigma";;
	    "XiM-SigP") had1="Xi"; had2="Sigma";;
	esac
	num=`printf "%02d" ${counter}`
	echo "else if (channel_name == \"${snksrc}\") {"
	echo "name      = channel_name;"
	echo "directory = \"BBwave.dir\";"
	echo "number    = \"3.${num}\";"
	echo "hadron1.set(\"${had1}\");"
	echo "hadron2.set(\"${had2}\");"
	echo "}"
	counter=$((counter+1))
    done
done
