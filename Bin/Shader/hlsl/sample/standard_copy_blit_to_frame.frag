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

#include "../hlsl_input.hlsl"
#include "../hlsl_common.hlsl"


[[vk::binding(1)]] Texture2D texture2D            : register(t1);
[[vk::binding(1)]] SamplerState texture2DSampler  : register(s1);


[[vk::binding(0)]]cbuffer copyBlitObjectConsts          : register(b0) 
{
    CopyBlitObjectConstants copyBlitObjectConsts;
}


float4 main(VSInput_Color4TexCood2 input) : SV_TARGET
{
    float4 outColor = texture2D.Sample(texture2DSampler, input.inTexCoord).rgba;
    outColor.xyz *= input.inColor.xyz;
    
    return outColor;
}