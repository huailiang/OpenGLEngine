#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# @Author: penghuailiang
# @Date  : 12/28/19

import cv2
import numpy as np
from cubemap import Cubemap
from harmonic import Harmonic
from vector import *
from util import *

path1 = "../data/CNTower/posx.jpg"
# src1 = cv2.imread(path1, 1)
# w_, d_ = src1.shape[0], src1.shape[2]
# print(w_, d_)
# w = 256
# h = 256
# face = cv2.resize(src1, (256, 256))
# print(face.shape)
# expand = np.zeros((3 * h, 4 * w, 3))
# expand[10:266, 256:512] = face
# cv2.imwrite("test.jpg", expand)

path = "../data/CNTower"
cube = Cubemap(path)
img = cube.GenExpandImage(256)
cv2.imwrite("cube.jpg", img)

degree = 4
samplenum = 1000000
harmonics = Harmonic(degree)
verticies = cube.RandomSample(samplenum)
harmonics.Evaluate(verticies)
coefs = harmonics.getCoefficients()
txt = CoefficientsString(coefs)
print(txt)
