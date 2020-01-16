#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# @Author: penghuailiang
# @Date  : 12/29/19
import struct
from vector import Vector3, CubeUV
import math
import random


def Cartesian2Spherical(p):
    """
    :type p: Vector3
    """
    s = Vector3.zero()
    s.radius = p.magnitude()
    s.theta = math.acos(p.y / s.radius)
    s.phi = math.atan2(p.z, p.x)
    return s


def Spherical2Cartesian(s):
    """
    :type s: Vector3
    """
    p = Vector3.zero()
    p.x = s.radius * math.sin(s.theta) * math.cos(s.phi)
    p.y = s.radius * math.cos(s.theta)
    p.z = s.radius * math.sin(s.theta) * math.sin(s.phi)
    return p


def CubeUV2XYZ(c):
    """
    :type c: CubeUV
    :return: Position
    """
    u = c.u * 2.0 - 1.0
    v = c.v * 2.0 - 1.0
    if c.index == 0:
        return Vector3(1, v, u)
    elif c.index == 1:
        return Vector3(-1, v, u)
    elif c.index == 2:
        return Vector3(u, 1, -v)
    elif c.index == 3:
        return Vector3(u, -1, v)
    elif c.index == 4:
        return Vector3(u, v, 1)
    elif c.index == 5:
        return Vector3(-u, v, -1)
    else:
        return Vector3.zero()


def XYZ2CubeUV(p):
    """
    :type p: Vector3
    :return type: CubeUV
    """
    ax = abs(p.x)
    ay = abs(p.y)
    az = abs(p.z)
    if ax >= ay and ax >= az:
        c = CubeUV(-p.z / p.x, p.y / ax, 0 if p.x >= 0 else 1)
    elif ay >= az:
        c = CubeUV(p.x / ay, -p.z / p.y, 2 if p.y >= 0 else 3)
    else:
        c = CubeUV(p.x / p.z, p.y / az, 4 if p.z > 0 else 5)
    c.u = c.u * 0.5 + 0.5
    c.v = c.v * 0.5 + 0.5
    return c


def NormalRandom(mu=0, sigma=1):
    """
    生成服从正态分布的随机数
    :param mu: 均值
    :param sigma: 标准差
    """
    return random.normalvariate(mu, sigma=sigma)


def Write(name, content):
    f = open("output/{0}.txt".format(name), 'w')
    f.write(content)
    f.close()


def writef(f, v):
    p = struct.pack("f", v)
    f.write(p)


def WriteRecord(name, record):
    f = open("output/{0}.txt".format(name), 'wb')
    for it in record:
        writef(f, it)
    f.close()
