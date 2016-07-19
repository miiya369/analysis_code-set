#!/usr/bin/python

if __name__ == "__main__":
    import sys
    import math
    
    argv = sys.argv
    argc = len(argv)
    
    if (argc < 3):
        print "usage: %s [ofile] [ifile1] [ifile2] ..." % argv[0]
        sys.exit(1)
    
    Naverage = argc - 2
    ipath    = argv
    opath    = argv[1].strip()
    
    Fcorr = open(ipath[2].strip(), 'r')
    Lcorr = Fcorr.readlines()
    Fcorr.close()
    
    Tsize   = len(Lcorr)
    CorrAve = [0.0] * (Tsize * 10) # 10 -> Momentum=5 * Real,Imag
    
    for it in range(Tsize):
        for i in range(10):
            Corr = Lcorr[it].split()
            CorrAve[i + 10 * it] = float(Corr[1+i])
    
    for iave in range(1, Naverage):
        Fcorr = open(ipath[iave+2].strip(), 'r')
        Lcorr = Fcorr.readlines()
        Fcorr.close()
        
        if (len(Lcorr) != Tsize):
            sys.exit("ERROR: Invalid Tsize in %s." % ipath[iave+2].strip())
        
        for it in range(Tsize):    
            for i in range(10):
                Corr = Lcorr[it].split()
                CorrAve[i + 10 * it] += float(Corr[1+i])
    
    Fcorr = open(opath, 'w')
    for it in range(Tsize):
        Fcorr.write("%d %1.16e %1.16e %1.16e %1.16e %1.16e %1.16e %1.16e %1.16e %1.16e %1.16e\n" % \
                        (it, CorrAve[0 + 10 * it] / float(Naverage), \
                             CorrAve[1 + 10 * it]   / float(Naverage), \
                             CorrAve[2 + 10 * it]   / float(Naverage), \
                             CorrAve[3 + 10 * it]   / float(Naverage), \
                             CorrAve[4 + 10 * it]   / float(Naverage), \
                             CorrAve[5 + 10 * it]   / float(Naverage), \
                             CorrAve[6 + 10 * it]   / float(Naverage), \
                             CorrAve[7 + 10 * it]   / float(Naverage), \
                             CorrAve[8 + 10 * it]   / float(Naverage), \
                             CorrAve[9 + 10 * it]   / float(Naverage)))
    Fcorr.close()
    
#    print "Nave = %d" % Naverage # Debug
