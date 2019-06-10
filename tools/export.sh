# ==============================================
# 将模型导出 可以直接导入到EBO, VBO
# ----------------------------------------------
# Author: Huailiang.Peng
# Data:   2019.06.05
# ==============================================
#!/bin/sh

echo "** make sure you are running on macos **"

cd `dirname $0`

cd ../

TARGET=glwf_proj/Build/Debug/

OBJS=resources/objects

MESH=resources/mesh/


# 如果没有生成版本 报错退出
if [ ! -d "$TARGET" ]; then  
	echo "error, not found build folfer"
	exit 1
fi

# 清空目录
if [ -d "$MESH" ]; then  
	rm -rf $MESH
	mkdir $MESH
fi


# 使用c++导出mesh等资源
function _export() 
{
	echo "\nstart export ${1}"
	./OpenGLEngine "tools" $1

}

cp ${TARGET}/OpenGLEngine ./


# 遍历资源目录 
dir=$(ls -l ${OBJS} |awk '/^d/ {print $NF}')

for i in $dir
do
	_export $i
done


# 移除残留
rm  -rf OpenGLEngine


echo "\njob done, good luck\n"