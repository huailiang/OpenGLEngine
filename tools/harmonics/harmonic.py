#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# @Author: penghuailiang
# @Date  : 12/28/19

from util import *
import math
import numpy as np
from tqdm import tqdm


class Harmonic:
    def __init__(self, degree=3):
        self.degree = degree
        n = (degree + 1) ** 2
        self.coefs = [Vector3.zero() for _ in range(n)]

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
        :type pos: Vector3 normalized
        """
        PI = math.pi
        Y = [0 for _ in range(16)]
        x, y, z = pos.x, pos.y, pos.z
        if self.degree >= 0:
            Y[0] = 0.5 * (1 / PI) ** 0.5
        if self.degree >= 1:
            Y[1] = (3 / (4 * PI)) ** 0.5 * z
            Y[2] = (3 / (4 * PI)) ** 0.5 * y
            Y[3] = (3 / (4 * PI)) ** 0.5 * x
        if self.degree >= 2:
            Y[4] = ((15 / PI) ** 0.5) * 0.5 * x * z
            Y[5] = ((15 / PI) ** 0.5) * 0.5 * y * z
            Y[6] = ((5 / PI) ** 0.5) * 0.25 * (-x * x - z * z + 2 * y * y)
            Y[7] = ((15 / PI) ** 0.5) * 0.5 * x * y
            Y[8] = ((15 / PI) ** 0.5) * 0.25 * (x * x - z * z)
        if self.degree >= 3:
            Y[9] = 0.25 * (35 / (2 * PI)) ** 0.5 * (3 * x * x - z * z) * z
            Y[10] = 0.5 * (105 / PI) ** 0.5 * x * z * y
            Y[11] = 0.25 * (21 / (2 * PI)) ** 0.5 * z * (4 * y * y - x * x - z * z)
            Y[12] = 0.25 * (7 / PI) ** 0.5 * y * (2 * y * y - 3 * x * x - 3 * z * z)
            Y[13] = 0.25 * (21 / (2 * PI)) ** 0.5 * x * (4 * y * y - x * x - z * z)
            Y[14] = 0.25 * (105 / PI) ** 0.5 * (x * x - z * z) * y
            Y[15] = 0.25 * (35 / (2 * PI)) ** 0.5 * (x * x - 3 * z * z) * x
        return Y

    @staticmethod
    def numpyBasis(pos):
        """
        :param pos: ndarray (number, 3)
        :return (number, 16)
        """
        PI = np.pi
        shape = pos.shape
        num = shape[0]
        rs = np.zeros((num, 16))
        x = pos[0:num, 0:1]
        y = pos[0:num, 1:2]
        z = pos[0:num, 2:3]
        rs[0:num, 0:1] = 0.5 * (1 / PI) ** 0.5
        rs[0:num, 1:2] = (3 / (4 * PI)) ** 0.5 * z
        rs[0:num, 2:3] = (3 / (4 * PI)) ** 0.5 * y
        rs[0:num, 3:4] = (3 / (4 * PI)) ** 0.5 * x
        rs[0:num, 4:5] = ((15 / PI) ** 0.5) * 0.5 * x * z
        rs[0:num, 5:6] = ((15 / PI) ** 0.5) * 0.5 * y * z
        rs[0:num, 6:7] = ((15 / PI) ** 0.5) * 0.25 * (-x * x - z * z + 2 * y * y)
        rs[0:num, 7:8] = ((15 / PI) ** 0.5) * 0.5 * x * y
        rs[0:num, 8:9] = ((15 / PI) ** 0.5) * 0.25 * (x * x - z * z)
        rs[0:num, 9:10] = 0.25 * (35 / (2 * PI) ** 0.5) * (3 * x * x - z * z) * z
        rs[0:num, 10:11] = 0.5 * (105 / PI) ** 0.5 * x * z * y
        rs[0:num, 11:12] = 0.25 * (21 / (2 * PI)) * 0.5 * z * (4 * y * y - x * x - z * z)
        rs[0:num, 12:13] = 0.25 * (7 / PI) ** 0.5 * y * (2 * y * y - 3 * x * x - 3 * z * z)
        rs[0:num, 13:14] = 0.25 * (21 / (2 * PI)) ** 0.5 * x * (4 * y * y - x * x - z * z)
        rs[0:num, 14:15] = 0.25 * (105 / PI) ** 0.5 * (x * x - z * z) * y
        rs[0:num, 15:16] = 0.25 * (35 / (2 * PI)) ** 0.5 * (x * x - 3 * z * z) * x
        return rs

    def evaluate(self, vertices):
        """
        :type vertices: list<Vertex>
        :param vertices: 顶点数组
        """
        n = (self.degree + 1) ** 2
        count = len(vertices)
        m_progress = tqdm(range(count))
        for idx in m_progress:
            v = vertices[idx]
            Y = self.basis(v.pos)
            for i in range(n):
                self.coefs[i] = self.coefs[i] + v.color * Y[i]
        for i in range(n):
            self.coefs[i] = self.coefs[i] / count * 4 * math.pi

    def numpyEval(self, pos, colors):
        """
        :param pos: vertex pos - shape:(num, 3)
        :param colors: vertex color - shape(num, 3)
        :return: coef str
        """
        n = (self.degree + 1) ** 2
        Y = self.numpyBasis(pos)  # (num, 16)
        num = pos.shape[0]
        str_ = ""
        for i in range(n):
            t = colors * Y[0:num, i:i + 1]
            v = t.sum(axis=0) / num * 4 * np.pi
            str_ += "{0:.5}\t{1:.5}\t{2:.5}\n".format(v[0], v[1], v[2])
        return str_

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

    def __str__(self):
        """
        harmonic coef
        """
        st = ""
        for c in self.coefs:
            st += "{0:.5}\t{1:.5}\t{2:.5}\n".format(c.r, c.g, c.b)
        return st
