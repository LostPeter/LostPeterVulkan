@REM #****************************************************************************
@REM # LostPeterVulkan - Copyright (C) 2022 by LostPeter
@REM # 
@REM # Author: LostPeter
@REM # Time:   2023-01-07
@REM #
@REM # This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
@REM #****************************************************************************/

@echo off
set debug=%1
echo %debug%

set name="libnoise-1.0.0"
if "%debug%" == "debug" (
    set name_project=%name%"_d"
    set name_lib=%name%"_d.lib"
    set nameutil_lib="libnoiseutils-1.0.0_d.lib"
) else (
    set name_project=%name%
    set name_lib=%name%".lib"
    set nameutil_lib="libnoiseutils-1.0.0.lib"
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
    msbuild Project.sln /p:configuration=debug
    copy /Y ".\src\Debug\noise.lib" "..\..\..\Lib\Windows\"%name_lib%
    copy /Y ".\noiseutils\Debug\noiseutils-static.lib" "..\..\..\Lib\Windows\"%nameutil_lib%
) else (
    cmake "../../../Sources/%name%/"
    msbuild Project.sln /p:configuration=release
    copy /Y ".\src\Release\noise.lib" "..\..\..\Lib\Windows\"%name_lib%
    copy /Y ".\noiseutils\Release\noiseutils-static.lib" "..\..\..\Lib\Windows\"%nameutil_lib%
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
mkdir %include_folder%"\noise"
mkdir %include_folder%"\noiseutils"

xcopy /E /S /Y /F "..\Sources\%name%\src\noise" %include_folder%"\noise\"
xcopy /E /S /Y /F "..\Sources\%name%\noiseutils\*.h" %include_folder%"\noiseutils\"