# author: LostPeter
# time:   2022-11-05

debug=${1}

./Build_assimp_lib_MacOS.sh $debug
./Build_glfw_lib_MacOS.sh $debug
./Build_glm_lib_MacOS.sh
./Build_imgui_lib_MacOS.sh glfw_vulkan $debug
./Build_stb_lib_MacOS.sh
./Build_zlib_lib_MacOS.sh $debug