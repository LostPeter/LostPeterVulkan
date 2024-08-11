/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-02-18
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


[[vk::binding(2)]]cbuffer materialConsts            : register(b2) 
{
    MaterialConstants materialConsts[MAX_MATERIAL_COUNT];
}


[[vk::binding(3)]]cbuffer instanceConsts            : register(b3) 
{
    InstanceConstants instanceConsts[MAX_INSTANCE_COUNT];
}


struct VSOutput
{
	float4 outPosition                            : SV_POSITION; //xyz: Object Pos; w: instanceIndex
    [[vk::location(0)]] float4 outColor           : COLOR0;
    [[vk::location(1)]] float3 outNormal          : NORMAL0;
    [[vk::location(2)]] float2 outTexCoord        : TEXCOORD0;
};


VSOutput main(VSInput_Pos3Color4Normal3TexCood2 input, 
              uint viewIndex : SV_ViewID,
              uint instanceIndex : SV_InstanceID)
{
    VSOutput output = (VSOutput)0;
    output.outPosition = float4(input.inPosition.xyz, instanceIndex);
    output.outColor = input.inColor;
    output.outNormal = input.inNormal;
    output.outTexCoord = input.inTexCoord;

    return output;
}