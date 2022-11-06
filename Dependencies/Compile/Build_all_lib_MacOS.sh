# author: LostPeter
# time:   2022-11-05

debug=${1}

./Build_glfw_lib_MacOS.sh $debug
./Build_glm_lib_MacOS.sh
./Build_stb_lib_MacOS.sh
./Build_tinyobjloader_lib_MacOS.sh