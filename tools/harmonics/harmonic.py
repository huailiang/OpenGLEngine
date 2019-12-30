#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# @Author: penghuailiang
# @Date  : 12/28/19

from vector import Vector3
from util import *
import math
import numpy as np
from tqdm import tqdm


class Harmonic:
    def __init__(self, degree=3):
        self.degree = degree
        self.coefs = []
        self.factorial = []
        self.Y = [0 for _ in range(16)]
        maxfact = degree * degree
        for i in range(maxfact):
            if i == 0:
                self.factorial.append(1)
            else:
                self.factorial.append(i * self.factorial[i - 1])

    def render(self, pos):
        n = (self.degree + 1) ** 2
        self.basis(pos)
        color = Vector3.zero()
        for i in range(n):
            color = color + self.Y[i] * self.coefs[i]
        return color

    def basis(self, pos):
        """
        https://huailiang.github.io/blog/2019/harmonics/
        :type pos: Vector3
        """
        PI = math.pi
        normal = pos.normal()
        x, y, z = normal.x, normal.y, normal.z
        if self.degree >= 0:
            self.Y[0] = 0.5 * (1 / PI) ** 0.5
        if self.degree >= 1:
            self.Y[1] = (3 / (4 * PI)) ** 0.5 * z
            self.Y[2] = (3 / (4 * PI)) ** 0.5 * y
            self.Y[3] = (3 / (4 * PI)) ** 0.5 * x
        if self.degree >= 2:
            self.Y[4] = ((15 / PI) ** 0.5) * 0.5 * x * z
            self.Y[5] = ((15 / PI) ** 0.5) * 0.5 * y * z
            self.Y[6] = ((15 / PI) ** 0.5) * 0.25 * (-x * x - z * z + 2 * y * y)
            self.Y[7] = ((15 / PI) ** 0.5) * 0.5 * x * y
            self.Y[8] = ((15 / PI) ** 0.5) * 0.25 * (x * x - z * z)
        if self.degree >= 3:
            self.Y[9] = 0.25 * (35 / (2 * PI) ** 0.5) * (3 * x * x - z * z) * z
            self.Y[10] = 0.5 * (105 / PI) ** 0.5 * x * z * y
            self.Y[11] = 0.25 * (21 / (2 * PI)) * 0.5 * z * (4 * y * y - x * x - z * z)
            self.Y[12] = 0.25 * (7 / PI) ** 0.5 * y * (2 * y * y - 3 * x * x - 3 * z * z)
            self.Y[13] = 0.25 * (21 / (2 * PI)) ** 0.5 * x * (4 * y * y - x * x - z * z)
            self.Y[14] = 0.25 * (105 / PI) ** 0.5 * (x * x - z * z) * y
            self.Y[15] = 0.25 * (35 / (2 * PI)) ** 0.5 * (x * x - 3 * z * z) * x

    def evaluate(self, vertices):
        """
        :type vertices: list<Vertex>
        :param vertices: 顶点数组
        """
        n = (self.degree + 1) ** 2
        self.coefs = [Vector3.zero() for _ in range(n)]
        count = len(vertices)
        m_progress = tqdm(range(count))
        for idx in m_progress:
            v = vertices[idx]
            self.basis(v.pos)
            for i in range(n):
                # self.coefs[i].x += v.color.x * self.Y[i]
                # self.coefs[i].y += v.color.y * self.Y[i]
                # self.coefs[i].z += v.color.z * self.Y[i]
                self.coefs[i] = self.coefs[i] + v.color * self.Y[i]
        for i in range(n):
            self.coefs[i] = self.coefs[i] / len(vertices) * 4 * math.pi

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
