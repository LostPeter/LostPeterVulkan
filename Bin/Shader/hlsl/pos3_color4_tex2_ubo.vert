//author: LostPeter
//time:   2022-11-16

struct VSInput
{
    [[vk::location(0)]]float3 inPosition    : POSITION0;
    [[vk::location(1)]]float4 inColor       : COLOR0;
    [[vk::location(2)]]float2 inTexCoord    : TEXCOORD0;
};

//Light
#define MAX_LIGHT_COUNT 16
struct Light
{
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
    Light g_Lights[MAX_LIGHT_COUNT];
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
    float4 diffuseAlbedo;
    float3 fresnelR0;
    float roughness;
    float4x4 matTransform;
    float alpha;
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
	float4 outPosition                      : SV_POSITION;
    [[vk::location(0)]] float4 outColor     : COLOR0;
    [[vk::location(1)]] float2 outTexCoord  : TEXCOORD0;
};


VSOutput main(VSInput input, uint instanceIndex : SV_InstanceID)
{
    VSOutput output = (VSOutput)0;
    output.outPosition = mul(passConsts.g_MatProj, mul(passConsts.g_MatView, mul(objectConsts[instanceIndex].g_MatWorld, float4(input.inPosition, 1.0))));
    output.outColor = input.inColor;
    output.outTexCoord = input.inTexCoord;

    return output;
}