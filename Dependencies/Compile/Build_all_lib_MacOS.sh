# author: LostPeter
# time:   2022-11-05

debug=${1}

./Build_glm_lib_MacOS.sh
./Build_spdlog_lib_MacOS.sh
./Build_stb_lib_MacOS.sh

./Build_zlib_lib_MacOS.sh $debug
./Build_glfw_lib_MacOS.sh $debug
./Build_imgui_lib_MacOS.sh glfw_vulkan $debug
./Build_assimp_lib_MacOS.sh $debug