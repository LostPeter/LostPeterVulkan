/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-11-16
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

[[vk::binding(4)]] Texture2D texture2D            : register(t1);
[[vk::binding(4)]] SamplerState texture2DSampler  : register(s1);

float4 main(VSOutput input) : SV_TARGET
{
    float3 outColor = texture2D.Sample(texture2DSampler, input.inTexCoord).rgb;
    outColor.xyz *= input.inColor.rgb;
    
    return float4(outColor, 1.0);
}