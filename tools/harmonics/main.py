#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# @Author: penghuailiang
# @Date  : 12/28/19

import cv2
from cubemap import Cubemap
from harmonic import Harmonic
from util import Write
import os
import time


if not os.path.exists("output"):
    os.mkdir("output")

print("reading cubemap ...")
path = "../data/CNTower"
cube = Cubemap(path)
img = cube.GenExpandImage(256)
cv2.imwrite("output/cube.jpg", img)

print("sampling ...")
degree = 3
samplenum = 100000
harmonics = Harmonic(degree)

start = time.clock()
pos, col = cube.NumpySample(samplenum)
txt = harmonics.numpyEval(pos, col)
Write("coef", txt)
print("numpy exec time:{}".format(time.clock() - start))

start = time.clock()
verticies = cube.RandomSample(samplenum)
harmonics.evaluate(verticies)
Write("coef2", str(harmonics))
print("\ncpu exec time:{}".format(time.clock() - start))
