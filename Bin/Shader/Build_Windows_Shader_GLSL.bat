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

echo file name: %name_shader%
echo folder name: %folder_shader%

set folderSrc=".\%folder_shader%"
set folderShader="..\Assets\Shader"
if not exist %folderShader% (
    mkdir %folderShader%
)

glslangValidator -V %folderSrc%\%name_shader% -o %folderShader%\%name_shader%.spv