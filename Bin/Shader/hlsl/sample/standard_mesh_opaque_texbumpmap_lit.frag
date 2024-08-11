/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-01-28
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../hlsl_input.hlsl"
#include "../hlsl_common.hlsl"
#include "../hlsl_lighting_lambert.hlsl"


[[vk::binding(0)]]cbuffer passConsts                : register(b0) 
{
    PassConstants passConsts;
}


[[vk::binding(1)]]cbuffer objectConsts              : register(b1) 
{
    ObjectConstants objectConsts[MAX_OBJECT_COUNT];
}


[[vk::binding(2)]]cbuffer materialConsts            : register(b2) 
{
    MaterialConstants materialConsts[MAX_MATERIAL_COUNT];
}


[[vk::binding(3)]]cbuffer instanceConsts            : register(b3) 
{
    InstanceConstants instanceConsts[MAX_INSTANCE_COUNT];
}


[[vk::binding(4)]] Texture2D texDiffuse             : register(t1);
[[vk::binding(4)]] SamplerState texDiffuseSampler   : register(s1);

[[vk::binding(5)]] Texture2D texBumpMap             : register(t2);
[[vk::binding(5)]] SamplerState texBumpMapSampler   : register(s2);


float4 main(VSInput_Pos4Color4Normal3TexCood2 input, 
            uint viewIndex : SV_ViewID) : SV_TARGET
{
    float3 outColor;

    MaterialConstants mat = materialConsts[floor(input.inWorldPos.w + 0.5)];
    float3 N = normalize(input.inWorldNormal);

    //BumpMap, Need Sample four times to get dx,dy
    float bumpMapType = mat.aTexLayers[1].indexTextureArray;
    if (bumpMapType == 1)
    {
        float bumpScale = mat.aTexLayers[1].texSpeedU;
        float bumpValueU = texBumpMap.Sample(texBumpMapSampler, input.inTexCoord + float2(-1.0 / mat.aTexLayers[1].texWidth, 0.0)).r -
                       texBumpMap.Sample(texBumpMapSampler, input.inTexCoord + float2(1.0 / mat.aTexLayers[1].texWidth, 0.0)).r;
        float bumpValueV = texBumpMap.Sample(texBumpMapSampler, input.inTexCoord + float2(0.0, -1.0 / mat.aTexLayers[1].texHeight)).r -
                        texBumpMap.Sample(texBumpMapSampler, input.inTexCoord + float2(0.0, 1.0 / mat.aTexLayers[1].texHeight)).r;
        N = float3(N.x + bumpValueU * bumpScale, N.y * bumpValueV * bumpScale, N.z);
    }
    
    float3 colorLight;
    //Main Light
    CameraConstants cam = passConsts.g_Cameras[viewIndex];
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
    if (bumpMapType == 0)
    {
        outColor = colorTexture;
    }
    else
    {
        outColor = colorLight * colorTexture * colorVertex;
    }

    return float4(outColor, 1.0);
}