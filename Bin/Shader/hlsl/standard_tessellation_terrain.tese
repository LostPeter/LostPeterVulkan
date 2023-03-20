/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2023-03-18
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/


//LightConstants
#define MAX_LIGHT_COUNT 16
struct LightConstants
{
    float4 common;      // x: type; y: enable(1 or 0); z: 0-11; w: spotPower
    float3 position;    // directional/point/spot
    float falloffStart; // point/spot light only
    float3 direction;   // directional/spot light only
    float falloffEnd;   // point/spot light only
    float4 ambient;     // ambient
    float4 diffuse;     // diffuse
    float4 specular;    // specular
};
//PassConstants
struct PassConstants
{
    float4x4 g_MatView;
	float4x4 g_MatView_Inv;
    float4x4 g_MatProj;
    float4x4 g_MatProj_Inv;
    float4x4 g_MatViewProj;
    float4x4 g_MatViewProj_Inv;

    float3 g_EyePosW;
    float g_cbPerObjectPad1;
    float g_NearZ;
    float g_FarZ;
    float g_TotalTime;
    float g_DeltaTime;

    float2 g_RenderTargetSize;
    float2 g_RenderTargetSize_Inv;

    float4 g_AmbientLight;
    
    LightConstants g_MainLight;
    LightConstants g_AdditionalLights[MAX_LIGHT_COUNT];
};

[[vk::binding(0)]]cbuffer passConsts                : register(b0) 
{
    PassConstants passConsts;
}


//ObjectConstants
#define MAX_OBJECT_COUNT 1024
struct ObjectConstants
{
    float4x4 g_MatWorld;
};

[[vk::binding(1)]]cbuffer objectConsts              : register(b1) 
{
    ObjectConstants objectConsts[MAX_OBJECT_COUNT];
}


struct TessellationConstants
{
    float tessLevel;
    float tessAlpha;
};

[[vk::binding(5)]]cbuffer tessellationConsts        : register(b5)
{
    TessellationConstants tessellationConsts[MAX_OBJECT_COUNT];
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

struct DSOutput
{
    float4 outPosition                              : SV_POSITION;
    [[vk::location(0)]] float4 outColor             : COLOR0;
    [[vk::location(1)]] float2 outTexCoord          : TEXCOORD0;
    [[vk::location(2)]] float4 outWorldPos          : TEXCOORD1; //xyz: World Pos; w: instanceIndex
    [[vk::location(3)]] float3 outWorldNormal       : TEXCOORD2;
};

PnPatch GetPnPatch(float pnPatch[10])
{
    PnPatch output;
    output.b210 = pnPatch[0];
	output.b120 = pnPatch[1];
	output.b021 = pnPatch[2];
	output.b012 = pnPatch[3];
	output.b102 = pnPatch[4];
	output.b201 = pnPatch[5];
	output.b111 = pnPatch[6];
	output.n110 = pnPatch[7];
	output.n011 = pnPatch[8];
	output.n101 = pnPatch[9];
    return output;
}


[domain("tri")]
DSOutput main(ConstantsHSOutput input, 
              float3 uvw                            : SV_DomainLocation, 
              const OutputPatch<HSOutput, 3> patch)
{
    DSOutput output = (DSOutput)0;

    PnPatch pnPatch[3];
    pnPatch[0] = GetPnPatch(patch[0].pnPatch);
    pnPatch[1] = GetPnPatch(patch[1].pnPatch);
    pnPatch[2] = GetPnPatch(patch[2].pnPatch);

    float3 uvwSquared = uvw * uvw;
    float3 uvwCubed   = uvwSquared * uvw;

    // extract control points
    float3 b210 = float3(pnPatch[0].b210, pnPatch[1].b210, pnPatch[2].b210);
    float3 b120 = float3(pnPatch[0].b120, pnPatch[1].b120, pnPatch[2].b120);
    float3 b021 = float3(pnPatch[0].b021, pnPatch[1].b021, pnPatch[2].b021);
    float3 b012 = float3(pnPatch[0].b012, pnPatch[1].b012, pnPatch[2].b012);
    float3 b102 = float3(pnPatch[0].b102, pnPatch[1].b102, pnPatch[2].b102);
    float3 b201 = float3(pnPatch[0].b201, pnPatch[1].b201, pnPatch[2].b201);
    float3 b111 = float3(pnPatch[0].b111, pnPatch[1].b111, pnPatch[2].b111);

    // extract control normals
    float3 n110 = normalize(float3(pnPatch[0].n110, pnPatch[1].n110, pnPatch[2].n110));
    float3 n011 = normalize(float3(pnPatch[0].n011, pnPatch[1].n011, pnPatch[2].n011));
    float3 n101 = normalize(float3(pnPatch[0].n101, pnPatch[1].n101, pnPatch[2].n101));

    // compute texcoords
    output.outTexCoord  = uvw[2] * patch[0].outTexCoord + uvw[0] * patch[1].outTexCoord + uvw[1] * patch[2].outTexCoord;

    // normal
    // Barycentric normal
    float3 barNormal = uvw[2] * patch[0].outWorldNormal + 
                       uvw[0] * patch[1].outWorldNormal + 
                       uvw[1] * patch[2].outWorldNormal;
    float3 pnNormal = patch[0].outWorldNormal * uvwSquared[2] + 
                      patch[1].outWorldNormal * uvwSquared[0] + 
                      patch[2].outWorldNormal * uvwSquared[1] +
                      n110 * uvw[2] * uvw[0] + 
                      n011 * uvw[0] * uvw[1] + 
                      n101 * uvw[2] * uvw[1];
    output.outWorldNormal = tessellationConsts[0].tessAlpha * pnNormal + (1.0 - tessellationConsts[0].tessAlpha) * barNormal;
    
    // compute interpolated pos
    float3 barPos = uvw[2] * patch[0].outPosition.xyz +
                    uvw[0] * patch[1].outPosition.xyz +
                    uvw[1] * patch[2].outPosition.xyz;

    // save some computations
    uvwSquared *= 3.0;

    // compute PN position
    float3 pnPos = patch[0].outPosition.xyz * uvwCubed[2] +
                   patch[1].outPosition.xyz * uvwCubed[0] +
                   patch[2].outPosition.xyz * uvwCubed[1] +
                   b210 * uvwSquared[2] * uvw[0] +
                   b120 * uvwSquared[0] * uvw[2] +
                   b201 * uvwSquared[2] * uvw[1] +
                   b021 * uvwSquared[0] * uvw[1] +
                   b102 * uvwSquared[1] * uvw[2] +
                   b012 * uvwSquared[1] * uvw[0] +
                   b111 * 6.0 * uvw[0] * uvw[1] * uvw[2];

    // final position and normal
    float3 finalPos = (1.0 - tessellationConsts[0].tessAlpha) * barPos + tessellationConsts[0].tessAlpha * pnPos;
    ObjectConstants objInstance = objectConsts[0];
    float4 outWorldPos = mul(objInstance.g_MatWorld, float4(finalPos.xyz, 1.0));
    output.outPosition = mul(passConsts.g_MatProj, mul(passConsts.g_MatView, float4(finalPos, 1.0)));
    output.outWorldPos.xyz = output.outPosition.xyz;
    return output;
}