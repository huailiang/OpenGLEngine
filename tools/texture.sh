# ==============================================
# 利用xcode-texturetool 将jpg，png等格式的图片转换成gpu直接可读的压缩图片
# 1. 调用solution.py  进行分辨率转换
# 2. 根据新的分辨率生成gpu直读文件
#
# 如果传入参数， 则参数只能是文件路径
# 此时只对文件路径生成对应的pvr, 而不是所有的文件
# ----------------------------------------------
# Author: Huailiang.Peng
# Data:   2019.06.18
# Useage:
# ==============================================
#!/bin/sh


IXCODE=`xcode-select -print-path`

ISDK=$IXCODE/Platforms/iPhoneOS.platform/Developer

ITEXTURE=$ISDK/usr/bin/texturetool

TEXS=resources/textures

OBJS=resources/objects

TEMP=resources/temp

FORMAT=pvr # format支持：pvr, raw, ktx

ENCODER=PVRTC # PVRTC, ASTC

OPTION=bits-per-pixel-2 # bits-per-pixel-2, bits-per-pixel-4, block-width-4, block-width-5, block-height-4...



function _checkdir()
{
    # 检查目录 
    if [ ! -d "${1}" ]; then  
        echo "error, not found folfer"${1}
        exit 1
    fi
}

function _checkenv()
{
    if [ -d "${TEXS}_tmp" ]; then  
        echo "\033[31m exit, error: there is already texs_tmp \033[0m \n"
        exit 10
    fi
    if [ -d "${OBJS}_tmp" ]; then  
        echo "\033[31mexit, error: there is already texs_tmp \033[0m \n"
        exit 11
    fi
}

function _extern()
{
    # echo ${1:0-3}  # 后缀
    if [[ ${1:0-3} = "jpg" ]]; then
        return 1
    fi
    if [[ ${1:0-3} = "png" ]]; then
        return 2
    fi
    if [[ ${1:0-4} = "jpeg" ]]; then
        return 3
    fi
    if [[ ${1:0-3} = "tga" ]]; then
        return 4
    fi
    return 0
}

# 清空生成目录
function _cleandir()
{
    if [ -d "${1}" ]; then  
    rm -rf ${1}
    fi
}


# 利用xcode 自带的texturetool做格式转换
function _export() 
{
	echo ${1%%.*}"."${FORMAT} 
    ${ITEXTURE}  -m -f ${FORMAT} -e ${ENCODER} --${OPTION} ${1} -o ${1%%.*}"."${FORMAT} #-p ${1%%.*}"_prev."${FORMAT}
    if [[ $? = 0 ]]; then  # 生成成功的话 删除原文件
        rm -rf ${1}
    fi
}


function _preprocess()
{
    echo "预处理"${1}
    for element in `ls $1`
    do  
        dir_or_file=$1"/"$element
        if [ -d $dir_or_file ]
        then 
            _preprocess $dir_or_file
        else
            _extern $dir_or_file
            if [[ $? > 0 ]]; then
                python tools/solution.py $dir_or_file
            fi
        fi  
    done
}


function _procesdir()
{
    echo "正在处理目录:"${1}
    for element in `ls $1`
    do  
        dir_or_file=$1"/"$element
        if [ -d $dir_or_file ]
        then 
            _procesdir $dir_or_file
        else
            _extern $dir_or_file
            if [[ $? > 0 ]]; then
                _export $dir_or_file
            fi
        fi  
    done
}


function _process()
{
    _checkdir ${1}
    _cleandir ${TEMP}
    cp -R ${1} ${TEMP}
    _preprocess ${TEMP}
    _procesdir ${TEMP}
    mv -f ${1} ${1}_tmp
    mv -f ${TEMP} ${1}
}


cd `dirname $0`
cd ../

if [ $# == 1 ] ; then 
    echo "process: "${1} 
    _extern ${1}
    if [[ $? > 0 ]]; then
        _export ${1}
    else
        echo ${1}" is not valid"
        exit 1
    fi
else
    _checkenv 
    $ITEXTURE -l
    echo "************ start ************"
    _process ${TEXS}
    _process ${OBJS}
fi

echo "job done, good luck"