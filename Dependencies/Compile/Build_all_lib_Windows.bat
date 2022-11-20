@REM author: LostPeter
@REM time:   2022-11-05

set debug=%1

call ./Build_assimp_lib_Windows.bat %debug%
call ./Build_glfw_lib_MacOS.bat %debug%
call ./Build_glm_lib_Windows.bat
call ./Build_imgui_lib_Windows.bat glfw_vulkan %debug%
call ./Build_stb_lib_Windows.bat