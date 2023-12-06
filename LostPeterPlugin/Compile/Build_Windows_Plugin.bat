@REM #****************************************************************************
@REM # LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
@REM # 
@REM # Author:   LostPeter
@REM # Time:     2023-11-19
@REM # Github:   https://github.com/LostPeter/LostPeterVulkan
@REM # Document: https://www.zhihu.com/people/lostpeter/posts
@REM #
@REM # This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
@REM #****************************************************************************/

@echo off
set name_plugin=%1
set debug=%2
set rebuild=%3

echo %name_plugin%
echo %debug%
echo %rebuild%


@REM # Build LostPeterPluginRendererVulkan
if "%debug%" == "debug" (
    set name_project=%name_plugin%"_d"
    set name_dll=%name_plugin%"_d.dll"
) else (
    set name_project=%name_plugin%
    set name_dll=%name_plugin%".dll"
)

@rem build folder
set build_folder="../Build/Windows/"%name_project%
if exist %build_folder% (
    if "%rebuild%" == "rebuild" (
        rmdir /S/Q %build_folder%
    )
)
mkdir %build_folder%


@rem Plugins folder/file
set plugins_folder="../../Bin/Assets/Plugins/Windows"
if not exist %plugins_folder% (
    mkdir %plugins_folder%
)
set plugins_libfile=%plugins_folder%"/"%name_dll%
if exist %plugins_libfile% (
    del /S/Q %plugins_libfile%
)


cd ..
cd Build
cd Windows
cd %name_project%

if "%debug%" == "debug" (
    cmake -DDEBUG=1 "../../../%name_plugin%/"
    msbuild "%name_project%".sln /p:configuration=debug
    copy /Y ".\Debug\"%name_dll% "..\..\..\..\Bin\Assets\Plugins\Windows\"%name_dll%
) else (
    cmake "../../../%name_plugin%/"
    msbuild "%name_project%".sln /p:configuration=release
    copy /Y ".\Release\"%name_dll% "..\..\..\..\Bin\Assets\Plugins\Windows\"%name_dll%
)


cd ..
cd ..
cd ..
cd Compile

@echo over