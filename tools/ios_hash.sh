# ==============================================
# 利用xcode-texturetool 将jpg，png等格式的图片转换成gpu直接可读的压缩图片
# 1. 生成hash 文件
# 2. 恢复原文件
# ----------------------------------------------
# Author: Huailiang.Peng
# Data:   2019.06.18
# Useage:
# ==============================================
#!/bin/sh


TEXS=resources/textures

OBJS=resources/objects

TEMP=resources/temp


function _check()
{
    if [ ! -d "${1}" ]; then  
        echo "\033[31m error, not found folfer ${1}  \033[0m \n"
        exit 1
    fi
}

# 清空生成目录
function _clean()
{
    if [ -d "${1}" ]; then  
    rm -rf ${1}
    fi
}


function _backup()
{
    rm -rf ${1}
    mv -f ${1}_tmp ${1}
}



cd `dirname ${0}`
cd ..
_check ${TEXS}_tmp
_check ${OBJS}_tmp
cd tools/
python hash.py
cd ..
_backup ${TEXS}
_backup ${OBJS}
echo "job done, good luck"