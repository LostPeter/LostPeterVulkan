#****************************************************************************
# LostPeterVulkan - Copyright (C) 2022 by LostPeter
# 
# Author:   LostPeter
# Time:     2022-12-10
# Github:   https://github.com/LostPeter/LostPeterVulkan
# Document: https://www.zhihu.com/people/lostpeter/posts
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

debug=${1}
rebuild=${2}
depends=${3}


#1> Dependencies
if [ "$depends" == "depends" ]; then
    cd Dependencies
    cd Compile

    ./Build_MacOS_lib_all.sh $debug $rebuild

    cd ..
    cd ..
fi


#2> Shaders
cd Bin
cd Shader

./Build_MacOS_Shader_HLSL_All.sh $debug

cd ..
cd ..


#3> LostPeterVulkan
cd LostPeterVulkan
cd Compile

./Build_MacOS_LostPeterVulkan.sh $debug $rebuild

cd ..
cd ..


#4> Sample
cd Sample
cd Compile

./Build_MacOS_Sample_All.sh $debug $rebuild

cd ..
cd ..