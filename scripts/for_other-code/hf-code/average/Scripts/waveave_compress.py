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

    tmp0   = 0
    tmp16  = 16
    tmp20  = 20
    tmp256 = 256
    Lsize  = [0, 0, 0]
    length = 0
    
    Fsize  = os.path.getsize(ipath[2].strip())

### Read Header
    ifile = open(ipath[2].strip(), 'rb')
    dummy = struct.unpack('<i', ifile.read(4))[0]
    if (dummy != 16):
        sys.exit("The file '%s' may be not Ishii-san's compressed NBS data." % ipath[2].strip())

    Lsize[0] = struct.unpack('<i', ifile.read(4))[0]
    Lsize[1] = struct.unpack('<i', ifile.read(4))[0]
    Lsize[2] = struct.unpack('<i', ifile.read(4))[0]
    length   = struct.unpack('<i', ifile.read(4))[0]
    RepLst   = [0] * length

    dummy = struct.unpack('<i', ifile.read(4))[0]
    dummy = struct.unpack('<i', ifile.read(4))[0]
    if (dummy != length * 4):
        sys.exit("The file '%s' may be not Ishii-san's compressed NBS data." % ipath[2].strip())
    
    for i in range(length):
        RepLst[i] = struct.unpack('<i', ifile.read(4))[0]

    dummy = struct.unpack('<i', ifile.read(4))[0]
    if (dummy != length * 4):
        sys.exit("The file '%s' may be not Ishii-san's compressed NBS data." % ipath[2].strip())
    ifile.close()
#########

    skip_len = 16 + length
    WaveSize = 32 * length
    Wave     = [0.0] * WaveSize
    
    for iave in range(Naverage):
        fsize = os.path.getsize(ipath[iave+2].strip())
        if (Fsize  != fsize):
            sys.exit("ERROR: Defferent file size, %s" % ipath[iave+2].strip())
        
        ifwave     = open(ipath[iave+2].strip(), 'rb')
        dummy_skip = ifwave.read(skip_len * 4)[0]
        
        for i in range(WaveSize):
            Wave[i] += struct.unpack('<d', ifwave.read(8))[0] / float(Naverage)
        
        dummy = struct.unpack('<i', ifwave.read(4))[0]
        if (dummy != length * 256):
            sys.exit("The file '%s' may be not Ishii-san's compressed NBS data." % ipath[iave+2].strip())
        
        ifwave.close()
    
    ofwave = open(opath, 'wb')
    ofwave.write(struct.pack('<i', tmp16))
    ofwave.write(struct.pack('<i', Lsize[0]))
    ofwave.write(struct.pack('<i', Lsize[1]))
    ofwave.write(struct.pack('<i', Lsize[2]))
    ofwave.write(struct.pack('<i', length))
    ofwave.write(struct.pack('<i', tmp16))
    ofwave.write(struct.pack('<i', length * 4))
    for i in range(length):
        ofwave.write(struct.pack('<i', RepLst[i]))
    ofwave.write(struct.pack('<i', length * 4))
    ofwave.write(struct.pack('<i', tmp20))
    ofwave.write(struct.pack('<i', tmp0))
    ofwave.write(struct.pack('<i', tmp0))
    ofwave.write(struct.pack('<i', tmp0))
    ofwave.write(struct.pack('<i', length))
    ofwave.write(struct.pack('<i', tmp256))
    ofwave.write(struct.pack('<i', tmp20))
    ofwave.write(struct.pack('<i', length * 256))
    for i in range(WaveSize):
        ofwave.write(struct.pack('<d', Wave[i]))
    ofwave.write(struct.pack('<i', length * 256))
    ofwave.close()
    
#    print "Nave = %d" % Naverage # Debug
