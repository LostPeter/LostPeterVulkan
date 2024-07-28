#****************************************************************************
# LostPeterVulkan - Copyright (C) 2022 by LostPeter
# 
# Author:   LostPeter
# Time:     2024-06-08
# Github:   https://github.com/LostPeter/LostPeterVulkan
# Document: https://www.zhihu.com/people/lostpeter/posts
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

debug=${1}
rebuild=${2}

name=spirv-cross-vulkan-sdk-1.3.283

name_lib_core=libspirv-cross-core
name_lib_c=libspirv-cross-c
name_lib_cpp=libspirv-cross-cpp
name_lib_glsl=libspirv-cross-glsl
name_lib_hlsl=libspirv-cross-hlsl
name_lib_msl=libspirv-cross-msl
name_lib_reflect=libspirv-cross-reflect
name_lib_util=libspirv-cross-util
name_exe=spirv-cross

if [ "$debug" == "debug" ]; then
    name_project=$name"_d"
    name_lib_core_final=$name_lib_core"_d.a"
    name_lib_c_final=$name_lib_c"_d.a"
    name_lib_cpp_final=$name_lib_cpp"_d.a"
    name_lib_glsl_final=$name_lib_glsl"_d.a"
    name_lib_hlsl_final=$name_lib_hlsl"_d.a"
    name_lib_msl_final=$name_lib_msl"_d.a"
    name_lib_reflect_final=$name_lib_reflect"_d.a"
    name_lib_util_final=$name_lib_util"_d.a"
    name_exe_final=$name_exe"_d"
else
    name_project=$name
    name_lib_core_final=$name_lib_core".a"
    name_lib_c_final=$name_lib_c".a"
    name_lib_cpp_final=$name_lib_cpp".a"
    name_lib_glsl_final=$name_lib_glsl".a"
    name_lib_hlsl_final=$name_lib_hlsl".a"
    name_lib_msl_final=$name_lib_msl".a"
    name_lib_reflect_final=$name_lib_reflect".a"
    name_lib_util_final=$name_lib_util".a"
    name_exe_final=$name_exe
fi

if [ "$rebuild" == "rebuild" ]; then
    rm -rf "../Build/MacOS/"$name_project
fi
mkdir -p "../Build/MacOS/"$name_project

rm -rf "../Lib/MacOS/"$name_lib_core_final
rm -rf "../Lib/MacOS/"$name_lib_c_final
rm -rf "../Lib/MacOS/"$name_lib_cpp_final
rm -rf "../Lib/MacOS/"$name_lib_glsl_final
rm -rf "../Lib/MacOS/"$name_lib_hlsl_final
rm -rf "../Lib/MacOS/"$name_lib_msl_final
rm -rf "../Lib/MacOS/"$name_lib_reflect_final
rm -rf "../Lib/MacOS/"$name_lib_util_final
rm -rf "../../Bin/MacOS/"$name_exe_final
mkdir -p "../Lib/MacOS"

cd ..
cd Build
cd MacOS
cd $name_project

#lib
if [ "$debug" == "debug" ]; then
    cmake -DDEBUG=1 -DCMAKE_BUILD_TYPE=Debug -DSPIRV_CROSS_ENABLE_TESTS=OFF ../../../Sources/$name/
else
    cmake -DSPIRV_CROSS_ENABLE_TESTS=OFF ../../../Sources/$name/
fi
make

cd ..
cd ..
cd ..
cd Compile

cp -rfp "../Build/MacOS/"$name_project"/"$name_lib_core".a" "../Lib/MacOS/"$name_lib_core_final
cp -rfp "../Build/MacOS/"$name_project"/"$name_lib_c".a" "../Lib/MacOS/"$name_lib_c_final
cp -rfp "../Build/MacOS/"$name_project"/"$name_lib_cpp".a" "../Lib/MacOS/"$name_lib_cpp_final
cp -rfp "../Build/MacOS/"$name_project"/"$name_lib_glsl".a" "../Lib/MacOS/"$name_lib_glsl_final
cp -rfp "../Build/MacOS/"$name_project"/"$name_lib_hlsl".a" "../Lib/MacOS/"$name_lib_hlsl_final
cp -rfp "../Build/MacOS/"$name_project"/"$name_lib_msl".a" "../Lib/MacOS/"$name_lib_msl_final
cp -rfp "../Build/MacOS/"$name_project"/"$name_lib_reflect".a" "../Lib/MacOS/"$name_lib_reflect_final
cp -rfp "../Build/MacOS/"$name_project"/"$name_lib_util".a" "../Lib/MacOS/"$name_lib_util_final
cp -rfp "../Build/MacOS/"$name_project"/"$name_exe "../../Bin/MacOS/"$name_exe_final

folderSrc="../Sources/$name"
folderDst="../Include/MacOS/"$name
rm -rf $folderDst
mkdir -p $folderDst

for file in ${folderSrc}/*.h
do 
    cp -rfp $file $folderDst
done
for file in ${folderSrc}/*.hpp
do 
    cp -rfp $file $folderDst
done