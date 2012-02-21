#!/usr/bin/env python

import select
import socket
import time

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind(('', 50000))
sock.listen(10)

reads = [sock]
writes = []
data = {}
adrs = {}

try:
    while 1:
        input, output, e = select.select(reads, writes, [])
        for x in input:
            if x is sock:
                newSock, addr = sock.accept()
                print "connect from ", addr
                reads.append(newSock)
                adrs[newSock] = addr
            else:
                newData = x.recv(512)
                if newData:
                    print "%s from %s" % (str(newData), adrs[x])
                    time.sleep(5)
                    data[x] = data.get(x, '') + newData
                    if x not in writes:
                        writes.append(x)
                else:
                    print "disconnect from ", adrs[x]
                    del adrs[x]
                    writes.remove(x)
                    x.close()

        for x in output:
            tosend = data.get(x)
            if tosend:
                n = x.send(tosend)
                print "%s to %s" % (str(tosend), adrs[x])
                tosend = tosend[n:]
            if tosend:
                print "%d bytes renain for %s " % (len(tosend), adrs[x])
                data[x] = tosend
            else:
                del data[x]
                writes.remove(x)

finally:
    sock.close()
