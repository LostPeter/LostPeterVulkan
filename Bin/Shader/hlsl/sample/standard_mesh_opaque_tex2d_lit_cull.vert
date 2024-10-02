/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-09-30
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../hlsl_input.hlsl"
#include "../hlsl_common.hlsl"


[[vk::binding(0)]]cbuffer passConsts              : register(b0) 
{
    PassConstants passConsts;
}


[[vk::binding(1)]]cbuffer objectConsts            : register(b1) 
{
    ObjectConstants objectConsts[MAX_OBJECT_COUNT];
}


[[vk::binding(2)]]cbuffer materialConsts          : register(b2) 
{
    MaterialConstants materialConsts[MAX_MATERIAL_COUNT];
}


[[vk::binding(3)]]cbuffer instanceConst           : register(b3) 
{
    CullInstanceConstants instanceConst;
}


[[vk::binding(5)]] StructuredBuffer<CullObjectInstanceConstants> instanceCB	: register(t0);
[[vk::binding(6)]] StructuredBuffer<uint> resultCB	                        : register(t1);


VSOutput_Pos4Color4Normal3TexCood2 main(VSInput_Pos3Color4Normal3TexCood2 input, 
                                        uint viewIndex : SV_ViewID,
                                        uint instanceIndex : SV_InstanceID)
{
    VSOutput_Pos4Color4Normal3TexCood2 output = (VSOutput_Pos4Color4Normal3TexCood2)0;

    TransformConstants trans = passConsts.g_Transforms[viewIndex];
    uint index = resultCB[instanceConst.nObjectOffset + instanceIndex];
    CullObjectInstanceConstants obj = instanceCB[index];

    output.outWorldPos = mul(obj.mat4Object2World, float4(input.inPosition, 1.0));
    output.outPosition = mul(trans.mat4Proj, mul(trans.mat4View, output.outWorldPos));
    output.outWorldPos.xyz /= output.outWorldPos.w;
    output.outWorldPos.w = instanceIndex;
    output.outColor = input.inColor;
    output.outWorldNormal = mul((float3x3)obj.mat4Object2World, input.inNormal);
    output.outTexCoord = input.inTexCoord;

    return output;
}