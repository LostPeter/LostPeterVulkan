@REM #****************************************************************************
@REM # LostPeterVulkan - Copyright (C) 2022 by LostPeter
@REM # 
@REM # Author:   LostPeter
@REM # Time:     2024-06-08
@REM # Github:   https://github.com/LostPeter/LostPeterVulkan
@REM # Document: https://www.zhihu.com/people/lostpeter/posts
@REM #
@REM # This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
@REM #****************************************************************************/

@echo off
set debug=%1
set rebuild=%2

echo %debug%
echo %rebuild%

set name="spirv-cross-vulkan-sdk-1.3.283"

set name_lib_core="spirv-cross-core"
set name_lib_c="spirv-cross-c"
set name_lib_cpp="spirv-cross-cpp"
set name_lib_glsl="spirv-cross-glsl"
set name_lib_hlsl="spirv-cross-hlsl"
set name_lib_msl="spirv-cross-msl"
set name_lib_reflect="spirv-cross-reflect"
set name_lib_util="spirv-cross-util"
set name_exe="spirv-cross"

if "%debug%" == "debug" (
    set name_project=%name%"_d"
    set name_lib_core_final=%name_lib_core%"_d.lib"
    set name_lib_c_final=%name_lib_c%"_d.lib"
    set name_lib_cpp_final=%name_lib_cpp%"_d.lib"
    set name_lib_glsl_final=%name_lib_glsl%"_d.lib"
    set name_lib_hlsl_final=%name_lib_hlsl%"_d.lib"
    set name_lib_msl_final=%name_lib_msl%"_d.lib"
    set name_lib_reflect_final=%name_lib_reflect%"_d.lib"
    set name_lib_util_final=%name_lib_util%"_d.lib"
    set name_exe_final=%name_exe%"_d.exe"
) else (
    set name_project=%name%
    set name_lib_core_final=%name_lib_core%".lib"
    set name_lib_c_final=%name_lib_c%".lib"
    set name_lib_cpp_final=%name_lib_cpp%".lib"
    set name_lib_glsl_final=%name_lib_glsl%".lib"
    set name_lib_hlsl_final=%name_lib_hlsl%".lib"
    set name_lib_msl_final=%name_lib_msl%".lib"
    set name_lib_reflect_final=%name_lib_reflect%".lib"
    set name_lib_util_final=%name_lib_util%".lib"
    set name_exe_final=%name_exe%".exe"
)

@rem build folder
set build_folder="..\Build\Windows\"%name_project%
if exist %build_folder% (
    if "%rebuild%" == "rebuild" (
        rmdir /S/Q %build_folder%
    )
)
mkdir %build_folder%


@rem Lib folder/file
set plugins_folder="..\Lib\Windows"
if not exist %plugins_folder% (
    mkdir %plugins_folder%
)


cd ..
cd Build
cd Windows
cd %name_project%

if "%debug%" == "debug" (
    cmake -DDEBUG=1 -DSPIRV_CROSS_ENABLE_TESTS=OFF "../../../Sources/%name%/"
    msbuild spirv-cross.sln /p:configuration=debug
    copy /Y ".\Debug\%name_lib_core%d.lib" "..\..\..\Lib\Windows\"%name_lib_core_final%
    copy /Y ".\Debug\%name_lib_c%d.lib" "..\..\..\Lib\Windows\"%name_lib_c_final%
    copy /Y ".\Debug\%name_lib_cpp%d.lib" "..\..\..\Lib\Windows\"%name_lib_cpp_final%
    copy /Y ".\Debug\%name_lib_glsl%d.lib" "..\..\..\Lib\Windows\"%name_lib_glsl_final%
    copy /Y ".\Debug\%name_lib_hlsl%d.lib" "..\..\..\Lib\Windows\"%name_lib_hlsl_final%
    copy /Y ".\Debug\%name_lib_msl%d.lib" "..\..\..\Lib\Windows\"%name_lib_msl_final%
    copy /Y ".\Debug\%name_lib_reflect%d.lib" "..\..\..\Lib\Windows\"%name_lib_reflect_final%
    copy /Y ".\Debug\%name_lib_util%d.lib" "..\..\..\Lib\Windows\"%name_lib_util_final%
    copy /Y ".\Debug\%name_exe%.exe" "..\..\..\..\Bin\Windows\"%name_exe_final%
) else (
    cmake -DSPIRV_CROSS_ENABLE_TESTS=OFF "../../../Sources/%name%/"
    msbuild spirv-cross.sln /p:configuration=release
    copy /Y ".\Release\%name_lib_core%.lib" "..\..\..\Lib\Windows\"%name_lib_core_final%
    copy /Y ".\Release\%name_lib_c%.lib" "..\..\..\Lib\Windows\"%name_lib_c_final%
    copy /Y ".\Release\%name_lib_cpp%.lib" "..\..\..\Lib\Windows\"%name_lib_cpp_final%
    copy /Y ".\Release\%name_lib_glsl%.lib" "..\..\..\Lib\Windows\"%name_lib_glsl_final%
    copy /Y ".\Release\%name_lib_hlsl%.lib" "..\..\..\Lib\Windows\"%name_lib_hlsl_final%
    copy /Y ".\Release\%name_lib_msl%.lib" "..\..\..\Lib\Windows\"%name_lib_msl_final%
    copy /Y ".\Release\%name_lib_reflect%.lib" "..\..\..\Lib\Windows\"%name_lib_reflect_final%
    copy /Y ".\Release\%name_lib_util%.lib" "..\..\..\Lib\Windows\"%name_lib_util_final%
    copy /Y ".\Release\%name_exe%.exe" "..\..\..\..\Bin\Windows\"%name_exe_final%
)


cd ..
cd ..
cd ..
cd Compile

set include_folder="..\Include\Windows\"%name%
if exist %include_folder% (
    rmdir /S/Q %include_folder%
)
mkdir %include_folder%

xcopy /Y /F "..\Sources\%name%\*.h" %include_folder%"\"
xcopy /Y /F "..\Sources\%name%\*.hpp" %include_folder%"\"