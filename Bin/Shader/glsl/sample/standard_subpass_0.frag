/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-03-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in VSOutput
{
    vec4 inColor;
    vec2 inTexCoord;
} ps_in;

layout(binding = 4) uniform sampler2D texSampler;

layout(location = 1) out vec4 oColor1;
layout(location = 2) out vec4 oColor2;
layout(location = 3) out vec4 oColor3;

void main()
{
    vec4 outColor = texture(texSampler, ps_in.inTexCoord);
    outColor.xyz *= ps_in.inColor.rgb;
    
    oColor1 = vec4(outColor.x, 0.0f, 0.0f, 1.0f);
    oColor2 = vec4(0.0f, outColor.y, 0.0f, 1.0f);
    oColor3 = vec4(0.0f, 0.0f, outColor.z, 1.0f);
}