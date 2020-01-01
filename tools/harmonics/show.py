#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# @Author: penghuailiang
# @Date  : 1/1/20

"""
此脚本使用scipy.special绘制球谐函数
"""

import numpy as np
import matplotlib.pyplot as plt
from scipy.special import sph_harm
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm

theta_1d = np.linspace(0, np.pi, 181)  # colatitude
phi_1d = np.linspace(0, 2 * np.pi, 361)  # longitude

theta_2d, phi_2d = np.meshgrid(theta_1d, phi_1d)
xyz_2d = np.array([np.sin(theta_2d) * np.sin(phi_2d), np.sin(theta_2d) * np.cos(phi_2d), np.cos(theta_2d)])

colormap = cm.ScalarMappable(cmap=plt.get_cmap("cool"))
colormap.set_clim(-0.45, 0.45)
limit = 0.5


def show_Y_lm(l, m):
    print('Y_{:d}_{:d}'.format(l, m))
    plt.figure(dpi=100)
    ax = plt.gca(projection='3d')
    Y_lm = sph_harm(m, l, phi_2d, theta_2d)
    r = np.abs(Y_lm.real) * xyz_2d  # need to distort the radius by some function
    ax.plot_surface(r[0], r[1], r[2], facecolors=colormap.to_rgba(Y_lm.real))
    plt.title('$Y^{:d}_{:d}$'.format(m, l))
    ax.set_xlim(-limit, limit)
    ax.set_ylim(-limit, limit)
    ax.set_zlim(-limit, limit)
    ax.set_axis_off()
    plt.show()


show_Y_lm(2, 0)
show_Y_lm(3, 3)
show_Y_lm(4, 2)
