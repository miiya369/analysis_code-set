#!/usr/bin/python

if __name__ == "__main__":
    import sys
    import math
    import os.path
    import struct
    
    argv = sys.argv
    argc = len(argv)
    
    if (argc != 3):
        print "usage: %s [binary file 1] [binary file 2]" % argv[0]
        quit()
    
    ifsize1 = os.path.getsize(argv[1].strip())
    ifsize2 = os.path.getsize(argv[2].strip())
    
    if (ifsize1 != ifsize2):
        print "Two files are different size."
        print "%d != %d" % (ifsize1, ifsize2)
        quit()
    
    if (ifsize1 % 8 != 0):
        print "Invalid file size: fsize % sizeof(double) != 0"
        quit()
    
    dfsize = ifsize1 / 8
    if1    = open(argv[1].strip(), 'rb')
    if2    = open(argv[2].strip(), 'rb')
    for i in range(dfsize):
        tmp_d1 = struct.unpack('d', if1.read(8))[0]
        tmp_d2 = struct.unpack('d', if2.read(8))[0]
        
        if (tmp_d1 != tmp_d2):
            print "Two files have different component."
            if1.close()
            if2.close()
            quit()
    
    print "Two files are same."
    if1.close()
    if2.close()
