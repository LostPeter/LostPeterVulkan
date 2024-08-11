/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-05-20
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../hlsl_input.hlsl"
#include "../hlsl_common.hlsl"


[[vk::binding(0)]]cbuffer passConsts                : register(b0) 
{
    PassConstants passConsts;
}

[[vk::constant_id(0)]] const float VIEW_LAYER = 0.0f;

[[vk::binding(1)]] Texture2DArray textureArray       : register(t1);
[[vk::binding(1)]] SamplerState textureArraySampler  : register(s1);


float4 main(VSInput_TexCood2 input) : SV_TARGET
{
    const float alpha = passConsts.g_Pad1;

    float2 p1 = float2(2.0 * input.inTexCoord - 1.0);
	float2 p2 = p1 / (1.0 - alpha * length(p1));
	p2 = (p2 + 1.0) * 0.5;

    float4 outColor = float4(0.0, 0.0, 0.0, 1.0);
    bool isInside = ((p2.x >= 0.0) && (p2.x <= 1.0) && (p2.y >= 0.0 ) && (p2.y <= 1.0));
    if (isInside)
    {
        outColor = textureArray.Sample(textureArraySampler, float3(p2, VIEW_LAYER));
    }

    return outColor;
}