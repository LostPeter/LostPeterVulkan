#****************************************************************************
# LostPeterVulkan - Copyright (C) 2022 by LostPeter
# 
# Author:   LostPeter
# Time:     2022-11-05
# Github:   https://github.com/LostPeter/LostPeterVulkan
# Document: https://www.zhihu.com/people/lostpeter/posts
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

debug=${1}
rebuild=${2}

./Build_MacOS_lib_glm.sh
./Build_MacOS_lib_spdlog.sh
./Build_MacOS_lib_stb.sh

./Build_MacOS_lib_libnoise.sh $debug $rebuild
./Build_MacOS_lib_zlib.sh $debug $rebuild
./Build_MacOS_lib_glfw.sh $debug $rebuild
./Build_MacOS_lib_imgui.sh $debug $rebuild
./Build_MacOS_lib_assimp.sh $debug $rebuild