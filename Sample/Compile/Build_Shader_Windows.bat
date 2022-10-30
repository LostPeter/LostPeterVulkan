@REM author: LostPeter
@REM time:   2022-10-29

@echo off

set name_shader=%1

set folderGLSL="..\..\Bin\Assets\Shader\GLSL"
set folderVulkan="..\..\Bin\Assets\Shader\Vulkan"
if not exist %folderVulkan% (
    mkdir %folderVulkan%
)

glslangValidator -V %folderGLSL%\%name_shader% -o %folderVulkan%\%name_shader%.spv