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

set debug=%1

set name_folder=hlsl
set folderSrc=.\%name_folder%
set folderShader=..\Assets\Shader
if exist %folderShader% (
    rmdir /S/Q %folderShader%
)
mkdir %folderShader%

echo "************** Shader Source .vert/.tesc/.tese/.geom/.frag/.comp **************"
for /F %%i in ('Dir %folderSrc%\*.* /B') do call ./Build_Windows_Shader_HLSL.bat %%i %debug%
echo "************** Shader Source .vert/.tesc/.tese/.geom/.frag/.comp **************"

echo "************** Shader Compile .spv ********************************************"
for %%i in (%folderShader%\*.*) do echo %%i
echo "************** Shader Compile .spv ********************************************"