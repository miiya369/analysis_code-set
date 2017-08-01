#!/usr/bin/python

if __name__ == "__main__":
    import struct
    import subprocess
    
    otmp1 = 1
    
    ofile = open("endian_check_tmp.bin", 'wb')
    ofile.write(struct.pack('i', otmp1))
    ofile.close()
    
    ifile = open("endian_check_tmp.bin", 'rb')
    itmp1 = struct.unpack('<i', ifile.read(4))[0]
    ifile.close()
    
    cmd = "rm endian_check_tmp.bin"
    subprocess.call(cmd.strip().split())
    
    if (otmp1 == itmp1):
        print "This machine is little endian."
    else:
        print "This machine is big endian."
