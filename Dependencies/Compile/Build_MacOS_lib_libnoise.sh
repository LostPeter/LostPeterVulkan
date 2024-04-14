#****************************************************************************
# LostPeterVulkan - Copyright (C) 2022 by LostPeter
# 
# Author:   LostPeter
# Time:     2023-01-07
# Github:   https://github.com/LostPeter/LostPeterVulkan
# Document: https://www.zhihu.com/people/lostpeter/posts
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

debug=${1}
rebuild=${2}

name=libnoise-1.0.0
if [ "$debug" == "debug" ]; then
    name_project=$name"_d"
    name_lib=$name"_d.a"
    nameutil_lib="libnoiseutils-1.0.0_d.a"
else
    name_project=$name
    name_lib=$name".a"
    nameutil_lib="libnoiseutils-1.0.0.a"
fi

if [ "$rebuild" == "rebuild" ]; then
    rm -rf "../Build/MacOS/"$name_project
fi
mkdir -p "../Build/MacOS/"$name_project

rm -rf "../Lib/MacOS/"$name_lib
mkdir -p "../Lib/MacOS"

cd ..
cd Build
cd MacOS
cd $name_project

#lib
if [ "$debug" == "debug" ]; then
    cmake -DDEBUG=1 -DCMAKE_BUILD_TYPE=Debug ../../../Sources/$name/
else
    cmake ../../../Sources/$name/
fi
make

cd ..
cd ..
cd ..
cd Compile

cp -rf "../Build/MacOS/"$name_project"/src/libnoise.a" "../Lib/MacOS/"$name_lib
cp -rf "../Build/MacOS/"$name_project"/noiseutils/libnoiseutils-static.a" "../Lib/MacOS/"$nameutil_lib

folderSrc="../Sources/"$name
folderDst="../Include/MacOS/"$name
rm -rf $folderDst
mkdir -p $folderDst
mkdir -p $folderDst"/noiseutils"

cp -rf $folderSrc"/src/noise" $folderDst

for file in ${folderSrc}/noiseutils/*.h
do 
    cp -rf $file $folderDst"/noiseutils"
done