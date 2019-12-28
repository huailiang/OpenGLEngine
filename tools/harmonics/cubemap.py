#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# @Author: penghuailiang
# @Date  : 12/28/19

from vector import Vertex
from util import *
import cv2
import numpy as np


class Cubemap:

    def __init__(self, files):
        """
        :type files: list
        :param files: 文件路径 len=6
        """
        self.files = files
        self._open_files()

    def __str__(self):
        return "width:{0}, height:{1}".format(self.width, self.height)

    def _open_files(self):
        count = len(self.files)
        assert count == 6
        print(count)
        self.images = []
        for i in range(count):
            img = cv2.imread(self.files[i])
            self.images.append(img)
            self.width = img.shape[0]
            self.height = img.shape[1]

    def getVertices(self):
        w = self.width
        h = self.height
        vertices = [0 for x in range(w * h * 6)]
        for k in range(6):
            img = self.images[k]
            for j in range(w):
                for i in range(h):
                    c = img[i, j]
                    print(c)
                    idx = k * w * h + i * w + j
                    u = j / (img.w - 1)
                    v = 1 - i / (img.h - 1)
                    p = CubeUV2XYZ(CubeUV(u, v, k))
                    vertices[idx] = Vertex(p, Vector3(c[2], c[1], c[0]))
        return vertices

    def GenExpandImage(self, maxsize):
        w = min(self.width, maxsize)
        h = min(self.height, maxsize)
        xarr = [2 * w, 0, w, w, w, 3 * w]
        yarr = [h, h, 0, 2 * h, h, h]
        expandimg = np.zeros((w * 3, h * 4))
        for i in range(6):
            img = None
            cv2.resize(self.images[i], (w, h), img)
            cv2.copyTo(img, i, expandimg)
        return expandimg

    def RandomSample(self, n):
        samples = []
        for i in range(n):
            x = NormalRandom()
            y = NormalRandom()
            z = NormalRandom()
            p = Vector3(x, y, z)
            c = self.sample(p)
            v = Vertex(p, c)
            samples.append(v)
        return samples

    def sample(self, pos):
        """
        采样空间某一点
        :param pos: Vector3
        :return: vector<Vertex>
        """
        cubeuv = XYZ2CubeUV(pos)
        j = int(cubeuv.u * (self.width - 1))
        i = int((1. - cubeuv.v) * (self.height - 1))
        c = self.images[cubeuv.index][i, j]
        return Vector3(c[2], c[1], c[0])

    def sampleT(self, theta, phi):
        s = Vector3(1, theta, phi)
        p = Spherical2Cartesian(s)
        return p
