#!/usr/bin/env python
# coding: utf-8

"""
此工具用来转换图片的分辨率 使分辨率是2的次幂 
用法：
python solution.py /path/xxx.png
"""

import sys
import os
import shutil

try:
    from PIL import Image
except:
    print ('\033[31m' + '缺少Image模块，正在安装Image模块，请等待...' + '\033[0m')
    success = os.system('python -m pip install Image')
    if success == 0:
      print('\033[7;32m' + 'Image模块安装成功.' + '\033[0m')
      from PIL import Image
    else:
      print ('\033[31m' + 'Image安装失败，请手动在终端执行：\'python -m pip install Image\'重新安装.' + '\033[0m')
      quit()


if len(sys.argv) <= 1:
    print ('\033[31m' + '请输入图片路径,eg: python solution.py /path/xxx.png' + '\033[0m')
    quit()

ImageName = sys.argv[1]
originImg = ''
try:
    originImg = Image.open(ImageName)
    
except:
    print ('\033[31m' + '\'' + ImageName + '\'' + '，该文件不是图片文件，请检查文件路径.' + '\033[0m')
    quit()



_dir,_file=os.path.split(ImageName)
_par,_ = os.path.split(_dir)
_dir = os.path.join(_par,"temp")
targetName=os.path.join(_dir,_file)
print(_file)

if not os.path.exists(_dir):
      os.makedirs(_dir)

def resize(target):
  img = originImg.resize((target, target), Image.ANTIALIAS)
  img.save(targetName)


# 这里是支持转换的分辨率
solutions = [ 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048 ]

i  = 0 
width = originImg.width


for x in solutions:
  if x == width:
    shutil.copyfile(ImageName, targetName)
    break
  if x < width and width < solutions[i+1]:
    print("invalid image resolution: "+ ImageName)
    resize(solutions[i+1])
    break
  if x > 2048:
    print("image solution too large, not support on iOS")
  i = i+1

