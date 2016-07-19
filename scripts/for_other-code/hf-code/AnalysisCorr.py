#!/usr/bin/python

Nconf = 1
Tsize = 1

mom   = 0 # <- Momentum

def SetPath(CONF, HAD, TSHIFT, XYZSHIFT = "000.000.000"): # <- You may change XYZshift
    return "%s/%s_correlator.+%s.%s.%s" % (CONF, HAD, TSHIFT, XYZSHIFT, CONF),

def makeJK(CorrOrg, CorrJK, do_JK = 0): # <- You may change do_JK (1 = Do Jack Knife mean)
    if (Nconf != 1 and do_JK == 1):
        for t in range(Tsize):
            TmpSum = complex(0.0, 0.0)
            
            for i in range(Nconf):
                TmpSum += Corr[t + Tsize * i]
            for i in range(Nconf):
                CorrJK[t + Tsize * i] = (TmpSum - Corr[t + Tsize * i]) / float(Nconf-1)
    else:
        for l in range(Tsize * Nconf):
            CorrJK[l] = Corr[l]

if __name__ == "__main__":
    import sys
    import math
    import cmath
    
    argv = sys.argv
    argc = len(argv)
    
    if (argc != 6):
        print "usage: %s [conf list] [ibase] [Hadron Name] [Tshift] [0=OrgCorr, 1=Effmass]" \
            % argv[0]
        quit()
    
    ConfList  = argv[1]
    ibase     = argv[2]
    Hadron    = argv[3]
    Tshift    = argv[4]
    CorEffFlg = int(argv[5])
    
    fconf = open(ConfList.strip(), 'r')
    Confs = fconf.readlines()
    Nconf = len(Confs)
    fconf.close()
    
    ifname = "%s/%s" % (ibase, SetPath(Confs[0].strip(), Hadron, Tshift)[0])
    fcorr = open(ifname, 'r')
    Lcorr = fcorr.readlines()
    Tsize = len(Lcorr)
    fcorr.close()
    
    print(" Results dir = %s" % ibase)
    print(" Conf list   = %s" % ConfList)
    print(" #.conf      = %d" % Nconf)
    print(" Time size   = %d" % Tsize)
    print(" Time shift  = %s" % Tshift)
    print(" Hadron Name = %s" % Hadron)
    
    Corr = [0.0] * Tsize * Nconf
    
    for i in range(Nconf):
        ifname = "%s/%s" % (ibase, SetPath(Confs[i].strip(), Hadron, Tshift)[0])
        fcorr = open(ifname, 'r')
        Lcorr = fcorr.readlines()
        fcorr.close()
        
        for l in range(Tsize):
            Wcorr = Lcorr[l].split()
            Corr[l + Tsize * i] = complex(float(Wcorr[1 + 2*mom]), float(Wcorr[2 + 2*mom]))
    
    JKcorr = [0.0] * Tsize * Nconf
    makeJK(Corr, JKcorr)
    
    MeanR  = [0.0] * Tsize
    Mean2R = [0.0] * Tsize
    ErrR   = [0.0] * Tsize
    MeanI  = [0.0] * Tsize
    Mean2I = [0.0] * Tsize
    ErrI   = [0.0] * Tsize
    
    if (CorEffFlg == 0):
        for l in range(Tsize):
            for i in range(Nconf):
                Correlator = JKcorr[l + Tsize * i]
                
                MeanR [l] += Correlator.real
                Mean2R[l] += Correlator.real ** 2
                MeanI [l] += Correlator.imag
                Mean2I[l] += Correlator.imag ** 2
            
            MeanR [l] /= float(Nconf)
            Mean2R[l] /= float(Nconf)
            MeanI [l] /= float(Nconf)
            Mean2I[l] /= float(Nconf)
            ErrR[l]    = math.sqrt((Nconf-1) * (Mean2R[l] - MeanR[l] ** 2))
            ErrI[l]    = math.sqrt((Nconf-1) * (Mean2I[l] - MeanI[l] ** 2))
            print "%d %e %e %e %e" % (l, MeanR[l], ErrR[l], MeanI[l], ErrI[l])
    elif (CorEffFlg == 1):
        for l in range(Tsize-1):
            for i in range(Nconf):
                if (JKcorr[l+1 + Tsize * i] == complex(0.0, 0.0)):
                    break
                elif (JKcorr[l + Tsize * i] == complex(0.0, 0.0)):
                    break
                
                Effmass = -cmath.log(JKcorr[l+1 + Tsize * i] / JKcorr[l + Tsize * i])
                
                MeanR [l] += Effmass.real
                Mean2R[l] += Effmass.real ** 2
                MeanI [l] += Effmass.imag
                Mean2I[l] += Effmass.imag ** 2
            
            MeanR [l] /= float(Nconf)
            Mean2R[l] /= float(Nconf)
            MeanI [l] /= float(Nconf)
            Mean2I[l] /= float(Nconf)
            ErrR[l]    = math.sqrt((Nconf-1) * (Mean2R[l] - MeanR[l] ** 2))
            ErrI[l]    = math.sqrt((Nconf-1) * (Mean2I[l] - MeanI[l] ** 2))
            print "%d %e %e %e %e" % (l, MeanR[l], ErrR[l], MeanI[l], ErrI[l])
