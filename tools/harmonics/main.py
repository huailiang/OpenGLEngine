#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# @Author: penghuailiang
# @Date  : 12/28/19

import cv2
from cubemap import Cubemap
from harmonic import Harmonic
from util import *
import os

if not os.path.exists("output"):
    os.mkdir("output")

print("reading cubemap ...")
path = "../data/CNTower"
cube = Cubemap(path)
img = cube.GenExpandImage(256)
cv2.imwrite("output/cube.jpg", img)

print("sampleing ...")
degree = 3
samplenum = 100000
harmonics = Harmonic(degree)
verticies = cube.RandomSample(samplenum)
# colors = cube.NumpySample(samplenum)
# print(colors.shape)
harmonics.evaluate(verticies)
txt = CoefficientsString(harmonics.coefs)
f = open("output/coef.txt", 'w')
f.write(txt)
f.close()
