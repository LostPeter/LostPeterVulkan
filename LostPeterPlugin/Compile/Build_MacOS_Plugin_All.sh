#****************************************************************************
# LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
# 
# Author:   LostPeter
# Time:     2023-12-06
# Github:   https://github.com/LostPeter/LostPeterVulkan
# Document: https://www.zhihu.com/people/lostpeter/posts
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

debug=${1}
rebuild=${2}

./Build_MacOS_Plugin.sh LostPeterPluginRendererVulkan $debug $rebuild
./Build_MacOS_Plugin.sh LostPeterPluginImageCodecSTB $debug $rebuild