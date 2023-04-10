@REM #****************************************************************************
@REM # LostPeterVulkan - Copyright (C) 2022 by LostPeter
@REM # 
@REM # Author:   LostPeter
@REM # Time:     2022-10-30
@REM # Github:   https://github.com/LostPeter/LostPeterVulkan
@REM # Document: https://www.zhihu.com/people/lostpeter/posts
@REM #
@REM # This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
@REM #****************************************************************************/

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
call ./Build_Windows_Sample.bat vulkan_009_instancing %debug% %rebuild%
call ./Build_Windows_Sample.bat vulkan_010_lighting %debug% %rebuild%
call ./Build_Windows_Sample.bat vulkan_011_texturing %debug% %rebuild%
call ./Build_Windows_Sample.bat vulkan_012_shadering %debug% %rebuild%
call ./Build_Windows_Sample.bat vulkan_013_indirectdraw %debug% %rebuild%