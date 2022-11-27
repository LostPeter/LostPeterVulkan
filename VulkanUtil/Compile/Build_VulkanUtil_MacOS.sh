# author: LostPeter
# time:   2022-10-30

debug=${1}

if [ "$debug" == "debug" ]; then
    name_project="VulkanUtil_d"
    name_dylib="libVulkanUtil_d.dylib"
else
    name_project="VulkanUtil"
    name_dylib="libVulkanUtil.dylib"
fi

# rm -rf "../Build/MacOS/"$name_project
mkdir -p "../Build/MacOS/"$name_project

rm -rf "../../Plugins/MacOS/"$name_dylib
mkdir -p "../../Plugins/MacOS"

cd ..
cd Build
cd MacOS
cd $name_project

#dylib
if [ "$debug" == "debug" ]; then
    cmake -DDEBUG=1 -DCMAKE_BUILD_TYPE=Debug ../../../Core/
else
    cmake ../../../Core/
fi
make

cd ..
cd ..
cd ..
cd Compile

cp -rf "../Build/MacOS/"$name_project/$name_dylib "../../Plugins/MacOS/"$name_dylib