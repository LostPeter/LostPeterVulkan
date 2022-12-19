@REM author: LostPeter
@REM time:   2022-10-30

set debug=%1
set rebuild=%2

call ./Build_Windows_Sample.bat vulkan_000_window %debug% %rebuild%
call ./Build_Windows_Sample.bat vulkan_001_triangle %debug% %rebuild%
call ./Build_Windows_Sample.bat vulkan_002_imgui %debug% %rebuild%
call ./Build_Windows_Sample.bat vulkan_003_texture %debug% %rebuild%
call ./Build_Windows_Sample.bat vulkan_004_model %debug% %rebuild%
call ./Build_Windows_Sample.bat vulkan_005_camera %debug% %rebuild%
call ./Build_Windows_Sample.bat vulkan_006_depth %debug% %rebuild%
call ./Build_Windows_Sample.bat vulkan_007_stencil %debug% %rebuild%
call ./Build_Windows_Sample.bat vulkan_008_blend %debug% %rebuild%