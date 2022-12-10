@REM author: LostPeter
@REM time:   2022-11-13

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