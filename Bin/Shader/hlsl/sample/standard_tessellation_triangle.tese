/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-03-19
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


[[vk::binding(5)]]cbuffer tessellationConsts        : register(b5)
{
    TessellationConstants tessellationConsts[MAX_OBJECT_COUNT];
}


struct HSOutput
{
    float4 outPosition                              : SV_POSITION; //xyz: Object Pos; w: instanceIndex
    [[vk::location(0)]] float4 outColor             : COLOR0;
    [[vk::location(1)]] float3 outNormal            : NORMAL0;
    [[vk::location(2)]] float2 outTexCoord          : TEXCOORD0;
};


struct ConstantsHSOutput
{
    float tessLevelOuter[3]                         : SV_TessFactor;
    float tessLevelInner                            : SV_InsideTessFactor;
};

struct DSOutput
{
    float4 outPosition                              : SV_POSITION;
    [[vk::location(0)]] float4 outWorldPos          : POSITION0; //xyz: World Pos; w: instanceIndex
    [[vk::location(1)]] float4 outColor             : COLOR0;
    [[vk::location(2)]] float3 outWorldNormal       : NORMAL0;
    [[vk::location(3)]] float2 outTexCoord          : TEXCOORD0;
};


[domain("tri")]
DSOutput main(ConstantsHSOutput input, 
              float3 uvw                            : SV_DomainLocation, 
              const OutputPatch<HSOutput, 3> patch,
              uint viewIndex : SV_ViewID)
{
    DSOutput output = (DSOutput)0;

    TransformConstants trans = passConsts.g_Transforms[viewIndex];
    uint instanceIndex = patch[0].outPosition.w;
    ObjectConstants obj = objectConsts[instanceIndex];
    output.outPosition.xyz = uvw.x * patch[0].outPosition.xyz +
                             uvw.y * patch[1].outPosition.xyz +
                             uvw.z * patch[2].outPosition.xyz;
    output.outPosition.w = 1.0;
    output.outWorldPos = mul(obj.g_MatWorld, output.outPosition);
    output.outPosition = mul(trans.mat4Proj, mul(trans.mat4View, output.outWorldPos));
    output.outWorldPos.xyz /= output.outWorldPos.w;
    output.outWorldPos.w = instanceIndex;
    output.outColor = uvw.x * patch[0].outColor + 
                      uvw.y * patch[1].outColor + 
                      uvw.z * patch[2].outColor;
    float3 outNormal = uvw.x * patch[0].outNormal + 
                       uvw.y * patch[1].outNormal + 
                       uvw.z * patch[2].outNormal;
    output.outWorldNormal = mul((float3x3)obj.g_MatWorld, outNormal);
    output.outTexCoord = uvw.x * patch[0].outTexCoord + 
                         uvw.y * patch[1].outTexCoord + 
                         uvw.z * patch[2].outTexCoord;

	return output;
}