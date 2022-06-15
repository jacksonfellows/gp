#!/usr/bin/env python3
import matplotlib.pyplot as plt
import sys
from math import *

xs = []
ys = []

for line in sys.stdin.readlines():
    x,y = line.split()
    xs.append(float(x))
    ys.append(float(y))

plt.plot(xs, ys, '--')
f = eval('lambda x:' + sys.argv[1])
plt.plot(xs, [f(x) for x in xs], 'r')
plt.show()
