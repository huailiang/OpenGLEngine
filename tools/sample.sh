# ==============================================
# cubemap采样env, 生成球谐参数
# data对应的目录的下载链接: https://pan.baidu.com/s/1osRsHoB6J3WBhjihH24e7A 提取码: h6b9 
# ----------------------------------------------
# Author: Huailiang.Peng
# Data:   2019.07.05
# Useage:
#   sh sample.sh --write-rendered 
# 	sh sample.sh
# ==============================================
#!/bin/sh


cd `dirname $0`

cd ../

TARGET=glwf_proj/Build/Debug/
DEGREE=3
SAMPLE_NUM=1000000

# WRITE_RENDERED="--write-rendered"

# 如果没有生成版本 报错退出
if [ ! -d "$TARGET" ]; then  
	echo "error, not found build folfer"
	exit 1
fi


echo "arg count: "$#
if [ $# == 1 ] ; then 
    echo "process: "${1} 
    if [[ ${1} == "--write-rendered" ]]; then
    	WRITE_RENDERED="--write-rendered"
    else
    	WRITE_RENDERED=
    fi
fi

cp ${TARGET}/OpenGLEngine ./


for f in tools/data/*
do 
    if [ -d "${f}" ]; then
        echo "===== processing ${f} ====="
		./OpenGLEngine ${f} jpg ${DEGREE} ${SAMPLE_NUM} ${WRITE_RENDERED}
    fi
done


# 移除残留
rm  -rf OpenGLEngine

echo "\njob done, good luck\n"