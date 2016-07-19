#!/usr/bin/python

if __name__ == "__main__":
    import sys
    import math

    argv = sys.argv
    argc = len(argv)
    
    if (argc != 3):
        print "usage: %s [ifile1] [ifile2]" % argv[0]
        quit()
    
    ifile1 = open(argv[1], 'r')
    ifile2 = open(argv[2], 'r')
    
    lines1 = ifile1.readlines()
    lines2 = ifile2.readlines()
    
    ifile1.close()
    ifile2.close()

    CompEps = float(10**(-30))
    
    Count_Zero_r    =  0
    Count_CompEps_r =  0
    Count_p_r       = [0] * 100
    Count_m_r       = [0] * 100

    Count_Zero_i    =  0
    Count_CompEps_i =  0
    Count_p_i       = [0] * 100
    Count_m_i       = [0] * 100
    
    tot_count       =  0
    
    if (len(lines1) != len(lines2)):
        print "ERROR: #.line of each files is different."
        quit()
    
    Nline = len(lines1)
    
    for l in range(Nline):
        words1 = lines1[l].split()
        words2 = lines2[l].split()
        
        if (len(words1) != len(words2)):
            print "ERROR: #.word of each files is different."
            quit()
        
        Nword = len(words1)
        
        for w in range((Nword-1)/2):
            dr1 = float(words1[2*w+1])
            dr2 = float(words2[2*w+1])
            
            if   (abs(dr1+dr2) == 0.0    ):
                Count_Zero_r    += 1
            elif (abs(dr1+dr2) <  CompEps):
                Count_CompEps_r += 1
            else:
                diff_r = 2 * abs((dr1-dr2)/(dr1+dr2))
                
                if   (diff_r == 0.0):
                    Count_Zero_r += 1
                else:
                    tmp_r = math.log10(diff_r)
                    
                    if (tmp_r > 0):
                        Count_p_r[ int(tmp_r)] += 1
                    else:
                        Count_m_r[-int(tmp_r)] += 1
            
            di1 = float(words1[2*w+2])
            di2 = float(words2[2*w+2])
            
            if   (abs(di1+di2) == 0.0    ):
                Count_Zero_i    += 1
            elif (abs(di1+di2) <  CompEps):
                Count_CompEps_i += 1
            else:
                diff_i = 2 * abs((di1-di2)/(di1+di2))
                
                if   (diff_i == 0.0):
                    Count_Zero_i += 1
                else:
                    tmp_i = math.log10(diff_i)
                    
                    if (tmp_i > 0):
                        Count_p_i[ int(tmp_i)] += 1
                    else:
                        Count_m_i[-int(tmp_i)] += 1
            
            tot_count += 2
    
    count_sum_r = Count_Zero_r + Count_CompEps_r + sum(Count_p_r) + sum(Count_m_r)
    count_sum_i = Count_Zero_i + Count_CompEps_i + sum(Count_p_i) + sum(Count_m_i)
    count_sum   = count_sum_r  + count_sum_i
    print "\n======================== CHACK RESULTS ========================"
    print "   #.ZERO    DIFFERENCE (REAL)\t= %d" % Count_Zero_r
    print "   #.CompEps DIFFERENCE (REAL)\t= %d\n" % Count_CompEps_r
    for i in range(99, -1, -1):
        if (Count_p_r[i] != 0):
            print "   #.1e%d DIFFERENCE (REAL)\t= %d" % (i, Count_p_r[i])
    
    for i in range(100):
        if (Count_m_r[i] != 0):
            print "   #.1e%d DIFFERENCE (REAL)\t= %d" % (-i, Count_m_r[i])
            
    print

    print "   #.ZERO    DIFFERENCE (IMAG)\t= %d" % Count_Zero_i
    print "   #.CompEps DIFFERENCE (IMAG)\t= %d\n" % Count_CompEps_i
    for i in range(99, -1, -1):
        if (Count_p_i[i] != 0):
            print "   #.1e%d DIFFERENCE (IMAG)\t= %d" % (i, Count_p_i[i])

    for i in range(100):
        if (Count_m_i[i] != 0):
            print "   #.1e%d DIFFERENCE (IMAG)\t= %d" % (-i, Count_m_i[i])

    print "\n   TOTAL SUM   = %d" % count_sum
    print "   TOTAL CHACK = %d" % tot_count
    print "===============================================================\n"
