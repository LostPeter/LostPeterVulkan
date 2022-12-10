@REM author: LostPeter
@REM time:   2022-12-03

set name="spdlog-1.10.0"

set include_folder="..\Include\Windows\"%name%
if exist %include_folder% (
    rmdir /S/Q %include_folder%
)
mkdir %include_folder%

xcopy /S /E /Y /F "..\Sources\%name%\include\*.h" %include_folder%"\"