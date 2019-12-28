#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# @Author: penghuailiang
# @Date  : 12/28/19

import numpy as np

arr = [x for x in range(10)]
arr[1] = 2
print(arr)

a = np.zeros((16, 16, 3))
print(a[0][0])
v = a[0][0]
print(type(v))
print(v[1])
