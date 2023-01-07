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


@REM 1) Build libshare
call ./Build_Windows_Sample_Share.bat %name_sample% %debug% %rebuild%


@REM #2) Build Sample
if "%debug%" == "debug" (
    set name_project=%name_sample%"_d"
) else (
    set name_project=%name_sample%
)
set name_exe=%name_project%".exe"

@rem build folder
set build_folder="..\Build\Windows\"%name_project%
if exist %build_folder% (
    if "%rebuild%" == "rebuild" (
        rmdir /S/Q %build_folder%
    )
)
mkdir %build_folder%

@rem Bin folder/file
set bin_folder="..\..\Bin\Windows"
if not exist %bin_folder% (
    mkdir %bin_folder%
)
set bin_file=%bin_folder%"\"%name_exe%
if exist %bin_file% (
    del /S/Q %bin_file%
)


cd ..
cd Build
cd Windows
cd %name_project%

if "%debug%" == "debug" (
    cmake -DDEBUG=1 "../../../"%name_sample%"/pc/"
    msbuild "%name_project%".sln /p:configuration=debug
    copy /Y ".\Debug\"%name_exe% "..\..\..\..\Bin\Windows\"%name_exe%
) else (
    cmake "../../../"%name_sample%"/pc/"
    msbuild "%name_project%".sln /p:configuration=release
    copy /Y ".\Release\"%name_exe% "..\..\..\..\Bin\Windows\"%name_exe%
)


cd ..
cd ..
cd ..
cd Compile

@echo over