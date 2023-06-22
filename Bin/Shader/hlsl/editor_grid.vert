/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-18
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

struct VSInput
{
    [[vk::location(0)]]float3 inPosition    : POSITION0;
    [[vk::location(1)]]float4 inColor       : COLOR0;
    [[vk::location(2)]]float2 inTexCoord    : TEXCOORD0;
};


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

[[vk::binding(0)]]cbuffer passConsts               : register(b0) 
{
    PassConstants passConsts;
}


//GridObjectConstants
struct GridObjectConstants
{
    float4x4 g_MatWorld;
    float4 color;
};

[[vk::binding(1)]]cbuffer gridObjectConsts          : register(b1) 
{
    GridObjectConstants gridObjectConsts;
}


struct VSOutput
{
	float4 outPosition                          : SV_POSITION;
    [[vk::location(0)]] float3 outWorldPos      : POSITION0;
    [[vk::location(1)]] float4 outColor         : COLOR0;
    [[vk::location(2)]] float2 outTexCoord      : TEXCOORD0;
};


VSOutput main(VSInput input, 
              uint viewIndex : SV_ViewID)
{
    VSOutput output = (VSOutput)0;

    TransformConstants trans = passConsts.g_Transforms[viewIndex];

    float4 outWorldPos = mul(gridObjectConsts.g_MatWorld, float4(input.inPosition, 1.0));
    output.outPosition = mul(trans.mat4Proj, mul(trans.mat4View, outWorldPos));
    output.outWorldPos = outWorldPos.xyz / outWorldPos.w;
    output.outColor = input.inColor;
    output.outTexCoord = input.inTexCoord;
    
    return output;
}