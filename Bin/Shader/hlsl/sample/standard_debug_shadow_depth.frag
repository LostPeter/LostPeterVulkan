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

#include "../hlsl_input.hlsl"
#include "../hlsl_common.hlsl"


[[vk::binding(4)]] Texture2D textureDepth               : register(t1);
[[vk::binding(4)]] SamplerState textureDepthSampler     : register(s1);


float4 main(VSInput_Pos4Color4Normal3TexCood2 input) : SV_TARGET
{
    float shadow = textureDepth.Sample(textureDepthSampler, input.inTexCoord).r;
    return float4(shadow, shadow, shadow, 1.0);
}