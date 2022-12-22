//author: LostPeter
//time:   2022-11-13

#version 450

layout(location = 0) in vec4 fragColor;

layout(location = 0) out vec4 outColor;

void main()
{
    outColor = vec4(fragColor.xyz, 1.0);
}
