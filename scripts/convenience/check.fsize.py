#!/usr/bin/python

if __name__ == "__main__":
    import sys
    argv = sys.argv
    argc = len(argv)
    
    if (argc != 3):
        print "usage: %s [file1] [file2]" % argv[0]
        quit()
    
    file1  = open(argv[1], 'r')
    lines1 = file1.readlines()
    file1.close()
    
    file2  = open(argv[2], 'r')
    lines2 = file2.readlines()
    file2.close()
    
    len1 = len(lines1)
    len2 = len(lines2)
    
    if (len1 != len2):
        print "Two files are different length !"
        quit()
    
    Ndiff = 0
    
    for i in range(len1):
        fsize1 = lines1[i].split()
        fsize2 = lines2[i].split()
        
        if (fsize1[4] != fsize2[4]):
            print "Different file size: %s != %s" % (lines1[i].strip(), lines2[i].strip())
            Ndiff += 1
    
    if (Ndiff == 0):
        print "Same."
