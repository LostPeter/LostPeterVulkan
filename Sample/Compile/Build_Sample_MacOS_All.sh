# author: LostPeter
# time:   2022-10-30

debug=${1}

./Build_Sample_MacOS.sh vulkan_000_window $debug
./Build_Sample_MacOS.sh vulkan_001_triangle $debug
./Build_Sample_MacOS.sh vulkan_002_imgui $debug
./Build_Sample_MacOS.sh vulkan_003_texture $debug