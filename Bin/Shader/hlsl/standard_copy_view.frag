/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-05-20
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

struct VSOutput
{
    [[vk::location(0)]] float2 inTexCoord         : TEXCOORD0;
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

[[vk::constant_id(0)]] const float VIEW_LAYER = 0.0f;

[[vk::binding(1)]] Texture2DArray textureArray       : register(t1);
[[vk::binding(1)]] SamplerState textureArraySampler  : register(s1);

float4 main(VSOutput input) : SV_TARGET
{
    const float alpha = passConsts.g_Pad1;

    float2 p1 = float2(2.0 * input.inTexCoord - 1.0);
	float2 p2 = p1 / (1.0 - alpha * length(p1));
	p2 = (p2 + 1.0) * 0.5;

    float4 outColor = float4(0.0, 0.0, 0.0, 1.0);
    bool isInside = ((p2.x >= 0.0) && (p2.x <= 1.0) && (p2.y >= 0.0 ) && (p2.y <= 1.0));
    if (isInside)
    {
        outColor = textureArray.Sample(textureArraySampler, float3(p2, VIEW_LAYER));
    }

    return outColor;
}