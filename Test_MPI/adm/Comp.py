#!/usr/bin/env python

'''
Test_MPI compilation script.

Usage:
  ./Comp.py - print this text
  ./Comp.py local - build program for local run
  ./Comp.py mvs - build program for mvs cluster
'''

import sys
import subprocess

#---------------------------------------------------------------------------------------------------
# Globals.
#---------------------------------------------------------------------------------------------------

#---------------------------------------------------------------------------------------------------
# Functions.
#---------------------------------------------------------------------------------------------------

'''
Print help.
'''
def Print_Help():
    print "Test_MPI compilation script."
    print ""
    print "Usage:"
    print "  ./Comp.py - print this text"
    print "  ./Comp.py local - build program for local run"
    print "  ./Comp.py mvs - build program for mvs cluster"

#---------------------------------------------------------------------------------------------------
# Script body.
#---------------------------------------------------------------------------------------------------

# Get argument.
assert(len(sys.argv) == 2)
arg = sys.argv[1]

# Compilation parameters.
srcs = "./src/*.cpp ../Lib/IO/*.cpp ../Lib/MPI/*.cpp"
cmds = []

# Analyze argument.
if (arg == "-h"):
    Print_Help()
elif (arg == "local"):
    cmds = ["rm -f test_mpi.*",
            "mpic++ -O3 " + srcs + " -I./src -I.. -o test_mpi.local -lm"]
elif (arg == "mvs"):
    cmds = ["rm -f test_mpi.*",
            "mpicc -O3 " + srcs + " -I./src -I.. -o test_mpi.mvs -lm",
            "mpicc -O3 " + srcs + " -I./src -I.. -o test_mpi.mvs.mic -mmic -lm"]
else:
    assert(False)

# Run compilation.
print "Prepare to execute commands:"
if (cmds != []):
    for cmd in cmds:
        print "  " + cmd
    cmd = reduce(lambda x, y: x + " ; " + y, cmds)
    subprocess.call(cmd, shell = True)

#---------------------------------------------------------------------------------------------------

