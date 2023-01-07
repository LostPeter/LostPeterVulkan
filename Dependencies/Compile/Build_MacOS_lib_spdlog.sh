#****************************************************************************
# LostPeterVulkan - Copyright (C) 2022 by LostPeter
# 
# Author: LostPeter
# Time:   2022-12-03
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

name=spdlog-1.10.0

folderSrc="../Sources/$name/include/spdlog"
folderDst="../Include/MacOS/"$name
rm -rf $folderDst
mkdir -p $folderDst

cp -r $folderSrc $folderDst