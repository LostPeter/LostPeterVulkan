#****************************************************************************
# LostPeterVulkan - Copyright (C) 2022 by LostPeter
# 
# Author:   LostPeter
# Time:     2022-10-30
# Github:   https://github.com/LostPeter/LostPeterVulkan
# Document: https://www.zhihu.com/people/lostpeter/posts
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

name_sample=${1}
debug=${2}
rebuild=${3}

if [ "$debug" == "debug" ]; then
    name_project="share_"$name_sample"_d"
    name_dylib="libshare_"$name_sample"_d.dylib"
else
    name_project="share_"$name_sample
    name_dylib="libshare_"$name_sample".dylib"
fi

if [ "$rebuild" == "rebuild" ]; then
    rm -rf "../Build/MacOS/"$name_project
fi
mkdir -p "../Build/MacOS/"$name_project

rm -rf "../Lib/MacOS/"$name_dylib
mkdir -p "../Lib/MacOS"

cd ..
cd Build
cd MacOS
cd $name_project

#dylib
if [ "$debug" == "debug" ]; then
    cmake -DDEBUG=1 -DCMAKE_BUILD_TYPE=Debug ../../../$name_sample/share/
else
    cmake ../../../$name_sample/share/
fi
make

cd ..
cd ..
cd ..
cd Compile

cp -rfp "../Build/MacOS/"$name_project/$name_dylib "../Lib/MacOS/"$name_dylib