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
    Dsize   = len(Lcorr[0].split()) - 1
    CorrAve = [0.0] * (Tsize * Dsize)
    
    for it in range(Tsize):
        Corr = Lcorr[it].split()
        for i in range(Dsize):
            CorrAve[i + Dsize * it] = float(Corr[1+i])
    
    for iave in range(1, Naverage):
        Fcorr = open(ipath[iave+2].strip(), 'r')
        Lcorr = Fcorr.readlines()
        Fcorr.close()
        
        if (len(Lcorr) != Tsize):
            sys.exit("ERROR: Invalid TimeSize in %s." % ipath[iave+2].strip())
        if (len(Lcorr[0].split())-1 != Dsize):
            sys.exit("ERROR: Invalid DataSize in %s." % ipath[iave+2].strip())
            
        for it in range(Tsize):
            Corr = Lcorr[it].split()
            for i in range(Dsize):
                CorrAve[i + Dsize * it] += float(Corr[1+i])
    
    Fcorr = open(opath, 'w')
    for it in range(Tsize):
        Fcorr.write("%d" % it)
        for i in range(Dsize):
            Fcorr.write(" %1.16e" % (CorrAve[i + Dsize * it] / float(Naverage)))
        Fcorr.write("\n")
    
    Fcorr.close()
    
#    print "Tsize = %d : Dsize = %d : Nave = %d" % (Tsize, Dsize, Naverage) # Debug
