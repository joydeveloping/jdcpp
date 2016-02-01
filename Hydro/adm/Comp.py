#!/usr/bin/env python

'''
Hydro compilation script.

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
    print "Hydro compilation script."
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
srcs = "./src/*.cpp ./src/Grid/*.cpp ./src/Solver/*.cpp ../Lib/IO/*.cpp ../Lib/MPI/*.cpp ../Lib/Math/*.cpp ../Lib/OMP/*.cpp"
cmds = []

# Analyze argument.
if (arg == "-h"):
    Print_Help()
elif (arg == "local"):
    cmds = ["rm -f hydro.*",
            "mpic++ -O3 " + srcs + " -I./src -I.. -o hydro.local -lm -fopenmp"]
elif (arg == "mvs"):
    cmds = ["rm -f hydro.*",
            "mpicc -O3 " + srcs + " -I./src -I.. -o hydro.mvs -lm -fopenmp",
            "mpicc -O3 " + srcs + " -I./src -I.. -o hydro.mvs.mic -mmic -lm -fopenmp"]
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

