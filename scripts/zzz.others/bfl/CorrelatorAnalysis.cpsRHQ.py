#!/usr/bin/python

if __name__ == "__main__":
    import sys
    import math
    import cmath
    
    argv = sys.argv
    argc = len(argv)
    
    if (argc != 5):
        print "usage: %s [ifiles (Use '@' for wildcard of number)] [#.conf] [0=OrgCorr, 1=Effmass] [HadronOp]" \
            % argv[0]
        quit()
    
    fileN     = argv[1]
    Nconf     = int(argv[2])
    CorEffFlg = int(argv[3])
    HadronOp  = argv[4]
    
    Ntime = 64 ### <-- Change Here !

    Corr  = [0]*Ntime*Nconf
    
    # Err Check
    fileN_sp = fileN.split("@")
    if (len(fileN_sp) > 2):
        print "ERROR: '@' is must be put only once."
        quit()
    elif (len(fileN_sp) == 2):
        if (Nconf == 1):
            print "ERROR: #.conf = 1, but there is '@' in the file name."
            quit()
    else:
        if (Nconf > 1):
            print "ERROR: #.conf > 1, but there is not '@' in the file name."
            quit()
    
    for iconf in range(Nconf):
        if (len(fileN_sp) == 1):
            FileName = fileN
        else:
            FileName = fileN_sp[0]+"%d"%(iconf+1)+fileN_sp[1]
        
        ifile = open(FileName, 'r')
        lines = ifile.readlines()
        ifile.close()
        
        Nline = len(lines)
        for l in range(Nline):
            words = lines[l].split()
            
            if (len(words) > 0):
                if (words[0] == "OPER:"):
                    if (words[1] == HadronOp):
                        for ll in range(Ntime):
                            Tmpwords             = lines[l+ll+1].split()
                            Corr[ll+Ntime*iconf] = complex(float(Tmpwords[1]), float(Tmpwords[2]))
                        
                        l += Ntime
    
    CorrJK = [0]*Ntime*Nconf
    if (Nconf != 1):
        for l in range(Ntime):
            TmpSum = complex(0.0, 0.0)
            for iconf in range(Nconf):
                TmpSum += Corr[l+Ntime*iconf]
            for iconf in range(Nconf):
                CorrJK[l+Ntime*iconf] = (TmpSum - Corr[l+Ntime*iconf]) / float(Nconf-1)
    else:
        for l in range(Ntime):
            CorrJK[l] = Corr[l]
    
    print "# %s\n" % HadronOp,
    Mean  = [0]*Ntime
    Mean2 = [0]*Ntime
    Err   = [0]*Ntime
    
    if (CorEffFlg == 0):
        for l in range(Ntime):
            for iconf in range(Nconf):
                Correlator = CorrJK[l+Ntime*iconf]

                Mean[l]  +=  Correlator
                Mean2[l] += (Correlator * Correlator)
            
            Mean[l]  /= float(Nconf)
            Mean2[l] /= float(Nconf)
            Err[l]    = cmath.sqrt((Nconf-1)*(Mean2[l] - Mean[l]*Mean[l]))
            print "%d %e %e %e %e\n" % (l, Mean[l].real, Err[l].real, Mean[l].imag, Err[l].imag),
    elif (CorEffFlg == 1):
        for l in range(Ntime-1):
            for iconf in range(Nconf):
                Effmass = -cmath.log(CorrJK[l+1+Ntime*iconf] / CorrJK[l+Ntime*iconf])

                Mean[l]  +=  Effmass
                Mean2[l] += (Effmass * Effmass)
                
            Mean[l]  /= float(Nconf)
            Mean2[l] /= float(Nconf)
            Err[l]    = cmath.sqrt((Nconf-1)*(Mean2[l] - Mean[l]*Mean[l]))    
            print "%d %e %e %e %e\n" % (l, Mean[l].real, Err[l].real, Mean[l].imag, Err[l].imag),
