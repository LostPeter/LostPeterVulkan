@REM author: LostPeter
@REM time:   2022-11-21

@echo off
set debug=%1
set rebuild=%2

echo %debug%
echo %rebuild%

set name="zlib-1.2.11"
if "%debug%" == "debug" (
    set name_project=%name%"_d"
    set name_lib=%name%"_d.lib"
) else (
    set name_project=%name%
    set name_lib=%name%".lib"
)

@rem build folder
set build_folder="..\Build\Windows\"%name_project%
if exist %build_folder% (
    if "%rebuild%" == "rebuild" (
        rmdir /S/Q %build_folder%
    )
)
mkdir %build_folder%


@rem Plugins folder/file
set plugins_folder="..\Lib\Windows"
if not exist %plugins_folder% (
    mkdir %plugins_folder%
)
set plugins_libfile=%plugins_folder%"\"%name_lib%
if exist %plugins_libfile% (
    del /S/Q %plugins_libfile%
)


cd ..
cd Build
cd Windows
cd %name_project%

if "%debug%" == "debug" (
    cmake -DDEBUG=1 "../../../Sources/%name%/"
    msbuild zlib.sln /p:configuration=debug
    copy /Y ".\Debug\zlibstaticd.lib" "..\..\..\Lib\Windows\"%name_lib%
) else (
    cmake "../../../Sources/%name%/"
    msbuild zlib.sln /p:configuration=release
    copy /Y ".\Release\zlibstatic.lib" "..\..\..\Lib\Windows\"%name_lib%
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
xcopy /Y /F "%build_folder%\zconf.h" %include_folder%"\"