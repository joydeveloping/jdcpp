#!/usr/bin/env python

'''
Script for run test_mpi on mvs.

Usage:
  ./Run.py -h - print this text
  ./Run.py <mode> <test> <dir> <pcs> - run <test> in directory <dir>
                                       with given list of processes count (<pcs>)
    <mode> is one of "local" or "mvs"
'''

import sys
import os
import os.path
import subprocess

#---------------------------------------------------------------------------------------------------
# Globals.
#---------------------------------------------------------------------------------------------------

Local_File = "test_mpi.local"
Mvs_File = "test_mpi.mvs"
Mvs_Mic_File = "test_mpi.mvs.mic"
Is_Local = False
Is_Mvs = False

#---------------------------------------------------------------------------------------------------
# Functions.
#---------------------------------------------------------------------------------------------------

'''
Print help.
'''
def Print_Help():
    print "Script for run test_mpi on mvs."
    print ""
    print "Usage:"
    print "  ./Run.py -h - print this text"
    print "  ./Run.py <mode> <test> <dir> <pcs> - run <test> in directory <dir>"
    print "                                       with given list of processes count (<pcs>)"
    print '    <mode> is one of "local" or "mvs"'

#---------------------------------------------------------------------------------------------------

'''
Run test in given directory and given set of processes count.

Arguments:
  t - test
  d - directory name
  pcs - list of processes counts
'''
def Run(t, d, pcs):
    print('Run "%s" in directory "%s" with "%s" processes.' % (t, d, pcs))

    # Prepare directory.
    if (not os.path.exists(d)):
        os.mkdir(d)
        print('Directory "%s" is created.' % d)

    # We need to run with each processes_counts element.
    pcs_list = pcs.split()
    for pc in pcs_list:
        Run_Single(t, d, int(pc))

#---------------------------------------------------------------------------------------------------

'''
Run test in created directory for given count of processes.

Arguments:
  t - test
  d - directory name
  pc - processes count
'''
def Run_Single(t, d, pc):
    print('Run "%s" in directory "%s" with %s processes.' % (t, d, pc))
    cwd = os.getcwd()

    # Create subdirectory d/run_00pc.
    subdir = "%s/run_%03d" % (d, pc)
    if (not os.path.exists(subdir)):
        os.mkdir(subdir)
        print('  Subdirectory "%s" is created.' % subdir)

    # Host file.
    if (Is_Mvs):
        Create_Hosts_File("%s/hosts" % subdir, pc)
        print('  Hosts file "%s/hosts" is created.' % subdir)

    # Commands.
    if (Is_Local):
        cmds = ["cd %s" % subdir,
                "cp %s/test_mpi.local ." % cwd,
                "mpirun -np %d ./test_mpi.local %s" % (pc, t),
                "cd %s" % cwd]
    elif (Is_Mvs):
        cmds = ["cd %s" % subdir,
                "cp %s/test_mpi.mvs ." % cwd,
                "cp %s/test_mpi.mvs.mic ."  % cwd,
                "mpirun -np %d -machinefile hosts -mic -maxtime 180 test_mpi.mvs %s" % (pc, t),
                "cd %s" % cwd]
    else:
        assert(false)

    # Run.
    for cmd in cmds:
        print "  " + cmd
    cmd = reduce(lambda x, y: x + " ; " + y, cmds)
    subprocess.call(cmd, shell = True)

#---------------------------------------------------------------------------------------------------

'''
Create hosts file.

Arguments:
  fn - file name
  pc - processes count on Xeon
'''
def Create_Hosts_File(fn, pc):
    f = open(fn, "w")

    # Write hosts.
    cur = 1
    pc_left = pc
    while (pc_left > 10):
        f.write("node%d:0\nnode%d:5\nnode%d:5\n" % (cur, cur + 1, cur + 2))
        cur += 3
        pc_left -= 10
    if (pc_left > 5):
        f.write("node%d:0\nnode%d:5\nnode%d:%d\n" % (cur, cur + 1, cur + 2, pc_left - 5))
    else:
        f.write("node%d:0\nnode%d:%d\nnode%d:0\n" % (cur, cur + 1, pc_left, cur + 2))

    f.close()

#---------------------------------------------------------------------------------------------------
# Script body.
#---------------------------------------------------------------------------------------------------

# Get args count.
args_count = len(sys.argv) - 1

# Analyze arguments.
if (args_count == 1):

    # Only -h.
    if (sys.argv[1] == "-h"):
        Print_Help()
    else:
        assert(false)

elif (args_count == 4):

    # Check exe file.
    if (sys.argv[1] == "local"):
        Exe_File = Local_File
        Is_Local = True
        assert(os.path.exists(Local_File))
    elif (sys.argv[1] == "mvs"):
        Exe_File = Mvs_File
        Is_Mvs = True
        assert(os.path.exists(Mvs_File))
        assert(os.path.exists(Mvs_Mic_File))
    else:
        assert(false)

    Run(sys.argv[2], sys.argv[3], sys.argv[4])

else:
    assert(false)

#---------------------------------------------------------------------------------------------------

