@REM author: LostPeter
@REM time:   2022-12-10

set debug=%1
set rebuild=%2
set depends=%3


@REM 1> Dependencies
if "%depends%" == "depends" (
    cd Dependencies
    cd Compile

    call ./Build_Windows_lib_all.bat %debug% %rebuild%

    cd ..
    cd ..
)


@REM 2> Shaders
cd Bin
cd Shader

call ./Build_Windows_Shader_HLSL_All.bat %debug% 

cd ..
cd ..


@REM 3> VulkanUtil
cd VulkanUtil
cd Compile

call ./Build_Windows_VulkanUtil.bat %debug% %rebuild%

cd ..
cd ..


@REM 4> Sample
cd Sample
cd Compile

call ./Build_Windows_Sample_All.bat %debug% %rebuild%

cd ..
cd ..