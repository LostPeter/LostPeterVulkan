/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2023-03-18
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/


struct TessellationConstants
{
    float tessLevel;
    float tessAlpha;
};

[[vk::binding(5)]]cbuffer tessellationConsts        : register(b5)
{
    TessellationConstants tessellationConsts;
}

// PN patch data
struct PnPatch
{
    float b210;
    float b120;
    float b021;
    float b012;
    float b102;
    float b201;
    float b111;
    float n110;
    float n011;
    float n101;
};

struct VSOutput 
{
    float4 outPosition                              : SV_POSITION;
    [[vk::location(0)]] float4 outColor             : COLOR0;
    [[vk::location(1)]] float2 outTexCoord          : TEXCOORD0;
    [[vk::location(2)]] float4 outWorldPos          : TEXCOORD1; //xyz: World Pos; w: instanceIndex
    [[vk::location(3)]] float3 outWorldNormal       : TEXCOORD2;
};

struct HSOutput
{
    float4 outPosition                              : SV_POSITION;
    [[vk::location(0)]] float4 outColor             : COLOR0;
    [[vk::location(1)]] float2 outTexCoord          : TEXCOORD0;
    [[vk::location(2)]] float4 outWorldPos          : TEXCOORD1; //xyz: World Pos; w: instanceIndex
    [[vk::location(3)]] float3 outWorldNormal       : TEXCOORD2;
    [[vk::location(4)]] float pnPatch[10]           : TEXCOORD3;
};


struct ConstantsHSOutput
{
    float tessLevelOuter[3]                         : SV_TessFactor;
    float tessLevelInner                            : SV_InsideTessFactor;
};

void SetPnPatch(out float output[10], PnPatch patch)
{
    output[0] = patch.b210;
    output[1] = patch.b120;
    output[2] = patch.b021;
    output[3] = patch.b012;
    output[4] = patch.b102;
    output[5] = patch.b201;
    output[6] = patch.b111;
    output[7] = patch.n110;
    output[8] = patch.n011;
    output[9] = patch.n101;
}

float wij(float4 iPos, float3 iNormal, float4 jPos)
{
    return dot(jPos.xyz - iPos.xyz, iNormal);
}

float vij(float4 iPos, float3 iNormal, float4 jPos, float3 jNormal)
{
    float3 Pj_minus_Pi = jPos.xyz = iPos.xyz;
    float3 Ni_plus_Nj = iNormal + jNormal;
    return 2.0 * dot(Pj_minus_Pi, Ni_plus_Nj) / dot(Pj_minus_Pi, Pj_minus_Pi);
}

ConstantsHSOutput ConstantsHS(InputPatch<VSOutput, 3> patch, uint InvocationID : SV_PrimitiveID)
{
    ConstantsHSOutput output = (ConstantsHSOutput)0;
	output.tessLevelOuter[0] = tessellationConsts.tessLevel;
	output.tessLevelOuter[1] = tessellationConsts.tessLevel;
	output.tessLevelOuter[2] = tessellationConsts.tessLevel;
	output.tessLevelInner = tessellationConsts.tessLevel;
    return output;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstantsHS")]
[maxtessfactor(20.0f)]
HSOutput main(InputPatch<VSOutput, 3> patch, uint InvocationID : SV_OutputControlPointID)
{
    HSOutput output = (HSOutput)0;

    // get data
    output.outPosition = patch[InvocationID].outPosition;
    output.outColor = patch[InvocationID].outColor;
	output.outWorldPos = patch[InvocationID].outWorldPos;
	output.outWorldNormal = patch[InvocationID].outWorldNormal;
	output.outTexCoord = patch[InvocationID].outTexCoord;

	// set base
	float P0 = patch[0].outPosition[InvocationID];
	float P1 = patch[1].outPosition[InvocationID];
	float P2 = patch[2].outPosition[InvocationID];
	float N0 = patch[0].outWorldNormal[InvocationID];
	float N1 = patch[1].outWorldNormal[InvocationID];
	float N2 = patch[2].outWorldNormal[InvocationID];

	// compute control points
	PnPatch pnPatch;
	pnPatch.b210 = (2.0 * P0 + P1 - wij(patch[0].outPosition, patch[0].outWorldNormal, patch[1].outPosition) * N0) / 3.0;
	pnPatch.b120 = (2.0 * P1 + P0 - wij(patch[1].outPosition, patch[1].outWorldNormal, patch[0].outPosition) * N1) / 3.0;
	pnPatch.b021 = (2.0 * P1 + P2 - wij(patch[1].outPosition, patch[1].outWorldNormal, patch[2].outPosition) * N1) / 3.0;
	pnPatch.b012 = (2.0 * P2 + P1 - wij(patch[2].outPosition, patch[2].outWorldNormal, patch[1].outPosition) * N2) / 3.0;
	pnPatch.b102 = (2.0 * P2 + P0 - wij(patch[2].outPosition, patch[2].outWorldNormal, patch[0].outPosition) * N2) / 3.0;
	pnPatch.b201 = (2.0 * P0 + P2 - wij(patch[0].outPosition, patch[0].outWorldNormal, patch[2].outPosition) * N0) / 3.0;
	float E = (pnPatch.b210 +
			   pnPatch.b120 +
			   pnPatch.b021 +
			   pnPatch.b012 +
			   pnPatch.b102 +
			   pnPatch.b201 ) / 6.0;
	float V = (P0 + P1 + P2) / 3.0;
	pnPatch.b111 = E + (E - V) * 0.5;
	pnPatch.n110 = N0 + N1 - vij(patch[0].outPosition, patch[0].outWorldNormal, patch[1].outPosition, patch[1].outWorldNormal) * (P1 - P0);
	pnPatch.n011 = N1 + N2 - vij(patch[1].outPosition, patch[1].outWorldNormal, patch[2].outPosition, patch[2].outWorldNormal) * (P2 - P1);
	pnPatch.n101 = N2 + N0 - vij(patch[2].outPosition, patch[2].outWorldNormal, patch[0].outPosition, patch[0].outWorldNormal) * (P0 - P2);
	SetPnPatch(output.pnPatch, pnPatch);

    return output;
}