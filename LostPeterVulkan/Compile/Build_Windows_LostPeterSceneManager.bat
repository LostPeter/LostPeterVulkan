@REM #****************************************************************************
@REM # LostPeterSceneManager - Copyright (C) 2022 by LostPeter
@REM # 
@REM # Author:   LostPeter
@REM # Time:     2023-06-12
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


@REM 1) Build LostPeterFoundation
call ./Build_Windows_LostPeterFoundation.bat %debug% %rebuild%


@REM #2) Build LostPeterSceneManager
if "%debug%" == "debug" (
    set name_project="LostPeterSceneManager_d"
    set name_lib="LostPeterSceneManager_d.lib"
    @REM set name_dll=%name%"_"%mode%"_d.dll"
) else (
    set name_project="LostPeterSceneManager"
    set name_lib="LostPeterSceneManager.lib"
    @REM set name_dll=%name%"_"%mode%".dll"
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
set plugins_folder="../../Plugins/Windows"
if not exist %plugins_folder% (
    mkdir %plugins_folder%
)
set plugins_libfile=%plugins_folder%"/"%name_lib%
if exist %plugins_libfile% (
    del /S/Q %plugins_libfile%
)


cd ..
cd Build
cd Windows
cd %name_project%

if "%debug%" == "debug" (
    cmake -DDEBUG=1 "../../../LostPeterSceneManager/"
    msbuild "%name_project%".sln /p:configuration=debug
    copy /Y ".\Debug\"%name_lib% "..\..\..\..\Plugins\Windows\"%name_lib%
    @REM copy /Y ".\Debug\"%name_dll% "..\..\..\..\Bin\Windows\"%name_dll%
) else (
    cmake "../../../LostPeterSceneManager/"
    msbuild "%name_project%".sln /p:configuration=release
    copy /Y ".\Release\"%name_lib% "..\..\..\..\Plugins\Windows\"%name_lib%
    @REM copy /Y ".\Release\"%name_dll% "..\..\..\..\Bin\Windows\"%name_dll%
)


cd ..
cd ..
cd ..
cd Compile

@echo over