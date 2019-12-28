#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# @Author: penghuailiang
# @Date  : 12/28/19


def blue():
    return Vector3(0, 0, 1)


class Vector3:

    def __init__(self, x=0, y=0, z=0):
        self.x = x
        self.y = y
        self.z = z

    def __add__(self, other):
        v = Vector3.one()
        v.x = self.x + other.x
        v.y = self.y + other.y
        v.z = self.z + other.z
        return v

    def __sub__(self, other):
        v = Vector3.one()
        v.x = self.x - other.x
        v.y = self.y - other.y
        v.z = self.z - other.z
        return v

    def __mul__(self, other):
        v = Vector3.one()
        v.x = self.x * other.x
        v.y = self.y * other.y
        v.z = self.z * other.z
        return v

    def __truediv__(self, other):
        v = Vector3.one()
        v.x = self.x / other.x
        v.y = self.y / other.y
        v.z = self.z / other.z
        return v

    def __str__(self):
        return "vector x:{0}, y:{1}, z:{2}".format(self.x, self.y, self.z)

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

    def normal(self):
        mag = self.magnitude()
        return Vector3(self.x / mag, self.y / mag, self.z / mag)


print(Vector3.one() / Vector3(1, 2, 4))
vec = Vector3(3, 2, 1)
print(vec.r)
vec.b = 4
print(vec)
print(vec.magnitude())
print(vec.normal())
print(vec * 2)
