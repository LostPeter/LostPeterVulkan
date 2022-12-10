@REM author: LostPeter
@REM time:   2022-11-05

set debug=%1
set rebuild=%2

call ./Build_Windows_lib_glm.bat
call ./Build_Windows_lib_spdlog.bat
call ./Build_Windows_lib_stb.bat

call ./Build_Windows_lib_zlib.bat %debug% %rebuild%
call ./Build_Windows_lib_glfw.bat %debug% %rebuild%
call ./Build_Windows_lib_imgui.bat glfw_vulkan %debug% %rebuild%
call ./Build_Windows_lib_assimp.bat %debug% %rebuild%