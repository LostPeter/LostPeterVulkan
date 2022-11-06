@REM author: LostPeter
@REM time:   2022-10-30

@echo off

set folderGLSL="..\..\Bin\Assets\Shader\GLSL"
set folderVulkan="..\..\Bin\Assets\Shader\Vulkan"
if exist %folderVulkan% (
    rmdir /S/Q %folderVulkan%
)
mkdir %folderVulkan%

echo "************** Shader Source .vert/.frag **************"
for /F %%i in ('Dir %folderGLSL%\*.* /B') do glslangValidator -V %folderGLSL%\%%i -o %folderVulkan%\%%i.spv
echo "************** Shader Compile .spv ********************"
for %%i in (%folderVulkan%\*.*) do echo %%i