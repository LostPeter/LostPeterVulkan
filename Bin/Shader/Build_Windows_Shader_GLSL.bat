@REM #****************************************************************************
@REM # LostPeterVulkan - Copyright (C) 2022 by LostPeter
@REM # 
@REM # Author: LostPeter
@REM # Time:   2022-11-13
@REM #
@REM # This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
@REM #****************************************************************************/

@echo off

set name_shader=%1

set name_folder="glsl"
set folderSrc=".\%name_folder%"
set folderShader="..\Assets\Shader"
if not exist %folderShader% (
    mkdir %folderShader%
)

glslangValidator -V %folderSrc%\%name_shader% -o %folderShader%\%name_shader%.spv