/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-05-14
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
    float g_Pad1;
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


struct VSOutput
{
	float4 outPosition                      : SV_POSITION;
    [[vk::location(0)]] float4 outColor     : COLOR0;
    [[vk::location(1)]] float2 outTexCoord  : TEXCOORD0;
};


VSOutput main(VSInput input)
{
    VSOutput output = (VSOutput)0;
    output.outPosition = float4(input.inPosition.xy * passConsts.g_Pad1, 0.0, 1.0);
    output.outColor = input.inColor;
    output.outTexCoord = input.inTexCoord;

    return output;
}