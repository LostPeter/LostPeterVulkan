#****************************************************************************
# LostPeterRHI - Copyright (C) 2022 by LostPeter
# 
# Author:   LostPeter
# Time:     2024-05-05
# Github:   https://github.com/LostPeter/LostPeterVulkan
# Document: https://www.zhihu.com/people/lostpeter/posts
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

debug=${1}
rebuild=${2}

if [ "$debug" == "debug" ]; then
    name_project="LostPeterRHI_d"
    name_dylib="libLostPeterRHI_d.dylib"
else
    name_project="LostPeterRHI"
    name_dylib="libLostPeterRHI.dylib"
fi


#1> Build LostPeterFoundation
#sh ./Build_MacOS_LostPeterFoundation.sh $debug $rebuild


#2> Build LostPeterRHI
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
    cmake -DDEBUG=1 -DCMAKE_BUILD_TYPE=Debug ../../../LostPeterRHI/
else
    cmake ../../../LostPeterRHI/
fi
make

cd ..
cd ..
cd ..
cd Compile

cp -rf "../Build/MacOS/"$name_project/$name_dylib "../../Plugins/MacOS/"$name_dylib