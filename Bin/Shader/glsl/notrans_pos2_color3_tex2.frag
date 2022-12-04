//author: LostPeter
//time:   2022-11-13

#version 450

layout(binding = 4) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() 
{
    vec3 color = texture(texSampler, fragTexCoord).xyz * fragColor;
    outColor = vec4(color, 1.0);
}