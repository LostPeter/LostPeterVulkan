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

set name_folder="glsl"
set folderSrc=".\%name_folder%"
set folderShader="..\Assets\Shader"
if exist %folderShader% (
    rmdir /S/Q %folderShader%
)
mkdir %folderShader%

echo "************** Shader Source .vert/.frag **************"
for /F %%i in ('Dir %folderSrc%\*.* /B') do glslangValidator -V %folderSrc%\%%i -o %folderShader%\%%i.spv
echo "************** Shader Compile .spv ********************"
for %%i in (%folderShader%\*.*) do echo %%i