@REM author: LostPeter
@REM time:   2022-11-12

@echo off
set debug=%1

if "%debug%" == "debug" (
    set name_project="Share_d"
    set name_lib="Share_d.lib"
) else (
    set name_project="Share"
    set name_lib="Share.lib"
)

echo %name_project%
echo %debug%

@rem build folder
set build_folder="..\Build\Windows\"%name_project%
if exist %build_folder% (
    rmdir /S/Q %build_folder%
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
    cmake -DDEBUG=1 "../../../Core/"
    msbuild "%name_project%".sln /p:configuration=debug
    copy /Y ".\Debug\"%name_lib% "..\..\..\Lib\Windows\"%name_lib%
) else (
    cmake "../../../Core/"
    msbuild "%name_project%".sln /p:configuration=release
    copy /Y ".\Release\"%name_lib% "..\..\..\Lib\Windows\"%name_lib%
)


cd ..
cd ..
cd ..
cd Compile

echo over