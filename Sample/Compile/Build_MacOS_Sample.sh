#****************************************************************************
# LostPeterVulkan - Copyright (C) 2022 by LostPeter
# 
# Author: LostPeter
# Time:   2022-10-30
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

name_sample=${1}
debug=${2}
rebuild=${3}

if [ "$debug" == "debug" ]; then
    name_project=$name_sample"_d"
else
    name_project=$name_sample
fi

#1> Build libshare
sh ./Build_MacOS_Sample_Share.sh $name_sample $debug $rebuild


#2> Build Sample
if [ "$rebuild" == "rebuild" ]; then
    rm -rf "../Build/MacOS/"$name_project
fi
mkdir -p "../Build/MacOS/"$name_project

rm -rf "../../Bin/MacOS/"$name_project
mkdir -p "../../Bin/MacOS"

cd ..
cd Build
cd MacOS
cd $name_project

if [ "$debug" == "debug" ]; then
    cmake -DDEBUG=1 ../../../$name_sample/pc
else
    cmake ../../../$name_sample/pc
fi
make

cd ..
cd ..
cd ..
cd Compile

cp -rf "../Build/MacOS/$name_project/$name_project" "../../Bin/MacOS"