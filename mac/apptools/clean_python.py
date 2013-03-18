#!/usr/bin/python

import sys, os

def usage():
  print """
Usage:
clean_python.py <DIR>

Will remove all .py if a .pyc file exists in or below <DIR>.
Will remove all .pyo if a .pyc file exists in or below <DIR>.
"""

if len(sys.argv) < 2:
  usage()
  sys.exit(0)

DIR = sys.argv[1]

if not os.exists(DIR):
  print "ERROR: <DIR> \""+DIR+"\" not found."
  usage()
  sys.exit(0)

singletonfiles = set()

def clean_dir():
  """
  Cleans the current of .py and .pyo iff a .pyc exists.
  Recursively calls itself on all sub-directories discovered in the process.
  """
  print os.getcwd()
  files = set(os.listdir('.'))
  for file in files:
    # Does it end in .pyc?
    if file.endswith('.pyc'):
      # If so, do we have a .py or .pyo?
      if os.path.exists(file[:-1]):
        os.remove(file[:-1])
        singletonfiles.add(file)
      if os.path.exists(file[:-1]+'o'):
        os.remove(file[:-1]+'o')
        singletonfiles.add(file)
    # Is it a directory?
    if os.isdir(file):
      os.chdir(file)
      clean_dir()
      os.chdir('..')

currentdir = os.getcwd()
os.chdir(DIR)
clean_dir()
os.chdir(currentdir)

print "Found "+str(len(singletonfiles))+" files with .py or .pyos."
