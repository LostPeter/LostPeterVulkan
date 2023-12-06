#****************************************************************************
# LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
# 
# Author:   LostPeter
# Time:     2023-11-19
# Github:   https://github.com/LostPeter/LostPeterVulkan
# Document: https://www.zhihu.com/people/lostpeter/posts
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

name_plugin=${1}
debug=${2}
rebuild=${3}

if [ "$debug" == "debug" ]; then
    name_project=$name_plugin"_d"
    name_dylib="lib"$name_plugin"_d.dylib"
else
    name_project=$name_plugin
    name_dylib="lib"$name_plugin".dylib"
fi


# Build Plugin
if [ "$rebuild" == "rebuild" ]; then
    rm -rf "../Build/MacOS/"$name_project
fi
mkdir -p "../Build/MacOS/"$name_project

rm -rf "../../Bin/Assets/Plugins/MacOS/"$name_dylib
mkdir -p "../../Bin/Assets/Plugins/MacOS"

cd ..
cd Build
cd MacOS
cd $name_project

#dylib
if [ "$debug" == "debug" ]; then
    cmake -DDEBUG=1 -DCMAKE_BUILD_TYPE=Debug ../../../$name_plugin/
else
    cmake ../../../$name_plugin/
fi
make

cd ..
cd ..
cd ..
cd Compile

cp -rf "../Build/MacOS/"$name_project/$name_dylib "../../Bin/Assets/Plugins/MacOS/"$name_dylib