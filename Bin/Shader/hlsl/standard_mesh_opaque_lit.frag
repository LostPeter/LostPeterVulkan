//author: LostPeter
//time:   2022-12-22

struct VSOutput
{
    [[vk::location(0)]] float4 inColor          : COLOR0;
    [[vk::location(1)]] float2 inTexCoord       : TEXCOORD0;
    [[vk::location(2)]] float3 inNormal         : TEXCOORD1;
    [[vk::location(3)]] float4 outWorld         : TEXCOORD2; //xyz: World Pos; w: instanceIndex
};


//LightConstants
#define MAX_LIGHT_COUNT 16
struct LightConstants
{
    float4 common;      // x: type; y:  z:  w:
    float3 position;    // directional/point/spot
    float falloffStart; // point/spot light only
    float3 direction;   // directional/spot light only
    float falloffEnd;   // point/spot light only
    float3 color;       // directional/point/spot
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


//MaterialConstants
#define MAX_MATERIAL_COUNT 128
struct MaterialConstants
{
    float4 factorAmbient;
    float4 factorDiffuse;
    float3 fresnelR0;
    float roughness;
    float4x4 matTransform;
    float alpha;
    float reserve0;
    float reserve1;
    float reserve2;
};

[[vk::binding(2)]]cbuffer materialConsts            : register(b2) 
{
    MaterialConstants materialConsts[MAX_MATERIAL_COUNT];
}


[[vk::binding(4)]] Texture2D texture            : register(t1);
[[vk::binding(4)]] SamplerState textureSampler  : register(s1);


float4 main(VSOutput input) : SV_TARGET
{
    float3 outColor;

    //Texture
    outColor = texture.Sample(textureSampler, input.inTexCoord).rgb;
    outColor.rgb *= input.inColor.rgb;

    //Material/Lighting
    MaterialConstants mat = materialConsts[(uint)input.outWorld.w];
    float3 posWorld = input.outWorld.xyz;
    float3 posLight = passConsts.g_MainLight.position;
    float3 L = normalize(posLight - posWorld);
    float3 N = normalize(input.inNormal);
    
    //Ambient
    float3 colorAmbient = mat.factorAmbient.rgb * passConsts.g_AmbientLight.rgb;
    //Diffuse
    float3 colorDiffuse = mat.factorDiffuse.rgb * passConsts.g_MainLight.color * max(dot(N, L), 0);
    
    //Final Color
    outColor.rgb += colorAmbient + colorDiffuse;

    return float4(outColor, 1.0);
}