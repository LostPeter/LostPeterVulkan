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

debug=${1}
rebuild=${2}

# mode only can be gl3, gles3_android, gles3_ios, glfw_gl3, glfw_vulkan, metal
m_gl3="gl3"
m_gles_android="gles3_android"
m_gles_ios="gles3_ios"
m_metal="metal"
m_glfw_gl3="glfw_gl3"
m_glfw_vulkan="glfw_vulkan"
m_glfw_metal="glfw_metal"

mode=${m_glfw_vulkan}
if [ [ "$mode" != "$m_gl3" ]\
    && [ "$mode" != "$m_gles_android" ]\
    && [ "$mode" != "$m_gles_ios" ]\
    && [ "$mode" != "$m_metal" ]\
    && [ "$mode" != "$m_glfw_gl3" ]\
    && [ "$mode" != "$m_glfw_vulkan" ]\
    && [ "$mode" != "$m_glfw_metal" ] ]; then

    echo "wrong mode type: ["$mode"], only can be [$m_gl3][$m_gles_android][$m_gles_ios][$m_metal][$m_glfw_gl3][$m_glfw_vulkan][$m_glfw_metal]"
    exit 1
fi
echo "mode: $mode, $debug"

name_base="imgui"
name="${name_base}-1.85"

if [ "$debug" == "debug" ]; then
    name_project=$name"_"$mode"_d"
    name_dylib="lib"$name"_"$mode"_d.a"
else
    name_project=$name"_"$mode
    name_dylib="lib"$name"_"$mode".a"
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

#lib
if [ "$debug" == "debug" ]; then
    cmake -DDEBUG=1 -DCMAKE_BUILD_TYPE=Debug -DPLATFORM_MODE=$mode ../../../Sources/$name/
else
    cmake -DPLATFORM_MODE=$mode ../../../Sources/$name/
fi
make

cd ..
cd ..
cd ..
cd Compile

cp -rfp "../Build/MacOS/"$name_project/$name_dylib "../Lib/MacOS/"$name_dylib

folderSrc="../Sources/$name/$name_base"
folderDstRoot="../Include/MacOS/"$name"_"$mode
rm -rf $folderDstRoot
mkdir -p $folderDstRoot
folderDst=$folderDstRoot"/"$name_base
mkdir -p $folderDst

for file in ${folderSrc}/*.h
do 
    cp -rfp $file $folderDst
done

if [ "$mode" == "$m_gl3" ]; then 
    echo "mode: $mode"

elif [ "$mode" == "$m_gles_android" ]; then 
    echo "mode: $mode"

elif [ "$mode" == "$m_gles_ios" ]; then 
    echo "mode: $mode"

elif [ "$mode" == "$m_metal" ]; then 
    echo "mode: $mode"

elif [ "$mode" == "$m_glfw_gl3" ]; then 
    echo "mode: $mode"

    folderSrc_Glfw="../Sources/"$name"/impl_glfw"
    floderSrc_GL3="../Sources/"$name"/impl_gl3"
    folderDst_Glfw="../Include/MacOS/"$name"_"$mode"/impl_glfw/"
    folderDst_GL3="../Include/MacOS/"$name"_"$mode"/impl_gl3/"
    mkdir -p $folderDst_Glfw
    mkdir -p $folderDst_GL3

    for file in ${folderSrc_Glfw}/*.h
    do 
        cp -rfp $file $folderDst_Glfw
    done

    for file in ${floderSrc_GL3}/*.h
    do 
        cp -rfp $file $folderDst_GL3
    done

elif [ "$mode" == "$m_glfw_vulkan" ]; then 
    echo "mode: $mode"

    folderSrc_Glfw="../Sources/"$name"/impl_glfw"
    floderSrc_Vulkan="../Sources/"$name"/impl_vulkan"
    folderDst_Glfw="../Include/MacOS/"$name"_"$mode"/impl_glfw/"
    folderDst_Vulkan="../Include/MacOS/"$name"_"$mode"/impl_vulkan/"
    mkdir -p $folderDst_Glfw
    mkdir -p $folderDst_Vulkan

    for file in ${folderSrc_Glfw}/*.h
    do 
        cp -rfp $file $folderDst_Glfw
    done

    for file in ${floderSrc_Vulkan}/*.h
    do 
        cp -rfp $file $folderDst_Vulkan
    done

elif [ "$mode" == "$m_glfw_metal" ]; then 
    echo "mode: $mode"

    folderSrc_Glfw="../Sources/"$name"/impl_glfw"
    floderSrc_Metal="../Sources/"$name"/impl_metal"
    folderDst_Glfw="../Include/MacOS/"$name"_"$mode"/impl_glfw/"
    folderDst_Metal="../Include/MacOS/"$name"_"$mode"/impl_metal/"
    mkdir -p $folderDst_Glfw
    mkdir -p $folderDst_Metal

    for file in ${folderSrc_Glfw}/*.h
    do 
        cp -rfp $file $folderDst_Glfw
    done

    for file in ${floderSrc_Metal}/*.h
    do 
        cp -rfp $file $folderDst_Metal
    done

else 
    echo "mode: $mode, unknown !"
    
fi
