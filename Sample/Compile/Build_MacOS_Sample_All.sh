# author: LostPeter
# time:   2022-10-30

debug=${1}
rebuild=${2}

./Build_MacOS_Sample.sh vulkan_000_window $debug $rebuild
./Build_MacOS_Sample.sh vulkan_001_triangle $debug $rebuild
./Build_MacOS_Sample.sh vulkan_002_imgui $debug $rebuild
./Build_MacOS_Sample.sh vulkan_003_texture $debug $rebuild
./Build_MacOS_Sample.sh vulkan_004_model $debug $rebuild
./Build_MacOS_Sample.sh vulkan_005_camera $debug $rebuild
./Build_MacOS_Sample.sh vulkan_006_depth $debug $rebuild
./Build_MacOS_Sample.sh vulkan_007_stencil $debug $rebuild
./Build_MacOS_Sample.sh vulkan_008_blend $debug $rebuild
./Build_MacOS_Sample.sh vulkan_009_instancing $debug $rebuild
./Build_MacOS_Sample.sh vulkan_010_lighting $debug $rebuild