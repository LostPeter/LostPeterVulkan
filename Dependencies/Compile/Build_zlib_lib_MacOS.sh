# author: LostPeter
# time:   2022-11-21

debug=${1}

name=zlib-1.2.11
if [ "$debug" == "debug" ]; then
    name_project=$name"_d"
    name_lib="lib"$name"_d.a"
else
    name_project=$name
    name_lib="lib"$name".a"
fi

rm -rf "../Build/MacOS/"$name_project
mkdir -p "../Build/MacOS/"$name_project

rm -rf "../Lib/MacOS/"$name_lib
mkdir -p "../Lib/MacOS"

cd ..
cd Build
cd MacOS
cd $name_project

#lib
if [ "$debug" == "debug" ]; then
    cmake -DDEBUG=1 -DCMAKE_BUILD_TYPE=Debug ../../../Sources/$name/
else
    cmake ../../../Sources/$name/
fi
make

cd ..
cd ..
cd ..
cd Compile

cp -rf "../Build/MacOS/"$name_project"/libz.a" "../Lib/MacOS/"$name_lib


folderSrc="../Sources/$name"
folderDst="../Include/MacOS/"$name
rm -rf $folderDst
mkdir -p $folderDst

for file in ${folderSrc}/*.h
do 
    cp -rf $file $folderDst
done
cp -rf "../Build/MacOS/"$name_project"/zconf.h" $folderDst

