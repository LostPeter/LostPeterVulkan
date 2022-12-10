@REM author: LostPeter
@REM time:   2022-10-30

set name="glm-0.9.9.8"

set include_folder="..\Include\Windows\"%name%
if exist %include_folder% (
    rmdir /S/Q %include_folder%
)
mkdir %include_folder%

xcopy /S /E /Y /F "..\Sources\%name%\include\glm" %include_folder%"\glm\"