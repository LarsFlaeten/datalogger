#!/usr/bin/env python
import sys
import os
import pickle
import BlackboxParser

if len(sys.argv) != 2:
    print 'Missing blackbox file!'
    sys.exit(0)

logfile = sys.argv[1]
print logfile

blackbox = BlackboxParser.parse_blackbox(logfile)

with open("{}.pickle".format(logfile),'wb') as f:
    pickle.dump(blackbox, f)

