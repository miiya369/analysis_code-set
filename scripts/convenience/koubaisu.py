#!/usr/bin/python

if __name__ == "__main__":
    import sys
    import math
    
    argv = sys.argv
    argc = len(argv)
    
    if (argc != 3):
        print "usage : %s [Max i] [Max j] (i>j)\n@ Then, find value i%%j=0." % argv[0]
        quit()
    
    max_i = int(argv[1])
    max_j = int(argv[2])
    
    if (max_i < max_j):
        print "usage : %s [Max i] [Max j] (i>j)\n@ Then, find value i%%j=0." % argv[0]
        quit()
    
    for i in range(max_j, max_i+1):
        for j in range(1, max_j+1):
            if (i%j != 0):
                break
            if (i%j == 0 and j != max_j):
                continue
            
            print "j=%d answer = %d" % (j, i)
            quit()
        tmp = i
    
    print "no answer, i=%d" % tmp
