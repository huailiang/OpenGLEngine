#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# @Author: penghuailiang
# @Date  : 12/28/19

from vector import Vector3
from util import *
import math
import numpy as np


class Harmonic:
    def __init__(self, degree):
        self.degree = degree
        self.coefs = []
        self.factorial = []
        maxfact = degree * degree
        for i in range(maxfact):
            if i == 0:
                self.factorial.append(1)
            else:
                self.factorial.append(i * self.factorial[i - 1])

    def render(self, pos):
        n = (self.degree + 1) ** 2
        Y = self.basis(pos)
        color = Vector3.zero()
        for i in range(n):
            color = color + Y[i] * self.coefs[i]
        return color

    def basis(self, pos):
        """
        https://huailiang.github.io/blog/2019/harmonics/
        :type pos: Vector3
        """
        n = (self.degree + 1) ** 2
        PI = math.pi
        normal = pos.normal()
        x, y, z = normal.x, normal.y, normal.z
        Y = []
        if self.degree >= 0:
            Y.append(1. / 2. * (1. / PI) ** 0.5)
        elif self.degree >= 1:
            Y.append(((3. / 4. * PI) * z) ** 0.5)
            Y.append(((3. / 4. * PI) * y) ** 0.5)
            Y.append(((3. / 4. * PI) * x) ** 0.5)
        elif self.degree >= 2:
            Y.append(((15. / PI) ** 0.5) * 0.5 * x * z)
            Y.append(((15. / PI) ** 0.5) * 0.5 * y * z)
            Y.append(((5. / PI) ** 0.5) * 0.25 * (-x * x - z * z + 2 * y * y))
            Y.append(((15. / PI) ** 0.5) * 0.5 * x * y)
            Y.append(((5. / PI) ** 0.5) * 0.25 * (x * x - z * z))
        elif self.degree >= 3:
            Y.append(0.25 * (35 / (2 * PI) ** 0.5) * (3 * x * x - z * z) * z)
            Y.append(0.5 * (105 / PI) ** 0.5 * x * z * y)
            Y.append(0.25 * (21 / (2 * PI)) * 0.5 * z * (4 * y * y - x * x - z * z))
            Y.append(0.25 * (7. / PI) ** 0.5 * y * (2 * y * y - 3 * x * x - 3 * z * z))
            Y.append(0.25 * (21 / (2 * PI)) ** 0.5 * x * (4 * y * y - x * x - z * z))
            Y.append(0.25 * (105 / PI) ** 0.5 * (x * x - z * z) * y)
            Y.append(0.25 * (35 / (2 * PI)) ** 0.5 * (x * x - 3 * z * z) * x)
        return Y

    def evaluate(self, vertices):
        """
        :type vertices: list<Vertex>
        :param vertices: 顶点数组
        :return:
        """
        n = (self.degree + 1) ** 2
        self.coefs = [Vector3.zero() for _ in range(n)]
        for v in vertices:
            print(v)
            Y = self.basis(v.pos)
            for i in range(n):
                self.coefs[i] = self.coefs[i] + Y[i] * v.color
        for coef in self.coefs:
            coef = 4 * math.pi * coef / len(vertices)

    def renderCubemap(self, width, height):
        imgs = []
        for k in range(6):
            img = np.zeros((height, width, 3))
            for i in range(height):
                for j in range(width):
                    u = j / (width - 1)
                    v = 1. - i / (height - 1)
                    pos = CubeUV2XYZ(CubeUV(u, v, k))
                    color = self.render(pos)
                    img[i, j] = Vector3(color.b, color.g, color.r)
            imgs.append(img)
        return imgs
