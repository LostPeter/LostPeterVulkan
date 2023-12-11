#****************************************************************************
# LostPeterVulkan - Copyright (C) 2022 by LostPeter
# 
# Author:   LostPeter
# Time:     2023-12-11
# Github:   https://github.com/LostPeter/LostPeterVulkan
# Document: https://www.zhihu.com/people/lostpeter/posts
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

debug=${1}
rebuild=${2}

name=libsquish-1.15
if [ "$debug" == "debug" ]; then
    name_project=$name"_d"
    name_lib=$name"_d.a"
else
    name_project=$name
    name_lib=$name".a"
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

if [ "$debug" == "debug" ]; then
    cp -rf "../Build/MacOS/"$name_project"/libsquishd.a" "../Lib/MacOS/"$name_lib
else
    cp -rf "../Build/MacOS/"$name_project"/libsquish.a" "../Lib/MacOS/"$name_lib
fi

folderSrc="../Sources/"$name
folderDst="../Include/MacOS/"$name
rm -rf $folderDst
mkdir -p $folderDst

for file in ${folderSrc}/*.h
do 
    cp -rf $file $folderDst
done