/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-07-06
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

struct VSOutput
{
    [[vk::location(0)]] float4 inWorldPos       : POSITION0; //xyz: World Pos; w: instanceIndex
    [[vk::location(1)]] float4 inColor          : COLOR0;
    [[vk::location(2)]] float3 inWorldNormal    : NORMAL0;
    [[vk::location(3)]] float2 inTexCoord       : TEXCOORD0;
};

[[vk::binding(4)]] Texture2D textureDepth               : register(t1);
[[vk::binding(4)]] SamplerState textureDepthSampler     : register(s1);


float4 main(VSOutput input) : SV_TARGET
{
    float shadow = textureDepth.Sample(textureDepthSampler, input.inTexCoord).r;
    return float4(shadow, shadow, shadow, 1.0);
}