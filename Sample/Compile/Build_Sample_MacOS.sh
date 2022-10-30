# author: LostPeter
# time:   2022-10-30

name_sample=${1}
debug=${2}

if [ "$debug" == "debug" ]; then
    name_project=$name_sample"_d"
else
    name_project=$name_sample
fi

#1> Build libShare
cd ..
cd $name_sample
cd share
cd Compile

sh ./Build_Share_MacOS.sh $debug

cd ..
cd ..
cd ..
cd Compile


#2> Build Sample
rm -rf "../Build/MacOS/"$name_project
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