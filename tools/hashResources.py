#!/usr/bin/env python
# coding: utf-8

import sys
import os
import shutil
import struct
import ctypes

"""
此工具根据路径生成文件对应路径的hash值为名的文件

所有的文件都是用hash，打到ios mainbundle中，

1. 可以避免不同路径下同名文件
2. 避免直接暴露资源名，加密

make so:
  gcc -o libc.so -shared -fPIC libc.c

"""


ll = ctypes.cdll.LoadLibrary

lib = ll("./libc.so")

path = "resource/obj/sui/export hello world.png"



def xhash(path):
  rt = lib.hash(path,len(path))
  return rt & 0xffffffff


def copy_res(src,dst):
  if not os.path.isfile(src):
        print "%s not exist!"%(src)
  else:
    # print(src,dst)
    fpath,fname=os.path.split(dst)
    if not os.path.exists(fpath):
      os.makedirs(fpath)
    shutil.copyfile(src,dst)



def fetch_files(dir):
  for root, dirs, files in os.walk(dir):
    # print(root)  #当前目录路径  
    idx= root.find("resources")
    # print(root[idx:])
    for x in files:
      if x == ".DS_Store":
        continue
      pp = os.path.join(root[idx:],x)
      print(pp,xhash(pp))
      src = os.path.join(root,x)
      par = os.path.split(root)[0]
      newp = os.path.join(root[:idx],"resources2")
      dst = os.path.join(newp,str(xhash(pp)))
      copy_res(src, dst)


if __name__ == '__main__':
  pwd = os.getcwd()
  pwd=os.path.split(pwd)[0]
  pwd=os.path.join(pwd,"resources")
  print pwd
  fetch_files(pwd)

