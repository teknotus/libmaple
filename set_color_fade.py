#!/usr/bin/env python

import liblo
import sys
import colorsys
import math
import time

step_size = 0.001
address   = '255.255.255.255'
port      = 12344


def frange5(limit1, limit2 = None, increment = 1.):
  """
  Range function that accepts floats (and integers).

  Usage:
  frange(-2, 2, 0.1)
  frange(10)
  frange(10, increment = 0.5)

  The returned value is an iterator.  Use list(frange) for a list.
  """

  if limit2 is None:
    limit2, limit1 = limit1, 0.
  else:
    limit1 = float(limit1)

  count = int(math.ceil(limit2 - limit1)/increment)
  return (limit1 + n*increment for n in range(count))

def set_light(r, g, b):
    liblo.send(target, "/light/color/set",
               ('f', r),
               ('f', g),
               ('f', b))
try:
    target = liblo.Address(address, port)
except liblo.AddressError, err:
    print str(err)
    sys.exit()

while True:
    for hue in frange5(0, 360.0, step_size):
        [r, g, b] = colorsys.hsv_to_rgb(hue, 1, 0.01)
        print [r, g, b]
        set_light(float(r),
                  float(g),
                  float(b))
        time.sleep(0.01)
