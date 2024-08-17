/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-08-16
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../hlsl_input.hlsl"
#include "../hlsl_common.hlsl"
#include "../hlsl_terrain.hlsl"
#include "../hlsl_lighting_lambert.hlsl"


[[vk::binding(0)]]cbuffer passConsts                : register(b0) 
{
    PassConstants passConsts;
}


[[vk::binding(2)]]cbuffer materialConsts            : register(b2) 
{
    MaterialConstants materialConsts[MAX_MATERIAL_COUNT];
}


[[vk::binding(4)]]cbuffer terrainConsts             : register(b4) 
{
    TerrainConstants terrainConsts;
}


[[vk::binding(7)]] Texture2DArray texture2DArrayDiffuse       : register(t1);
[[vk::binding(7)]] SamplerState texture2DArrayDiffuseSampler  : register(s1);

[[vk::binding(8)]] Texture2DArray texture2DArrayNormal        : register(t2);
[[vk::binding(8)]] SamplerState texture2DArrayNormalSampler   : register(s2);

[[vk::binding(9)]] Texture2DArray texture2DArrayControl       : register(t3);
[[vk::binding(9)]] SamplerState texture2DArrayControlSampler  : register(s3);


float4 main(VSInput_Terrain input, 
            uint viewIndex : SV_ViewID) : SV_TARGET
{
    float3 outColor;

    MaterialConstants mat = materialConsts[floor(input.inWorldPos.w + 0.5)];
    float3 N = normalize(input.inWorldNormal);

    //Terrain Splat
    float4 splatControl = Terrain_GetSplatControl(texture2DArrayControl,
                                                  texture2DArrayControlSampler,
                                                  0,
                                                  input.inTexCoord.xy);
    float4 diffuseRemapScale[4];
    diffuseRemapScale[0] = terrainConsts.aSplats[0].diffuseRemapScale;
    diffuseRemapScale[1] = terrainConsts.aSplats[1].diffuseRemapScale;
    diffuseRemapScale[2] = terrainConsts.aSplats[2].diffuseRemapScale;
    diffuseRemapScale[3] = terrainConsts.aSplats[3].diffuseRemapScale;
    float normalRemapScale[4];
    normalRemapScale[0] = terrainConsts.aSplats[0].normalRemapScale;
    normalRemapScale[1] = terrainConsts.aSplats[1].normalRemapScale;
    normalRemapScale[2] = terrainConsts.aSplats[2].normalRemapScale;
    normalRemapScale[3] = terrainConsts.aSplats[3].normalRemapScale;
    float weight;
    float4 mixedDiffuse;
    Terrain_SplatMapMix(texture2DArrayDiffuse,
                        texture2DArrayDiffuseSampler,
                        texture2DArrayNormal,
                        texture2DArrayNormalSampler,
                        0,
                        input.inTexCoord.xy,
                        input.uvSplat01,
                        input.uvSplat23,
                        splatControl,
                        diffuseRemapScale,
                        normalRemapScale,
                        weight,
                        mixedDiffuse,
                        N);

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
    float3 colorTexture = mixedDiffuse.xyz;
    //VertexColor
    float3 colorVertex = input.inColor.rgb;

    //Final Color
    outColor = colorLight * colorTexture * colorVertex;

    return float4(outColor, 1.0);
}