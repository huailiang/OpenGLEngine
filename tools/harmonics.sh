# ==============================================
# 球谐光照使用的环境贴图 转化为skybox, 在转化为bundle
# ----------------------------------------------
# Author: Huailiang.Peng
# Data:   2019.07.07
# Useage:
# ==============================================
#!/bin/sh


TEXS=tools/data

SKYS=resources/textures/skybox

ROOT=

function _checkdir()
{
    # 检查目录 
    if [ ! -d "${1}" ]; then  
        echo "error, not found folfer "${1}
        exit 1
    fi
}

function _exec()
{
    if [[ ! -d "${1}" ]]; then
        echo ${1}
    fi
}


function _proc_dir()
{
    echo "processing dir: "${1}" "${2}
    ORIG=${ROOT}/${1}
    DEST=${ROOT}/${SKYS}/${2}
    if [[ ! -d ${DEST} ]]; then
        mkdir ${DEST}

        cp ${ORIG}/posx.jpg ${DEST}/right.jpg
        cp ${ORIG}/negx.jpg ${DEST}/left.jpg
        cp ${ORIG}/posy.jpg ${DEST}/top.jpg
        cp ${ORIG}/negy.jpg ${DEST}/bottom.jpg
        cp ${ORIG}/posz.jpg ${DEST}/front.jpg
        cp ${ORIG}/negz.jpg ${DEST}/back.jpg
    fi
}

function _recy_dir()
{
    echo "processing dir: "${1}" "${2}
    ORIG=${ROOT}/${1}
    DEST=${ROOT}/${SKYS}/${2}
    
    cp ${DEST}/right.jpg ${ORIG}/posx.jpg
    cp ${DEST}/left.jpg ${ORIG}/negx.jpg 
    cp ${DEST}/top.jpg ${ORIG}/posy.jpg
    cp ${DEST}/bottom.jpg ${ORIG}/negy.jpg
    cp ${DEST}/front.jpg ${ORIG}/posz.jpg
    cp ${DEST}/back.jpg ${ORIG}/negz.jpg 
}


function _process()
{
    for element in `ls ${1}`
    do  
        dir_or_file=${1}"/"${element}
        if [ -d ${dir_or_file} ]
        then 
            _proc_dir ${dir_or_file} ${element}
            if [[ $? > 0 ]]; then
                echo "error: "${dir_or_file}
            fi
        else
            echo "file:"$dir_or_file
        fi  
    done
}



cd `dirname $0`
cd ../
ROOT=`pwd`
echo ${ROOT}
_checkdir ${TEXS}
_checkdir ${SKYS}
_process ${TEXS}


echo "job done, good luck"
