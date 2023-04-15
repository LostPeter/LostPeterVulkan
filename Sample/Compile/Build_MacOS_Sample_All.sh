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

./Build_MacOS_Sample.sh vulkan_000_window $debug $rebuild
./Build_MacOS_Sample.sh vulkan_001_triangle $debug $rebuild
./Build_MacOS_Sample.sh vulkan_002_imgui $debug $rebuild
./Build_MacOS_Sample.sh vulkan_003_texture $debug $rebuild
./Build_MacOS_Sample.sh vulkan_004_model $debug $rebuild
./Build_MacOS_Sample.sh vulkan_005_camera $debug $rebuild
./Build_MacOS_Sample.sh vulkan_006_depth $debug $rebuild
./Build_MacOS_Sample.sh vulkan_007_stencil $debug $rebuild
./Build_MacOS_Sample.sh vulkan_008_blend $debug $rebuild
./Build_MacOS_Sample.sh vulkan_009_instancing $debug $rebuild
./Build_MacOS_Sample.sh vulkan_010_lighting $debug $rebuild
./Build_MacOS_Sample.sh vulkan_011_texturing $debug $rebuild
./Build_MacOS_Sample.sh vulkan_012_shadering $debug $rebuild
./Build_MacOS_Sample.sh vulkan_013_indirectdraw $debug $rebuild
./Build_MacOS_Sample.sh vulkan_014_terrain $debug $rebuild