# author: LostPeter
# time:   2022-10-30

name=glm-0.9.9.8

folderSrc="../Sources/$name/include/glm"
folderDst="../Include/MacOS/"$name
rm -rf $folderDst
mkdir -p $folderDst

cp -pr $folderSrc $folderDst