@REM #****************************************************************************
@REM # LostPeterVulkan - Copyright (C) 2022 by LostPeter
@REM # 
@REM # Author: LostPeter
@REM # Time:   2022-10-30
@REM #
@REM # This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
@REM #****************************************************************************/

@echo off
set name_sample=%1
set debug=%2
set rebuild=%3

echo %name_sample%
echo %debug%
echo %rebuild%

if "%debug%" == "debug" (
    set name_project="share_"%name_sample%"_d"
    set name_lib="share_"%name_sample%"_d.lib"
) else (
    set name_project="share_"%name_sample%
    set name_lib="share"%name_sample%".lib"
)

echo %name_project%
echo %debug%

@rem build folder
set build_folder="..\Build\Windows\"%name_project%
if exist %build_folder% (
    if "%rebuild%" == "rebuild" (
        rmdir /S/Q %build_folder%
    )
)
mkdir %build_folder%


@rem lib folder/file
set lib_folder="..\Lib\Windows"
if not exist %lib_folder% (
    mkdir %lib_folder%
)
set lib_file="..\Lib\Windows\"%name_lib%
if exist %lib_file% (
    del /S/Q %lib_file%
)


cd ..
cd Build
cd Windows
cd %name_project%

if "%debug%" == "debug" (
    cmake -DDEBUG=1 "../../../%name_sample%/share/"
    msbuild "%name_project%".sln /p:configuration=debug
    copy /Y ".\Debug\"%name_lib% "..\..\..\Lib\Windows\"%name_lib%
) else (
    cmake "../../../%name_sample%/share/"
    msbuild "%name_project%".sln /p:configuration=release
    copy /Y ".\Release\"%name_lib% "..\..\..\Lib\Windows\"%name_lib%
)


cd ..
cd ..
cd ..
cd Compile

echo over