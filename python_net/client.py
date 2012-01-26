#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Copyright (C) 2011 Shimomura Kimihiko <kshimo69@gmail.com>

import socket

host = socket.gethostbyname('localhost')
port = 50000

soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
soc.connect((host, port))

print "Wait\n"

while True:
    msg = soc.recv(512)
    print msg
    msg = raw_input()
    soc.send(msg)
    if msg == 'quit':
        break

soc.close()
