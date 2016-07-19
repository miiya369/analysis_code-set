#!/usr/bin/python

if __name__ == "__main__":
    import sys
    import math
    import os.path
    import struct

    argv = sys.argv
    argc = len(argv)
    
    if (argc != 3):
        print "usage: %s [ifile1] [ifile2]" % argv[0]
        quit()
    
    fsize1 = os.path.getsize(argv[1].strip())
    fsize2 = os.path.getsize(argv[2].strip())

    if (fsize1 != fsize2):
        print "ERROR: The size of each files is different."
        quit()
        
    Lsize  = [0, 0, 0]
    length = 0
    
### Read Header 
    ifile = open(argv[1], 'rb')

    dummy = struct.unpack('<i', ifile.read(4))[0]
    if (dummy != 16):
        print "The file '%s' may be not Ishii-san's compressed NBS data." % argv[1].strip()
        quit()
    
    Lsize[0] = struct.unpack('<i', ifile.read(4))[0]
    Lsize[1] = struct.unpack('<i', ifile.read(4))[0]
    Lsize[2] = struct.unpack('<i', ifile.read(4))[0]
    length   = struct.unpack('<i', ifile.read(4))[0]

    ifile.close()
#########

    skip_len = 16 + length
    WaveSize = 32 * length

    ifile1 = open(argv[1], 'rb')
    ifile2 = open(argv[2], 'rb')

    dummy_skip = ifile1.read(skip_len * 4)[0]
    dummy_skip = ifile2.read(skip_len * 4)[0]

    CompEps = float(10**(-100))
    
    Count_Zero    =  0
    Count_CompEps =  0
    Count_p       = [0] * 100
    Count_m       = [0] * 100

    for i in range(WaveSize):
        tmp1 = struct.unpack('<d', ifile1.read(8))[0]
        tmp2 = struct.unpack('<d', ifile2.read(8))[0]
        
        if   (abs(tmp1+tmp2) == 0.0    ):
            Count_Zero    += 1
        elif (abs(tmp1+tmp2) <  CompEps):
            Count_CompEps += 1
        else:
            diff = 2 * abs((tmp1-tmp2)/(tmp1+tmp2))
            
            if   (diff == 0.0):
                Count_Zero += 1
            else:
                tmp = math.log10(diff)
                
                if (tmp > 0):
                    Count_p[ int(tmp)] += 1
                else:
                    Count_m[-int(tmp)] += 1
    
    count_sum = Count_Zero + Count_CompEps + sum(Count_p) + sum(Count_m)
    print "\n======================== CHACK RESULTS ========================"
    print "   #.ZERO    DIFFERENCE \t= %d" % Count_Zero
    print "   #.CompEps DIFFERENCE \t= %d\n" % Count_CompEps
    for i in range(99, -1, -1):
        if (Count_p[i] != 0):
            print "   #.1e%d DIFFERENCE \t= %d" % (i, Count_p[i])
    
    for i in range(100):
        if (Count_m[i] != 0):
            print "   #.1e%d DIFFERENCE \t= %d" % (-i, Count_m[i])
    print
    print "   TOTAL SUM   = %d" % count_sum
    print "   TOTAL CHACK = %d" % WaveSize
    print "===============================================================\n"
