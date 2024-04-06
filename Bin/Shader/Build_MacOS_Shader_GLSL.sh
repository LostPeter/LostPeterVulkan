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

name_shader=${1}
folder_shader=${2}

folderSrc=$folder_shader
folderVulkan="../Assets/Shader"
mkdir -p $folderVulkan

glslangValidator -V $folderSrc/$name_shader -o $folderVulkan/$name_shader.spv