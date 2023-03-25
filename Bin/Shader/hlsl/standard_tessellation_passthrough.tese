/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2023-03-19
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


struct HSOutput
{
    float4 outPosition                              : SV_POSITION;
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
              const OutputPatch<HSOutput, 3> patch)
{
    DSOutput output = (DSOutput)0;

    uint instanceIndex = patch[0].outPosition.w;
    ObjectConstants objInstance = objectConsts[instanceIndex];
    output.outPosition.xyz = uvw.x * patch[0].outPosition.xyz +
                             uvw.y * patch[1].outPosition.xyz +
                             uvw.z * patch[2].outPosition.xyz;
    output.outPosition.w = 1.0;
    output.outWorldPos = mul(objInstance.g_MatWorld, output.outPosition);
    output.outPosition = mul(passConsts.g_MatProj, mul(passConsts.g_MatView, output.outWorldPos));
    output.outWorldPos.xyz /= output.outWorldPos.w;
    output.outWorldPos.w = instanceIndex;
    output.outColor = uvw.x * patch[0].outColor + 
                      uvw.y * patch[1].outColor + 
                      uvw.z * patch[2].outColor;
    float3 outNormal = uvw.x * patch[0].outNormal + 
                       uvw.y * patch[1].outNormal + 
                       uvw.z * patch[2].outNormal;
    output.outWorldNormal = mul((float3x3)objInstance.g_MatWorld, outNormal);
    output.outTexCoord = uvw.x * patch[0].outTexCoord + 
                         uvw.y * patch[1].outTexCoord + 
                         uvw.z * patch[2].outTexCoord;

	return output;
}