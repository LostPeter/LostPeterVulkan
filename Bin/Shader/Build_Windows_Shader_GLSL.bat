@REM author: LostPeter
@REM time:   2022-11-13

@echo off

set name_shader=%1

set name_folder="glsl"
set folderSrc=".\%name_folder%"
set folderShader="..\Assets\Shader"
if not exist %folderShader% (
    mkdir %folderShader%
)

glslangValidator -V %folderSrc%\%name_shader% -o %folderShader%\%name_shader%.spv