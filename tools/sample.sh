# ==============================================
# cubemap采样env, 生成球谐参数
# ----------------------------------------------
# Author: Huailiang.Peng
# Data:   2019.07.05
# Useage:
# ==============================================
#!/bin/sh


cd `dirname $0`

cd ../

TARGET=glwf_proj/Build/Debug/


# 如果没有生成版本 报错退出
if [ ! -d "$TARGET" ]; then  
	echo "error, not found build folfer"
	exit 1
fi

cp ${TARGET}/OpenGLEngine ./


degree=3
samplenum=1000000
write_rendered=
#write_rendered="--write-rendered"
for f in data/*
do 
    if [ -d "$f" ]; then
        echo "===== processing $f ====="
		OpenGLEngine $f jpg $degree $samplenum $write_rendered
    fi
done



# 移除残留
rm  -rf OpenGLEngine



echo "\njob done, good luck\n"