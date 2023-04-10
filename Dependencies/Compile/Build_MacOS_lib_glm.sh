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

name=glm-0.9.9.8

folderSrc="../Sources/$name/include/glm"
folderDst="../Include/MacOS/"$name
rm -rf $folderDst
mkdir -p $folderDst

cp -pr $folderSrc $folderDst