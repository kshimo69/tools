#!/usr/bin/env python

# for kernel 2.6.18-164.el5

import sys

pid = ''

try:
    pid = sys.argv[1]
except IndexError:
    print "usage: %s PID" % sys.argv[0]
    sys.exit(1)

f = open('/proc/%s/stat' % pid)
data = f.read()
f.close

def _print(param, value):
    print "%-16s: %s" % (param, value)

params = ['pid', 'tcomm', 'state', 'ppid', 'pgid', 'sid', 'tty_nr', 'tty_pgrp',
          'flags', 'min_flt', 'cmin_flt', 'maj_flt', 'cmaj_flt', 'utime',
          'stime', 'cutime', 'cstime', 'priority', 'nice', 'num_threads', 0,
          'start_time', 'vsize', 'mm_rss', 'rsslim', 'start_code', 'end_code',
          'start_stack', 'esp', 'eip', 'pending', 'blocked', 'sigign',
          'sigcatch', 'wchan', 0, 0, 'exit_signal', 'cpu', 'rt_priority',
          'policy', 'blkio_ticks']
[_print(params[i], v) for i, v in enumerate(data.split())]