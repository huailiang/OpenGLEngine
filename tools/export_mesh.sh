# ==============================================
# This tool is for export batch mesh
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

if [ ! -d "$TARGET" ]; then  
	echo "error, not found build folfer"
	exit 1
fi


function _export() 
{
	echo "\nstart export ${1}"
	./OpenGLEngine "tools" $1

}

cp ${TARGET}/OpenGLEngine ./

dir=$(ls -l ${OBJS} |awk '/^d/ {print $NF}')

for i in $dir
do
	_export $i
done


rm  -rf OpenGLEngine

echo "\njob done, good luck\n"