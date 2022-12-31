//author: LostPeter
//time:   2022-12-17

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
    float4 common;      // x: type; y:  z:  w:
    float3 position;    // point light only
    float falloffStart; // point/spot light only
    float3 direction;   // directional/spot light only
    float falloffEnd;   // point/spot light only
    float3 strength;    // directional/point/spot
    float spotPower;    // spot light only
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
#define MAX_OBJECT_COUNT 128
struct ObjectConstants
{
    float4x4 g_MatWorld;
    float4 outlineColor;
    float4 outlineWidth;
};

[[vk::binding(1)]]cbuffer objectConsts              : register(b1) 
{
    ObjectConstants objectConsts[MAX_OBJECT_COUNT];
}


struct VSOutput
{
	float4 outPosition                      : SV_POSITION;
    [[vk::location(0)]] float4 outColor     : COLOR0;
};


VSOutput main(VSInput input, uint instanceIndex : SV_InstanceID)
{
    VSOutput output = (VSOutput)0;
    float4 pos = float4(input.inPosition.xyz + input.inNormal * objectConsts[instanceIndex].outlineWidth.x, 1.0);
    output.outPosition = mul(passConsts.g_MatProj, mul(passConsts.g_MatView, mul(objectConsts[instanceIndex].g_MatWorld, pos)));
    output.outColor = objectConsts[instanceIndex].outlineColor;

    return output;
}