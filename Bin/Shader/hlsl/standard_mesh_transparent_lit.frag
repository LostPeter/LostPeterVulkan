//author: LostPeter
//time:   2022-12-22

struct VSOutput
{
    [[vk::location(0)]] float4 inColor          : COLOR0;
    [[vk::location(1)]] float2 inTexCoord       : TEXCOORD0;
    [[vk::location(2)]] float inAlpha           : TEXCOORD1;
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
    float2 g_RenderTargetSize;
    float2 g_RenderTargetSize_Inv;
    float g_NearZ;
    float g_FarZ;
    float g_TotalTime;
    float g_DeltaTime;
    float4 g_AmbientLight;
    LightConstants g_Lights[MAX_LIGHT_COUNT];
};

[[vk::binding(0)]]cbuffer passConsts               : register(b0) 
{
    PassConstants passConsts;
}


[[vk::binding(4)]] Texture2D texture            : register(t1);
[[vk::binding(4)]] SamplerState textureSampler  : register(s1);

float4 main(VSOutput input) : SV_TARGET
{
    float3 outColor = texture.Sample(textureSampler, input.inTexCoord).rgb;
    outColor.xyz *= input.inColor.rgb;
    
    return float4(outColor, input.inAlpha);
}