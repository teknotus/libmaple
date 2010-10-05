#!/usr/bin/env python

import liblo
import sys
import colorsys
import math
import time

addresses  = ('192.168.1.2',
              '192.168.1.3',
              '192.168.1.4')
port       = 12344

step_size  = 0.001
delay      = 0.01
saturation = 1.0
value      = 1.0


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

def set_light(address, r, g, b):
    try:
        target = liblo.Address(address, port)
    except liblo.AddressError, err:
        print str(err)
        sys.exit()

    liblo.send(target, "/light/color/set",
               ('f', r),
               ('f', g),
               ('f', b))
while True:
    for hue in frange5(0, 360.0, step_size):
        [r, g, b] = colorsys.hsv_to_rgb(hue, saturation, value)
#        print [r, g, b]
        for address in addresses:
            set_light(address, float(r),
                               float(g),
                               float(b))
        time.sleep(delay)
