/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-10-27
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../hlsl_input.hlsl"
#include "../hlsl_common.hlsl"


[[vk::binding(0)]]cbuffer hizDepthConstants         : register(b0) 
{
    HizDepthConstants hizDepthConstants;
}


[[vk::binding(1)]] Texture2D texture2D              : register(t1);
[[vk::binding(1)]] SamplerState texture2DSampler    : register(s1);


float main(VSInput_Color4TexCood2 input) : SV_TARGET
{
    float2 offset = hizDepthConstants.vRtDepthSize.zw * 0.5;
    float x = texture2D.Sample(texture2DSampler, input.inTexCoord + offset, 0).x;
    float y = texture2D.Sample(texture2DSampler, input.inTexCoord - offset, 0).x;
    float z = texture2D.Sample(texture2DSampler, input.inTexCoord + float2(offset.x, -offset.y), 0).x;
    float w = texture2D.Sample(texture2DSampler, input.inTexCoord + float2(-offset.x, offset.y), 0).x;
    float4 readDepth = float4(x,y,z,w);
    readDepth.xy = max(readDepth.xy, readDepth.zw);
    readDepth.x = max(readDepth.x, readDepth.y);
    
    return readDepth.x;
}