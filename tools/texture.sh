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

TEMP=resources/temp

FORMAT=pvr # format支持：pvr, raw, ktx

ENCODER=PVRTC # PVRTC, ASTC

OPTION=bits-per-pixel-2 # bits-per-pixel-2, bits-per-pixel-4, block-width-4, block-width-5, block-height-4...

TARGET=resources/${FORMAT}

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


_clean $TARGET
_clean $TEMP


mkdir $TARGET

${ITEXTURE} -l

# 利用xcode 自带的texturetool做格式转换
function _export() 
{
	echo ${1%%.*}"."${FORMAT} 
	${ITEXTURE}  -m -f ${FORMAT} -e ${ENCODER} --${OPTION} ${1} -o ${1%%.*}"."${FORMAT} -p ${1%%.*}"_prev."${FORMAT}
}


function _preprocess()
{
    echo "开始预处理图片"
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
    echo "正在生成目标图片"
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

_procesdir $TEMP


mv ${TEMP}/*.${FORMAT} ${TARGET}/

_clean ${TEMP}

echo "job done, good luck"