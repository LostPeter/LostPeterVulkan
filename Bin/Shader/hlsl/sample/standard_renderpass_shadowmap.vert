/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-06-23
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


[[vk::binding(1)]]cbuffer objectConsts              : register(b1) 
{
    ObjectConstants objectConsts[MAX_OBJECT_COUNT];
}


struct VSOutput
{
	float4 outPosition : SV_POSITION;
};


VSOutput main(VSInput_Pos3Color4Normal3TexCood2 input,
              uint instanceIndex : SV_InstanceID)
{
    VSOutput output = (VSOutput)0;

    ObjectConstants obj = objectConsts[instanceIndex];
    output.outPosition = mul(passConsts.g_MainLight.depthMVP, mul(obj.g_MatWorld, float4(input.inPosition, 1.0)));

    return output;
}