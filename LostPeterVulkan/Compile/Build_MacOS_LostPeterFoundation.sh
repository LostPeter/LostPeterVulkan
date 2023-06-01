#****************************************************************************
# LostPeterFoundation - Copyright (C) 2022 by LostPeter
# 
# Author:   LostPeter
# Time:     2023-06-01
# Github:   https://github.com/LostPeter/LostPeterVulkan
# Document: https://www.zhihu.com/people/lostpeter/posts
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

debug=${1}
rebuild=${2}

if [ "$debug" == "debug" ]; then
    name_project="LostPeterFoundation_d"
    name_dylib="libLostPeterFoundation_d.dylib"
else
    name_project="LostPeterFoundation"
    name_dylib="libLostPeterFoundation.dylib"
fi

if [ "$rebuild" == "rebuild" ]; then
    rm -rf "../Build/MacOS/"$name_project
fi
mkdir -p "../Build/MacOS/"$name_project

rm -rf "../../Plugins/MacOS/"$name_dylib
mkdir -p "../../Plugins/MacOS"

cd ..
cd Build
cd MacOS
cd $name_project

#dylib
if [ "$debug" == "debug" ]; then
    cmake -DDEBUG=1 -DCMAKE_BUILD_TYPE=Debug ../../../LostPeterFoundation/
else
    cmake ../../../LostPeterFoundation/
fi
make

cd ..
cd ..
cd ..
cd Compile

cp -rf "../Build/MacOS/"$name_project/$name_dylib "../../Plugins/MacOS/"$name_dylib