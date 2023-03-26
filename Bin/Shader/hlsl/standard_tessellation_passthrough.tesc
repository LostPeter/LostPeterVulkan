/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2023-03-19
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/


#define MAX_OBJECT_COUNT 1024
struct TessellationConstants
{
    float tessLevel;
    float tessAlpha;
    float tessStrength;
};

[[vk::binding(5)]]cbuffer tessellationConsts        : register(b5)
{
    TessellationConstants tessellationConsts[MAX_OBJECT_COUNT];
}


struct VSOutput 
{
    float4 outPosition                              : SV_POSITION; //xyz: Object Pos; w: instanceIndex
    [[vk::location(0)]] float4 outColor             : COLOR0;
    [[vk::location(1)]] float3 outNormal            : NORMAL0;
    [[vk::location(2)]] float2 outTexCoord          : TEXCOORD0;
};

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


ConstantsHSOutput ConstantsHS(InputPatch<VSOutput, 3> patch, uint InvocationID : SV_PrimitiveID)
{
    ConstantsHSOutput output = (ConstantsHSOutput)0;

	output.tessLevelInner = 1;
    output.tessLevelOuter[0] = 1;
    output.tessLevelOuter[1] = 1;
    output.tessLevelOuter[2] = 1;
    
    return output;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstantsHS")]
[maxtessfactor(20.0f)]
HSOutput main(InputPatch<VSOutput, 3> patch, uint InvocationID : SV_OutputControlPointID)
{
    HSOutput output = (HSOutput)0;
    output.outPosition = patch[InvocationID].outPosition;
	output.outColor = patch[InvocationID].outColor;
    output.outNormal = patch[InvocationID].outNormal;
	output.outTexCoord = patch[InvocationID].outTexCoord;
    
    return output;
}