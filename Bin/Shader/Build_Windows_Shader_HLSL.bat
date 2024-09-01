@REM #****************************************************************************
@REM # LostPeterVulkan - Copyright (C) 2022 by LostPeter
@REM # 
@REM # Author:   LostPeter
@REM # Time:     2022-11-13
@REM # Github:   https://github.com/LostPeter/LostPeterVulkan
@REM # Document: https://www.zhihu.com/people/lostpeter/posts
@REM #
@REM # This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
@REM #****************************************************************************/

@echo off

set name_shader=%1
set folder_shader=%2
set folder_include=%3
set debug=%4

set name_vert=\.vert
set name_tesc=\.tesc
set name_tese=\.tese
set name_geom=\.geom
set name_frag=\.frag
set name_comp=\.comp

echo file name: %name_shader%
echo folder name: %folder_shader%
echo folder include: %folder_include%

set /A isProcess=1

echo %name_shader%| findstr %name_vert% >nul && (
    set name_profile=vs_6_1
	@REM echo "file is .vert !"
) || echo %name_shader%| findstr %name_tesc% >nul && (
    set name_profile=hs_6_1
	@REM echo "file is .tesc !"
) || echo %name_shader%| findstr %name_tese% >nul && (
    set name_profile=ds_6_1
	@REM echo "file is .tese !"
) || echo %name_shader%| findstr %name_geom% >nul && (
    set name_profile=gs_6_1
	@REM echo "file is .geom !"
) || echo %name_shader%| findstr %name_frag% >nul && (
    set name_profile=ps_6_1
	@REM echo "file is .frag !"
) || echo %name_shader%| findstr %name_comp% >nul && (
    set name_profile=cs_6_1
	@REM echo "file is .comp !"
) || ( echo Ignore file, is not [.vert/.tesc/.tese/.geom/.frag.comp], name: %name_shader%
    set /A isProcess=0
)


set folderSrc=.\%folder_shader%
set folderShader=..\Assets\Shader
if not exist %folderShader% (
    mkdir %folderShader%
)

if %isProcess% EQU 1 (
    echo profile name: %name_profile%

    if "%debug%" == "debug" (
        dxc -Od -Zi -spirv -T %name_profile% -E main -I %folder_include% -fspv-extension=SPV_KHR_ray_tracing -fspv-extension=SPV_KHR_multiview -fspv-extension=SPV_KHR_shader_draw_parameters -fspv-extension=SPV_EXT_descriptor_indexing -fspv-extension=SPV_KHR_non_semantic_info -fspv-debug=vulkan-with-source %folderSrc%\%name_shader% -Fo %folderShader%\%name_shader%.spv
    ) else (
        dxc -spirv -T %name_profile% -E main -I %folder_include% -fspv-extension=SPV_KHR_ray_tracing -fspv-extension=SPV_KHR_multiview -fspv-extension=SPV_KHR_shader_draw_parameters -fspv-extension=SPV_EXT_descriptor_indexing %folderSrc%\%name_shader% -Fo %folderShader%\%name_shader%.spv
    )
)