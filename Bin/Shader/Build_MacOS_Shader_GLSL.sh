#****************************************************************************
# LostPeterVulkan - Copyright (C) 2022 by LostPeter
# 
# Author: LostPeter
# Time:   2022-11-13
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

name_shader=${1}

name_folder="glsl"
folderSrc="./$name_folder"
folderShader="../Assets/Shader"
mkdir -p $folderShader

glslangValidator -V $folderSrc/$name_shader -o $folderShader/$name_shader.spv