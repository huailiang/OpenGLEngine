#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# @Author: penghuailiang
# @Date  : 12/29/19

from setuptools import setup, find_packages
from os import path

here = path.abspath(path.dirname(__file__))

print(here)

setup(name="neural",
      version="0.0.1",
      description="harmonic for opengl engine",
      url="https://github.com/huailiang/OpenGLEngine",
      author_email="peng_huailiang@qq.com",
      classifiers=[
          "Intended Audience :: Developers",
          "Programming Language :: Python :: 3.5",
      ],
      packages=find_packages(),
      zip_safe=False,
      install_requires=["tqdm", "numpy>=1.13.3,<2.0", 'cv2', 'matplotlib', 'scipy'],
      python_requires=">=3.5"
      )
