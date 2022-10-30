@REM author: LostPeter
@REM time:   2022-10-30

@echo off
set debug=%1
echo %debug%

set name="glfw-3.3.6"
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
    rmdir /S/Q %build_folder%
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
    msbuild GLFW.sln /p:configuration=debug
    copy /Y ".\src\Debug\glfw3.lib" "..\..\..\Lib\Windows\"%name_lib%
) else (
    cmake "../../../Sources/%name%/"
    msbuild GLFW.sln /p:configuration=release
    copy /Y ".\src\Release\glfw3.lib" "..\..\..\Lib\Windows\"%name_lib%
)


cd ..
cd ..
cd ..
cd Compile

set include_folder="..\Include\"%name%"\GLFW"
if exist %include_folder% (
    rmdir /S/Q %include_folder%
)
mkdir %include_folder%

xcopy /S /E /Y /F "..\Sources\%name%\include\GLFW\*.h" %include_folder%"\"