#!/usr/bin/python

if __name__ == "__main__":
    import sys
    import math
    import os.path
    import struct
    
    argv = sys.argv
    argc = len(argv)
    
    if (argc != 4):
        print "usage: %s [binary file] [endian convert (0=no, 1=yes)] [component number (0,1,2,...)]" % argv[0]
        quit()
    
    ifsize     = os.path.getsize(argv[1].strip())
    endian_cnv = int(argv[2])
    index      = int(argv[3])
    
    if (ifsize % 8 != 0):
        print "Invalid file size: fsize % sizeof(double) != 0"
        quit()
    
    dfsize = ifsize / 8

    if (dfsize < index+1):
        print "Too large component: ifile[%d] is max" % (dfsize-1)
        quit()

    ifile  = open(argv[1].strip(), 'rb')
    for i in range(dfsize):
        tmp_d = struct.unpack('d', ifile.read(8))[0]
        
        if (i == index):
            if (endian_cnv == 1):
                tmp_d = struct.unpack(">d",struct.pack("<d",tmp_d))[0]
            print "ifile[%d] = %1.16e" % (index, tmp_d)
