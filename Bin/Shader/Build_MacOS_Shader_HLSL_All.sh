#****************************************************************************
# LostPeterVulkan - Copyright (C) 2022 by LostPeter
# 
# Author:   LostPeter
# Time:     2022-11-13
# Github:   https://github.com/LostPeter/LostPeterVulkan
# Document: https://www.zhihu.com/people/lostpeter/posts
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

debug=${1}

name_folder="hlsl"
dirSrc="./$name_folder"
dirDst="../Assets/Shader"
dirInclude="./$name_folder"

rm -rf $dirDst
mkdir -p $dirDst

function read_dir() {
    for file in `ls $1`
    do 
        if [ -d $1"/"$file ]; then
            read_dir $1"/"$file 
        else
            echo $1"/"$file 
        fi
    done
}

function compile_shader() {
    for file in `ls $1`
    do 
        if [ -d $1"/"$file ]; then
            compile_shader $1"/"$file
        else
            ./Build_MacOS_Shader_HLSL.sh $file $1 $dirInclude $debug
        fi
    done
}

echo "************** Shader Source .vert/.tesc/.tese/.geom/.frag/.comp **************"
{
    compile_shader $dirSrc
}
echo "************** Shader Source .vert/.tesc/.tese/.geom/.frag/.comp **************"

echo "************** Shader Compile .spv ********************************************"
{
    read_dir $dirDst
}
echo "************** Shader Compile .spv ********************************************"