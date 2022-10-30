# author: LostPeter
# time:   2022-10-30

name=tinyobjloader-2.0.0

folderSrc="../Sources/$name"
folderDst="../Include/MacOS/"$name
rm -rf $folderDst
mkdir -p $folderDst

for file in ${folderSrc}/*.h
do 
    cp -rf $file $folderDst
done