/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-03-25
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/


//TransformConstants
struct TransformConstants
{
    float4x4 mat4View;
    float4x4 mat4View_Inv;
    float4x4 mat4Proj;
    float4x4 mat4Proj_Inv;
    float4x4 mat4ViewProj;
    float4x4 mat4ViewProj_Inv;
};
//CameraConstants
struct CameraConstants
{
    float3 posEyeWorld;    
    float fNearZ;
    float fFarZ;
    float fReserved1;
    float fReserved2;
    float fReserved3;
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

    float4x4 depthMVP;  // depthMVP
};
//PassConstants
struct PassConstants
{
    //TransformConstants
    TransformConstants g_Transforms[2]; //0: Eye Left(Main); 1: Eye Right
    //CameraConstants
    CameraConstants g_Cameras[2]; //0: Eye Left(Main); 1: Eye Right
    
    //TimeConstants
    float g_TotalTime;
    float g_DeltaTime;
    float g_Pad1;
    float g_Pad2;

    //RenderTarget
    float2 g_RenderTargetSize;
    float2 g_RenderTargetSize_Inv;

    //Material
    float4 g_AmbientLight;
    
    //Light
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
    float tessLevelOuter;
    float tessLevelInner;
    float tessAlpha;
    float tessStrength;
};

[[vk::binding(6)]]cbuffer tessellationConsts        : register(b6)
{
    TessellationConstants tessellationConsts[MAX_OBJECT_COUNT];
}


[[vk::binding(4)]] Texture2D texDisplacementMap              : register(t1);
[[vk::binding(4)]] SamplerState texDisplacementMapSampler    : register(s1);


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
              const OutputPatch<HSOutput, 3> patch,
              uint viewIndex : SV_ViewID)
{
    DSOutput output = (DSOutput)0;

    TransformConstants trans = passConsts.g_Transforms[viewIndex];
    uint instanceIndex = uvw.x * patch[0].outPosition.w;
    ObjectConstants obj = objectConsts[instanceIndex];
    output.outPosition.xyz = uvw.x * patch[0].outPosition.xyz +
                             uvw.y * patch[1].outPosition.xyz +
                             uvw.z * patch[2].outPosition.xyz;
    float3 outNormal = uvw.x * patch[0].outNormal + 
                       uvw.y * patch[1].outNormal + 
                       uvw.z * patch[2].outNormal;
    float2 outTexCoord = uvw.x * patch[0].outTexCoord + 
                         uvw.y * patch[1].outTexCoord + 
                         uvw.z * patch[2].outTexCoord;
    
    

    TessellationConstants tessellationConst = tessellationConsts[instanceIndex];
    output.outPosition.xyz += normalize(outNormal) * (max(texDisplacementMap.SampleLevel(texDisplacementMapSampler, outTexCoord.xy, 0).a, 0.0) * tessellationConst.tessStrength);
    output.outPosition.w = 1.0;
    output.outWorldPos = mul(obj.g_MatWorld, output.outPosition);
    output.outPosition = mul(trans.mat4Proj, mul(trans.mat4View, output.outWorldPos));
    output.outWorldPos.xyz /= output.outWorldPos.w;
    output.outWorldPos.w = instanceIndex;
    output.outColor = uvw.x * patch[0].outColor + 
                      uvw.y * patch[1].outColor + 
                      uvw.z * patch[2].outColor;
    output.outWorldNormal = mul((float3x3)obj.g_MatWorld, outNormal);
    output.outTexCoord = outTexCoord;

	return output;
}