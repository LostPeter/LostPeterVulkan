@REM author: LostPeter
@REM time:   2022-11-13

@echo off

set debug=%1

set name_folder=hlsl
set folderSrc=.\%name_folder%
set folderShader=..\Assets\Shader
if exist %folderShader% (
    rmdir /S/Q %folderShader%
)
mkdir %folderShader%

echo "************** Shader Source .vert/.frag **************"
for /F %%i in ('Dir %folderSrc%\*.* /B') do call ./Build_Windows_Shader_HLSL.bat %%i %debug%
echo "************** Shader Compile .spv ********************"
for %%i in (%folderShader%\*.*) do echo %%i