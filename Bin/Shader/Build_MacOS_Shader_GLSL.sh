# author: LostPeter
# time:   2022-11-13

name_shader=${1}

name_folder="glsl"
folderSrc="./$name_folder"
folderShader="../Assets/Shader"
mkdir -p $folderShader

glslangValidator -V $folderSrc/$name_shader -o $folderShader/$name_shader.spv