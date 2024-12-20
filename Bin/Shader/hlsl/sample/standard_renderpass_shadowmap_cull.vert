/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-09-28
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


[[vk::binding(1)]]cbuffer instanceConst             : register(b1) 
{
    CullInstanceConstants instanceConst;
}

[[vk::binding(2)]] StructuredBuffer<CullObjectInstanceConstants> instanceCB	: register(t0);
[[vk::binding(3)]] StructuredBuffer<uint> resultCB	                        : register(t1);

struct VSOutput
{
	float4 outPosition : SV_POSITION;
};


VSOutput main(VSInput_Pos3Color4Normal3TexCood2 input,
              uint instanceIndex : SV_InstanceID)
{
    VSOutput output = (VSOutput)0;

    uint index = resultCB[instanceConst.nObjectOffset + instanceIndex];
    CullObjectInstanceConstants obj = instanceCB[index];

    output.outPosition = mul(passConsts.g_MainLight.depthMVP, mul(obj.mat4Object2World, float4(input.inPosition, 1.0)));

    return output;
}