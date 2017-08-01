#!/usr/bin/python

if __name__ == "__main__":
    import sys
    import math
    import os.path
    import struct
    
    argv = sys.argv
    argc = len(argv)
    
    if (argc < 3):
        print "usage: %s [ofile] [ifile1] [ifile2] ..." % argv[0]
        sys.exit(1)
    
    Naverage = argc - 2
    ipath    = argv
    opath    = argv[1].strip()
    
    Fsize    = os.path.getsize(ipath[2].strip())
    if (Fsize % 8 != 0):
        sys.exit("Invalid file size: fsize % sizeof(double) != 0")
    
    WaveSize = Fsize / 8
    Wave     = [0.0] * WaveSize    
    
    for iave in range(Naverage):
        fsize = os.path.getsize(ipath[iave+2].strip())
        if (Fsize  != fsize):
            sys.exit("ERROR: Defferent file size, %s" % ipath[iave+2].strip())
        
        ifwave = open(ipath[iave+2].strip(), 'rb')
        for i in range(WaveSize):
            Wave[i] += struct.unpack('d', ifwave.read(8))[0] / float(Naverage)
        
        ifwave.close()
    
    ofwave = open(opath, 'wb')
    for i in range(WaveSize):
        ofwave.write(struct.pack('d', Wave[i]))
    
    ofwave.close()
    
#    print "Nave = %d" % Naverage # Debug
