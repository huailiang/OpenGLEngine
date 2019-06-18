# ==============================================
# 利用xcode-texturetool 将jpg，png等格式的图片转换成gpu直接可读的压缩图片
# 1. 调用solution.py  进行分辨率转换
# 2. 根据新的分辨率生成gpu直读文件
# ----------------------------------------------
# Author: Huailiang.Peng
# Data:   2019.06.18
# ==============================================
#!/bin/sh


IXCODE=`xcode-select -print-path`

ISDK=$IXCODE/Platforms/iPhoneOS.platform/Developer

ITEXTURE=$ISDK/usr/bin/texturetool

TEXS=resources/textures

TEXS2=resources/texture2

PVR=resources/pvr

cd `dirname $0`

cd ../



# 检查纹理目录 
if [ ! -d "$TEXS" ]; then  
	echo "error, not found textures folfer"
	exit 1
fi


$ITEXTURE -l

# 清空生成目录
function _clean()
{
    if [ -d "${1}" ]; then  
    rm -rf ${1}
    fi
}


_clean $PVR
_clean $TEXS2


mkdir $PVR

${ITEXTURE} -l

# 利用xcode 自带的texturetool做格式转换
function _export() 
{
	echo ${1%%.*}."pvr" 
	${ITEXTURE}  -m -f PVR -e PVRTC --bits-per-pixel-4 ${1} -o ${1%%.*}."pvr" 
}


function _preprocess()
{
    for element in `ls $1`
    do  
        dir_or_file=$1"/"$element
        if [ -d $dir_or_file ]
        then 
            # _preprocess $dir_or_file
            echo $dir_or_file
        else
            python tools/solution.py $dir_or_file
        fi  
    done
}


function _procesdir()
{
    for element in `ls $1`
    do  
        dir_or_file=$1"/"$element
        if [ -d $dir_or_file ]
        then 
            # _procesdir $dir_or_file
            echo $dir_or_file
        else
            _export $dir_or_file
        fi  
    done
}

_preprocess $TEXS

_procesdir $TEXS2


mv ${TEXS2}/*.pvr ${PVR}/

echo "job done, good luck"