@REM #****************************************************************************
@REM # LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
@REM # 
@REM # Author:   LostPeter
@REM # Time:     2023-12-06
@REM # Github:   https://github.com/LostPeter/LostPeterVulkan
@REM # Document: https://www.zhihu.com/people/lostpeter/posts
@REM #
@REM # This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
@REM #****************************************************************************/

set debug=%1
set rebuild=%2

call ./Build_Windows_Plugin.bat LostPeterPluginImageCodecSTB %debug% %rebuild%

call ./Build_Windows_Plugin.bat LostPeterPluginRHIDummy %debug% %rebuild%
call ./Build_Windows_Plugin.bat LostPeterPluginRHIVulkan %debug% %rebuild%

call ./Build_Windows_Plugin.bat LostPeterPluginRendererVulkan %debug% %rebuild%