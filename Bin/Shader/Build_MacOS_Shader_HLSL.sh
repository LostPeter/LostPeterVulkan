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
debug=${3}

name_vert="\.vert"
name_tesc="\.tesc"
name_tese="\.tese"
name_geom="\.geom"
name_frag="\.frag"
name_comp="\.comp"

name_profile=""
if [[ $name_shader =~ $name_vert ]]; then
    name_profile="vs_6_1"
	# echo "file is .vert !"
elif [[ $name_shader =~ $name_tesc ]]; then
    name_profile="hs_6_1"
	# echo "file is .tesc !"
elif [[ $name_shader =~ $name_tese ]]; then
    name_profile="ds_6_1"
	# echo "file is .tese !"
elif [[ $name_shader =~ $name_geom ]]; then
    name_profile="gs_6_1"
	# echo "file is .geom !"
elif [[ $name_shader =~ $name_frag ]]; then
    name_profile="ps_6_1"
	# echo "file is .frag !"
elif [[ $name_shader =~ $name_comp ]]; then
    name_profile="cs_6_1"
	# echo "file is .comp !"
else
	echo "file is not valid !"
	exit
fi


folderSrc=$folder_shader
folderVulkan="../Assets/Shader"
mkdir -p $folderVulkan

if [ $debug == "debug" ]; then
	dxc -Zi -spirv -T $name_profile -E main -fspv-extension=SPV_KHR_ray_tracing -fspv-extension=SPV_KHR_multiview -fspv-extension=SPV_KHR_shader_draw_parameters -fspv-extension=SPV_EXT_descriptor_indexing $folderSrc/$name_shader -Fo $folderVulkan/$name_shader.spv
else
	dxc -spirv -T $name_profile -E main -fspv-extension=SPV_KHR_ray_tracing -fspv-extension=SPV_KHR_multiview -fspv-extension=SPV_KHR_shader_draw_parameters -fspv-extension=SPV_EXT_descriptor_indexing $folderSrc/$name_shader -Fo $folderVulkan/$name_shader.spv
fi