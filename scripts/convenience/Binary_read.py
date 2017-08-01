#!/usr/bin/python

if __name__ == "__main__":
    import sys
    import math
    import os.path
    import struct
    
    argv = sys.argv
    argc = len(argv)
    
    if (argc < 3):
        print "usage: %s [Binary file] [Format1] [Format2] ..." % argv[0]
        print "     : Format should be C's one (int, double, ...)"
        print "     : To skip the byte, put [bytes (Normal Num.)]"
        quit()
    
    ifile = open(argv[1].strip(), 'rb')
    fsize = os.path.getsize(argv[1].strip())
    print "Start: file size = %d" % fsize

    Bcount = 0
    for i in range(argc-2):
        if (argv[i+2].strip() == "int"):
            if (Bcount + 4 > fsize):
                break
            print "%10d: ( int32) %d" % (Bcount, struct.unpack('i', ifile.read(4))[0])
            Bcount += 4
        elif (argv[i+2].strip() == "double"):
            if (Bcount + 8 > fsize):
                break
            print "%10d: (double) %1.16e" % (Bcount, struct.unpack('d', ifile.read(8))[0])
            Bcount += 8
        elif (argv[i+2].strip().isdigit()):
            if (Bcount + int(argv[i+2]) > fsize):
                break
            print "%10d:          SKIP %d bytes" % (Bcount, int(argv[i+2]))
            dummy   = ifile.read(int(argv[i+2]))
            Bcount += int(argv[i+2])
        else:
            print "Invalid format '%s'" % argv[i+2]
            quit()
    
    print "Finish"
    ifile.close()
