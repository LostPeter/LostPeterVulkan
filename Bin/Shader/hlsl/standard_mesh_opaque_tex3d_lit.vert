/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2023-01-14
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

struct VSInput
{
    [[vk::location(0)]]float3 inPosition    : POSITION0;
    [[vk::location(1)]]float4 inColor       : COLOR0;
    [[vk::location(2)]]float3 inNormal      : NORMAL0;
    [[vk::location(3)]]float2 inTexCoord    : TEXCOORD0;
};

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

[[vk::binding(0)]]cbuffer passConsts               : register(b0) 
{
    PassConstants passConsts;
}


//ObjectConstants
#define MAX_OBJECT_COUNT 1024
struct ObjectConstants
{
    float4x4 g_MatWorld;
};

[[vk::binding(1)]]cbuffer objectConsts            : register(b1) 
{
    ObjectConstants objectConsts[MAX_OBJECT_COUNT];
}


//MaterialConstants
#define MAX_MATERIAL_COUNT 128
struct MaterialConstants
{
    float4 factorAmbient;
    float4 factorDiffuse;
    float4 factorSpecular;

    float shininess;
    float alpha;
    float lighting;
    float indexTextureArray;

    float texSpeedU;
    float texSpeedV;
    float texSpeedW;
    float reserve;

    float4x4 matTransform;
};

[[vk::binding(2)]]cbuffer materialConsts          : register(b2) 
{
    MaterialConstants materialConsts[MAX_MATERIAL_COUNT];
}


//InstanceConstants
#define MAX_INSTANCE_COUNT 1024
struct InstanceConstants
{
    int indexObject;
    int indexMaterial;
};

[[vk::binding(3)]]cbuffer instanceConsts          : register(b3) 
{
    InstanceConstants instanceConsts[MAX_INSTANCE_COUNT];
}


struct VSOutput
{
	float4 outPosition                            : SV_POSITION;
    [[vk::location(0)]] float4 outColor           : COLOR0;
    [[vk::location(1)]] float2 outTexCoord        : TEXCOORD0;
    [[vk::location(2)]] float4 outWorldPos        : TEXCOORD1; //xyz: World Pos; w: instanceIndex
    [[vk::location(3)]] float3 outWorldNormal     : TEXCOORD2;
};


VSOutput main(VSInput input, uint instanceIndex : SV_InstanceID)
{
    VSOutput output = (VSOutput)0;
    ObjectConstants objInstance = objectConsts[instanceIndex];
    output.outWorldPos = mul(objInstance.g_MatWorld, float4(input.inPosition, 1.0));
    output.outPosition = mul(passConsts.g_MatProj, mul(passConsts.g_MatView, output.outWorldPos));
    output.outColor = input.inColor;
    output.outTexCoord = input.inTexCoord;
    output.outWorldPos.xyz /= output.outWorldPos.w;
    output.outWorldPos.w = instanceIndex;
    float4 worldNormal = mul(objInstance.g_MatWorld, float4(input.inNormal, 1.0));
    output.outWorldNormal.xyz = worldNormal.xyz / worldNormal.w;

    return output;
}