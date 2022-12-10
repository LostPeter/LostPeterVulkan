# author: LostPeter
# time:   2022-12-03

name=spdlog-1.10.0

folderSrc="../Sources/$name/include/spdlog"
folderDst="../Include/MacOS/"$name
rm -rf $folderDst
mkdir -p $folderDst

cp -r $folderSrc $folderDst