#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# @Author: penghuailiang
# @Date  : 12/28/19


class Vector3:

    def __init__(self, x=0, y=0, z=0):
        self.x = x
        self.y = y
        self.z = z

    def __add__(self, other):
        return Vector3(self.x + other.x, self.y + other.y, self.z + other.z)

    def __sub__(self, other):
        return Vector3(self.x - other.x, self.y - other.y, self.z - other.z)

    def __mul__(self, other):
        if type(other) == type(self):
            return Vector3(self.x * other.x, self.y * other.y, self.z * other.z)
        else:
            return Vector3(self.x * other, self.y * other, self.z * other)

    def __truediv__(self, other):
        if type(other) == type(self):
            return Vector3(self.x / other.x, self.y / other.y, self.z / other.z)
        else:
            return Vector3(self.x / other, self.y / other, self.z / other)

    def __mod__(self, other):
        if type(other) == type(self):
            return Vector3(self.x % other.x, self.y % other.y, self.z % other.z)
        else:
            return Vector3(self.x % other, self.y % other, self.z % other)

    def __pow__(self, power, modulo=None):
        if type(power) == type(self):
            return Vector3(self.x ** power.x, self.y ** power.y, self.z ** power.z)
        else:
            return Vector3(self.x ** power, self.y ** power, self.z ** power)

    def __str__(self):
        return "({0}, {1}, {2})".format(self.x, self.y, self.z)

    @property
    def r(self):
        return self.x

    @r.setter
    def r(self, v):
        self.x = v

    @property
    def g(self):
        return self.y

    @g.setter
    def g(self, v):
        self.y = v

    @property
    def b(self):
        return self.z

    @b.setter
    def b(self, v):
        self.z = v

    @property
    def radius(self):
        return self.x

    @radius.setter
    def radius(self, v):
        self.x = v

    @property
    def theta(self):
        return self.y

    @theta.setter
    def theta(self, v):
        self.y = v

    @property
    def phi(self):
        return self.z

    @phi.setter
    def phi(self, v):
        self.z = v

    @staticmethod
    def one():
        return Vector3(1, 1, 1)

    @staticmethod
    def left():
        return Vector3(-1, 0, 0)

    @staticmethod
    def right():
        return Vector3(1, 0, 0)

    @staticmethod
    def up():
        return Vector3(0, 1, 0)

    @staticmethod
    def down():
        return Vector3(0, -1, 0)

    @staticmethod
    def forward():
        return Vector3(0, 0, 1)

    @staticmethod
    def back():
        return Vector3(0, 0, -1)

    @staticmethod
    def zero():
        return Vector3()

    @staticmethod
    def red():
        return Vector3(1, 0, 0)

    @staticmethod
    def green():
        return Vector3(0, 1, 0)

    @staticmethod
    def black():
        return Vector3.zero()

    @staticmethod
    def white():
        return Vector3.zero()

    def magnitude(self):
        return (self.x ** 2 + self.y ** 2 + self.z ** 2) ** 0.5

    def distance(self):
        return self.magnitude()

    def normal(self):
        mag = self.magnitude()
        return Vector3(self.x / mag, self.y / mag, self.z / mag)

    def toarray(self):
        return self.x, self.y, self.z


class Vertex:
    def __init__(self, pos=Vector3.zero(), color=Vector3.white()):
        """
        :param pos: 顶点位置
        :param color: 顶点色
        """
        self.pos = pos
        self.color = color

    def __str__(self):
        return "pos:{0} color:{1}".format(self.pos, self.color)


class CubeUV:
    def __init__(self, u=0, v=0, index=0):
        """
        :param u: u
        :param v: v
        :param index: cubemap六个面
            0 +x
            1 -x
            2 +y
            3 -y
            4 +z
            5 -z
        """
        self.u = u
        self.v = v
        self.index = index

    def __str__(self):
        return "u:{0} v:{1} index:{2}".format(self.u, self.v, self.index)
