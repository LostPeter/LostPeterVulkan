# author: LostPeter
# time:   2022-11-13

debug=${1}

name_folder="hlsl"
dirSrc="./$name_folder"
dirDst="../Assets/Shader"

rm -rf $dirDst
mkdir -p $dirDst

function read_dir() {
    for file in `ls $1`
    do 
        if [ -d $1"/"$file ]; then
            read_dir $1"/"$file 
        else
            echo $1"/"$file 
        fi
    done
}

function compile_shader() {
    for file in `ls $1`
    do 
        if [ -d $1"/"$file ]; then
            compile_shader $1"/"$file 
        else
            ./Build_Shader_HLSL_MacOS.sh $file $debug
        fi
    done
}

echo "************** Shader Source .vert/.frag/.comp/.geom/.tesc/.tese **************"
# read_dir $dirSrc
{
    compile_shader $dirSrc
}
echo "************** Shader Compile .spv ********************"
read_dir $dirDst