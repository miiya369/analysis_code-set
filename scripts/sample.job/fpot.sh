#!/bin/bash

AAA=(12 28 40 58 90 208)
mass=(1900 2103 2155 2194 2226 2260)
r0=(0.941065 1.03735 1.05265 1.05764 1.09341 1.14768)
diff=(0.425 0.475 0.523 0.540 0.515 0.515)

dir=(Ens1_bin57 Ens2_bin40 Ens3_bin45)
time=(13 12 10)

base=/Users/miiya/Dropbox/programs/data/pacs-cs/results_kekb_pacs-cs_bridge_RHQ

for i in `seq 0 1 2`; do
    ibase=$base/${dir[$i]}/Analysis/fit/Pro-Lam_Pro-Lam_potential_S0_Sz0_fitparam_t0${time[$i]}.bin
    for j in `seq 0 1 5`; do
	obase=$base/${dir[$i]}/Analysis/fpot/Pro-Lam_Pro-Lam_potential_S0_Sz0_t0${time[$i]}.A${AAA[$j]}
	./bin/fpot -f arg/arguments_folding_potential.txt \
	    -ifile $ibase -ofile $obase \
	    -A ${AAA[$j]} -r0 ${r0[$j]} -dif ${diff[$j]} -mass ${mass[$j]}
	./bin/fpot -f arg/arguments_folding_potential.txt \
	    -ifile $ibase -ofile $obase \
	    -A ${AAA[$j]} -r0 ${r0[$j]} -dif ${diff[$j]} -mass ${mass[$j]} -phase
    done
done
