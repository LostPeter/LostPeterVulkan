# author: LostPeter
# time:   2022-12-10

debug=${1}
rebuild=${2}
depends=${3}


#1> Dependencies
if [ "$depends" == "depends" ]; then
    cd Dependencies
    cd Compile

    ./Build_MacOS_lib_all.sh $debug $rebuild

    cd ..
    cd ..
fi


#2> Shaders
cd Bin
cd Shader

./Build_MacOS_Shader_HLSL_All.sh $debug

cd ..
cd ..


#3> VulkanUtil
cd VulkanUtil
cd Compile

./Build_MacOS_VulkanUtil.sh $debug $rebuild

cd ..
cd ..


#4> Sample
cd Sample
cd Compile

./Build_MacOS_Sample_All.sh $debug $rebuild

cd ..
cd ..