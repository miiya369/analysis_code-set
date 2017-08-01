#!/usr/bin/python

if __name__ == "__main__":
    import sys
    import math
    
    argv = sys.argv
    argc = len(argv)
    
    if (argc != 3):
        print "usage : %s [value] [#.yakusu]" % argv[0]
        quit()
    
    valu    = int(argv[1])
    Nyakusu = int(argv[2])
    
    tmp_j   = 2
    
    for i in range(Nyakusu+1):
        for j in range(tmp_j, valu):
            if (valu%j == 0):
                print " %d" % j,
                tmp_j = j+1
                break
    print
