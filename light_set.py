#!/usr/bin/env python

import liblo
import sys

port = 12344

try:
    target = liblo.Address('192.168.0.8', port)
except liblo.AddressError, err:
    print str(err)
    sys.exit()

def set_light(r, g, b):
    liblo.send(target, "/light/color/set",
               ('f', r),
               ('f', g),
               ('f', b))


set_light(float(sys.argv[1]),
          float(sys.argv[2]),
          float(sys.argv[3]))
