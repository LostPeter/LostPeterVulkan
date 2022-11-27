# author: LostPeter
# time:   2022-11-21

debug=${1}

if [ "$debug" == "debug" ]; then
    name_project="Share_d"
    name_dylib="libShare_d.dylib"
else
    name_project="Share"
    name_dylib="libShare.dylib"
fi

rm -rf "../Build/MacOS/"$name_project
mkdir -p "../Build/MacOS/"$name_project

rm -rf "../Lib/MacOS/"$name_dylib
mkdir -p "../Lib/MacOS"

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

cp -rf "../Build/MacOS/"$name_project/$name_dylib "../Lib/MacOS/"$name_dylib