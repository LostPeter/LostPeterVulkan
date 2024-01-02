@REM #****************************************************************************
@REM # LostPeterVulkan - Copyright (C) 2022 by LostPeter
@REM # 
@REM # Author:   LostPeter
@REM # Time:     2024-01-01
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

set name="zziplib-0.13.72"
set namewrap="zzipwraplib-0.13.72"
if "%debug%" == "debug" (
    set name_project=%name%"_d"
    set name_lib=%name%"_d.lib"
    set name_lib_wrap=%namewrap%"_d.lib"
) else (
    set name_project=%name%
    set name_lib=%name%".lib"
    set name_lib_wrap=%namewrap%".lib"
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
set plugins_libfile=%plugins_folder%"\"%name_lib%
if exist %plugins_libfile% (
    del /S/Q %plugins_libfile%
)
set plugins_libwrapfile=%plugins_folder%"\"%name_lib_wrap%
if exist %plugins_libwrapfile% (
    del /S/Q %plugins_libwrapfile%
)

set dirCur=%~dp0
set dirZlibInclude=%dirCur%..\Include\Windows\zlib-1.2.11
set dirDependencyLib_d=%dirCur%..\Lib\Windows\zlib-1.2.11_d.lib
set dirDependencyLib=%dirCur%..\Lib\Windows\zlib-1.2.11.lib

echo %dirCur%
echo %dirZlibInclude%
echo %dirDependencyLib_d%
echo %dirDependencyLib%


cd ..
cd Build
cd Windows
cd %name_project%

if "%debug%" == "debug" (
    cmake -DDEBUG=1 -DZLIB_INCLUDE_DIR=%dirZlibInclude% -DZLIB_LIBRARY=%dirDependencyLib_d% "../../../Sources/%name%/"
    msbuild zziplib.sln /p:configuration=debug
    copy /Y ".\zzip\Debug\zzip.lib" "..\..\..\Lib\Windows\"%name_lib%
    copy /Y ".\zzipwrap\Debug\zzipwrap.lib" "..\..\..\Lib\Windows\"%name_lib_wrap%
) else (
    cmake -DZLIB_INCLUDE_DIR=%dirZlibInclude% -DZLIB_LIBRARY=%dirDependencyLib% "../../../Sources/%name%/"
    msbuild zziplib.sln /p:configuration=release
    copy /Y ".\zzip\Release\zzip-0.lib" "..\..\..\Lib\Windows\"%name_lib%
    copy /Y ".\zzipwrap\Release\zzipwrap-0.lib" "..\..\..\Lib\Windows\"%name_lib_wrap%
)


cd ..
cd ..
cd ..
cd Compile

set include_folder="..\Include\Windows\"%name%"\zzip"
if exist %include_folder% (
    rmdir /S/Q %include_folder%
)
mkdir %include_folder%
xcopy /S /E /Y /F "..\Sources\%name%\zzip\*.h" %include_folder%"\"


set include_folder_wrap="..\Include\Windows\"%name%"\zzipwrap"
if exist %include_folder_wrap% (
    rmdir /S/Q %include_folder_wrap%
)
mkdir %include_folder_wrap%
xcopy /S /E /Y /F "..\Sources\%name%\zzipwrap\*.h" %include_folder_wrap%"\"