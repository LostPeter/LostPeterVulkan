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

struct VSOutput
{
    [[vk::location(0)]] float4 inColor          : COLOR0;
    [[vk::location(1)]] float2 inTexCoord       : TEXCOORD0;
};

[[vk::binding(4)]] Texture2D texture2D            : register(t1);
[[vk::binding(4)]] SamplerState texture2DSampler  : register(s1);

struct PSOutput {
  float4 oColor0 : SV_TARGET0;
  float4 oColor1 : SV_TARGET1;
  float4 oColor2 : SV_TARGET2;
  float4 oColor3 : SV_TARGET3;
};

PSOutput main(VSOutput input)
{
    float3 outColor = texture2D.Sample(texture2DSampler, input.inTexCoord).rgb;
    outColor.xyz *= input.inColor.rgb;
    
    PSOutput ret = (PSOutput)0;
    ret.oColor0 = float4(0.0f, 0.0f, 0.0f, 0.0f);
    ret.oColor1 = float4(outColor.x, 0.0f, 0.0f, 1.0f);
    ret.oColor2 = float4(0.0f, outColor.y, 0.0f, 1.0f);
    ret.oColor3 = float4(0.0f, 0.0f, outColor.z, 1.0f);
    return ret;
}