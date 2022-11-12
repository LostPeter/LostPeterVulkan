@REM author: LostPeter
@REM time:   2022-10-30

set debug=%1

call ./Build_Sample_Windows.bat vulkan_000_window %debug%
call ./Build_Sample_Windows.bat vulkan_001_triangle %debug%
call ./Build_Sample_Windows.bat vulkan_002_imgui %debug%