/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-24
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

struct VSOutput
{
    [[vk::location(0)]] float4 inColor          : COLOR0;
    [[vk::location(1)]] float2 inTexCoord       : TEXCOORD0;
};

[[vk::binding(1)]] Texture2D texture2D            : register(t1);
[[vk::binding(1)]] SamplerState texture2DSampler  : register(s1);

float4 main(VSOutput input) : SV_TARGET
{
    float4 outColor = texture2D.Sample(texture2DSampler, input.inTexCoord).rgba;
    outColor.xyz *= input.inColor.xyz;
    
    return outColor;
}