/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-04-07
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../hlsl_input.hlsl"
#include "../hlsl_common.hlsl"
#include "../hlsl_lighting_lambert.hlsl"

struct GSOutput
{
    [[vk::location(0)]] float4 inWorldPos       : POSITION0; //xyz: World Pos; w: instanceIndex
    [[vk::location(1)]] float4 inColor          : COLOR0;
    [[vk::location(2)]] float3 inWorldNormal    : NORMAL0;
    [[vk::location(3)]] float3 inWorldTangent   : TANGENT0;
    [[vk::location(4)]] float2 inTexCoord       : TEXCOORD0;
};


[[vk::binding(0)]]cbuffer passConsts               : register(b0) 
{
    PassConstants passConsts;
}


[[vk::binding(1)]]cbuffer objectConsts             : register(b1) 
{
    ObjectConstants objectConsts[MAX_OBJECT_COUNT];
}


[[vk::binding(2)]]cbuffer materialConsts           : register(b2) 
{
    MaterialConstants materialConsts[MAX_MATERIAL_COUNT];
}


[[vk::binding(3)]]cbuffer instanceConsts           : register(b3) 
{
    InstanceConstants instanceConsts[MAX_INSTANCE_COUNT];
}


[[vk::binding(4)]] Texture2D texDiffuse             : register(t1);
[[vk::binding(4)]] SamplerState texDiffuseSampler   : register(s1);

[[vk::binding(5)]] Texture2D texAlpha               : register(t2);
[[vk::binding(5)]] SamplerState texAlphaSampler     : register(s2);


float4 main(GSOutput input, uint viewIndex : SV_ViewID) : SV_TARGET
{
    CameraConstants cam = passConsts.g_Cameras[viewIndex];
    float4 alphaTexture = texAlpha.Sample(texAlphaSampler, input.inTexCoord);
    clip(alphaTexture - 0.5);

    float3 outColor;

    MaterialConstants mat = materialConsts[floor(input.inWorldPos.w + 0.5)];
    //Normal Map
    float3 N = normalize(input.inWorldNormal);
    
    float3 colorLight;
    //Main Light
    float3 colorMainLight = calculate_Light(passConsts.g_AmbientLight.rgb,
                                            passConsts.g_MainLight,
                                            mat,
                                            input.inWorldPos.xyz,
                                            cam.posEyeWorld, 
                                            N);
    colorLight = colorMainLight;

    //Additional Light



    //Texture
    float3 colorTexture = texDiffuse.Sample(texDiffuseSampler, input.inTexCoord).rgb;
    
    //VertexColor
    float3 colorVertex = input.inColor.rgb;

    //Final Color
    outColor = colorLight * colorTexture * colorVertex;
    
    return float4(outColor, alphaTexture.g);
}