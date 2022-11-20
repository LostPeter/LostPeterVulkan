//author: LostPeter
//time:   2022-11-16

struct VSInput
{
    [[vk::location(0)]]float3 inPosition    : POSITION0;
    [[vk::location(1)]]float3 inColor       : COLOR0;
    [[vk::location(2)]]float2 inTexCoord    : TEXCOORD0;
};

#define MaxLights 16
struct Light
{
    float3 position;    // point light only
    float falloffStart; // point/spot light only
    float3 direction;   // directional/spot light only
    float falloffEnd;   // point/spot light only
    float3 strength;    // directional/point/spot
    float spotPower;    // spot light only
};

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
    float2 g_RenderTargetSize;
    float2 g_RenderTargetSize_Inv;
    float g_NearZ;
    float g_FarZ;
    float g_TotalTime;
    float g_DeltaTime;
    float4 g_AmbientLight;
    Light g_Lights[MaxLights];
};

[[vk::binding(0)]]cbuffer passConsts               : register(b0) 
{
    PassConstants passConsts;
}

struct ObjectConstants
{
    float4x4 g_MatWorld;
    float4x4 g_TexTransform;
};

[[vk::binding(1)]]cbuffer objectConsts            : register(b1) 
{
    ObjectConstants objectConsts;
}


struct VSOutput
{
	float4 outPosition                      : SV_POSITION;
    [[vk::location(0)]] float3 outColor     : COLOR0;
    [[vk::location(1)]] float2 outTexCoord  : TEXCOORD0;
};


VSOutput main(VSInput input)
{
    VSOutput output = (VSOutput)0;
    output.outPosition = mul(passConsts.g_MatProj, mul(passConsts.g_MatView, mul(objectConsts.g_MatWorld, float4(input.inPosition, 1.0))));
    output.outColor = input.inColor;
    output.outTexCoord = input.inTexCoord;

    return output;
}