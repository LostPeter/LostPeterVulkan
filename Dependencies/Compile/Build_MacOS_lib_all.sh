# author: LostPeter
# time:   2022-11-05

debug=${1}
rebuild=${2}

./Build_MacOS_lib_glm.sh
./Build_MacOS_lib_spdlog.sh
./Build_MacOS_lib_stb.sh

./Build_MacOS_lib_zlib.sh $debug $rebuild
./Build_MacOS_lib_glfw.sh $debug $rebuild
./Build_MacOS_lib_imgui.sh glfw_vulkan $debug $rebuild
./Build_MacOS_lib_assimp.sh $debug $rebuild