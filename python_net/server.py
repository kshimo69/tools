#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Copyright (C) 2011 Shimomura Kimihiko <kshimo69@gmail.com>

import socket

host = socket.gethostbyname('localhost')
port = 50000

soc_waiting = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
soc_waiting.bind((host, port))
soc_waiting.listen(1)
print "server running at %d\n" % port

(soc, client_addr) = soc_waiting.accept()
soc_waiting.close()

print "Start!"

msg = ""

while True:
    msg = raw_input()
    soc.send(msg)
    msg = soc.recv(512)
    print msg
    if msg == 'quit':
        break

soc.close()
