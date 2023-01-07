@REM #****************************************************************************
@REM # LostPeterVulkan - Copyright (C) 2022 by LostPeter
@REM # 
@REM # Author: LostPeter
@REM # Time:   2022-11-05
@REM #
@REM # This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
@REM #****************************************************************************/

set debug=%1
set rebuild=%2

call ./Build_Windows_lib_glm.bat
call ./Build_Windows_lib_spdlog.bat
call ./Build_Windows_lib_stb.bat

call ./Build_Windows_lib_libnoise.bat %debug% %rebuild%
call ./Build_Windows_lib_zlib.bat %debug% %rebuild%
call ./Build_Windows_lib_glfw.bat %debug% %rebuild%
call ./Build_Windows_lib_imgui.bat %debug% %rebuild%
call ./Build_Windows_lib_assimp.bat %debug% %rebuild%