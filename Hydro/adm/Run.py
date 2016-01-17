#!/usr/bin/env python

'''
Script for run hydro on mvs.

Usage:
  ./Run.py -h - print this text
  ./Run.py <mode> <grid> <dir> <pcs> - run hydro with grid <grid> in directory <dir>
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

Local_File = "hydro.local"
Mvs_File = "hydro.mvs"
Mvs_Mic_File = "hydro.mvs.mic"
Is_Local = False
Is_Mvs = False

#---------------------------------------------------------------------------------------------------
# Functions.
#---------------------------------------------------------------------------------------------------

'''
Print help.
'''
def Print_Help():
    print "Script for run hydro on mvs."
    print ""
    print "Usage:"
    print "  ./Run.py -h - print this text"
    print "  ./Run.py <mode> <grid> <dir> <pcs> - run hydro with grid <grid> in directory <dir>"
    print "                                       with given list of processes count (<pcs>)"
    print '    <mode> is one of "local" or "mvs"'

#---------------------------------------------------------------------------------------------------

'''
Run hydro with grid in given directory and given set of processes count.

Arguments:
  g - grid path
  d - directory name
  pcs - list of processes counts
'''
def Run(g, d, pcs):
    print('Run "%s" in directory "%s" with "%s" processes.' % (g, d, pcs))

    # Check files.
    assert(os.path.exists(g + ".pfg"))
    assert(os.path.exists(g + ".ibc"))

    # Prepare directory.
    if (not os.path.exists(d)):
        os.mkdir(d)
        print('Directory "%s" is created.' % d)

    # We need to run with each processes_counts element.
    pcs_list = pcs.split()
    for pc in pcs_list:
        Run_Single(g, d, int(pc))

#---------------------------------------------------------------------------------------------------

'''
Run hydro for given grid in created directory for given count of processes.

Arguments:
  g - grid path
  d - directory name
  pc - processes count
'''
def Run_Single(g, d, pc):
    print('Run "%s" in directory "%s" with %s processes.' % (g, d, pc))
    cwd = os.getcwd()

    # Create subdirectory d/run_00pc.
    subdir = "%s/run_%03d" % (d, pc)
    if (not os.path.exists(subdir)):
        os.mkdir(subdir)
        print('  Subdirectory "%s" is created.' % subdir)

    # Host file.
 #   if (Is_Mvs):
        Create_Hosts_File("%s/hosts" % subdir, pc)
        print('  Hosts file "%s/hosts" is created.' % subdir)

    # Commands.
    if (Is_Local):
        cmds = ["cd %s" % subdir,
                "cp %s/hydro.local ." % cwd,
                "mpirun -np %d ./hydro.local" % pc,
                "cd %s" % cwd]
    elif (Is_Mvs):
        cmds = ["cd %s" % subdir,
                "cp %s/hydro.mvs ." % cwd,
                "cp %s/hydro.mvs.mic ."  % cwd,
                "mpirun -np %d -machinefile hosts -mic -maxtime 30 hydro.mvs" % pc,
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

