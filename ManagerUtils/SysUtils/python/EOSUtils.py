#*******************************************************************************
 #
 #  Filename    : EOSUtils.py
 #  Description : Helper functiosn for eos system interaction
 #  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 #
 #  Mainly using xrdfs tools, should be available in CMSSW environment
 #
#*******************************************************************************
import os
import subprocess

def listremote(siteurl, query):
    proc = subprocess.Popen(["xrdfs", siteurl, "ls", query ], stdout=subprocess.PIPE, stderr=subprocess.PIPE )
    out, err = proc.communicate()
    if err :
        print "Error detected when calling xrdfs!"
        print ">>> xrdfs", siteurl, "ls", query
        print "Have you setup your permission? (voms-proxy-init -voms cms -valid 192:0)"
        raise Exception("Error in input!")
    return out.split()
