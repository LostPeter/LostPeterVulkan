/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2022-11-13
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

struct VSOutput
{
    [[vk::location(0)]] float4 inColor          : COLOR0;
    [[vk::location(1)]] float2 inTexCoord       : TEXCOORD0;
};

[[vk::binding(4)]] Texture2D texture            : register(t1);
[[vk::binding(4)]] SamplerState textureSampler  : register(s1);

float4 main(VSOutput input) : SV_TARGET
{
    float3 outColor = texture.Sample(textureSampler, input.inTexCoord).rgb;
    outColor.xyz *= input.inColor.xyz;  

    return float4(outColor, 1.0);
}