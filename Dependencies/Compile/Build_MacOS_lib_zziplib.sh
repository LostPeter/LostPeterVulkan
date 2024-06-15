#****************************************************************************
# LostPeterVulkan - Copyright (C) 2022 by LostPeter
# 
# Author:   LostPeter
# Time:     2024-01-01
# Github:   https://github.com/LostPeter/LostPeterVulkan
# Document: https://www.zhihu.com/people/lostpeter/posts
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

debug=${1}
rebuild=${2}

name=zziplib-0.13.72
namewrap=zzipwraplib-0.13.72
if [ "$debug" == "debug" ]; then
    name_project=$name"_d"
    name_lib="lib"$name"_d.a"
    name_lib_wrap="lib"$namewrap"_d.a"
else
    name_project=$name
    name_lib="lib"$name".a"
    name_lib_wrap="lib"$namewrap".a"
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
    cp -rfp "../Build/MacOS/"$name_project"/zzip/libzzip.a" "../Lib/MacOS/"$name_lib
    cp -rfp "../Build/MacOS/"$name_project"/zzipwrap/libzzipwrap.a" "../Lib/MacOS/"$name_lib_wrap
else
    cp -rfp "../Build/MacOS/"$name_project"/zzip/libzzip-0.a" "../Lib/MacOS/"$name_lib
    cp -rfp "../Build/MacOS/"$name_project"/zzipwrap/libzzipwrap-0.a" "../Lib/MacOS/"$name_lib_wrap
fi


folderSrc="../Sources/"$name"/zzip"
folderDst="../Include/MacOS/"$name"/zzip"
rm -rf $folderDst
mkdir -p $folderDst

for file in ${folderSrc}/*.h
do 
    cp -rfp $file $folderDst
done
cp -rfp "../Build/MacOS/"$name_project"/zzip/_config.h" $folderDst


folderSrcWrap="../Sources/"$name"/zzipwrap"
folderDstWrap="../Include/MacOS/"$name"/zzipwrap"
rm -rf $folderDstWrap
mkdir -p $folderDstWrap

for file in ${folderSrcWrap}/*.h
do 
    cp -rfp $file $folderDstWrap
done