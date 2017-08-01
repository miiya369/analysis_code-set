#!/usr/bin/python

if __name__ == "__main__":
    import sys
    import math
    import cmath
    
    argv = sys.argv
    argc = len(argv)
    
    if (argc != 4):
        print "usage: %s [ifile.cps] [ifile.bridge] [hadron.cps]" % argv[0]
        quit()
    
    Ntime   = 64 ### <-- Change Here !

    CorrCPS = [0] * Ntime
    CorrBRG = [0] * Ntime
    
    ifileCPS = open(argv[1], 'r')
    ifileBRG = open(argv[2], 'r')
    linesCPS = ifileCPS.readlines()
    linesBRG = ifileBRG.readlines()
    ifileCPS.close()
    ifileBRG.close()
    
    if (len(linesBRG) != Ntime):
        print "#.line of Bridge's file is not %d" % Ntime
        quit()
    
    check = 0
    for l in range(len(linesCPS)):
        words = linesCPS[l].split()
        
        if (len(words) != 0):
            if (words[0] == "OPER:"):
                if (words[1].strip() == argv[3].strip()):
                    check = 1
                    for ll in range(Ntime):
                        TmpWords    = linesCPS[l+ll+1].split()
                        CorrCPS[ll] = float(TmpWords[1])
    
    if (check == 0):
        print "There is not '%s' in the CPS's file" % argv[3]
        quit()
    
    for l in range(Ntime):
        words      = linesBRG[l].split()
        CorrBRG[l] = float(words[1])
    
    for l in range(Ntime):
        print "%e / %e = %e" % (CorrCPS[l], CorrBRG[l], (CorrCPS[l]/CorrBRG[l]))
