#****************************************************************************
# LostPeterVulkan - Copyright (C) 2022 by LostPeter
# 
# Author: LostPeter
# Time:   2022-11-13
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

name_folder="glsl"
folderSrc="./$name_folder"
folderShader="../Assets/Shader"

rm -rf $folderShader
mkdir -p $folderShader

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
            glslangValidator -V $1"/"$file -o $folderShader"/"$file.spv
        fi
    done
}

echo "************** Shader Source .vert/.frag **************"
# read_dir $folderSrc
{
    compile_shader $folderSrc
}
echo "************** Shader Compile .spv ********************"
read_dir $folderShader