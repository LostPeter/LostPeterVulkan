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

#ifndef _HLSL_TERRAIN_
#define _HLSL_TERRAIN_

#include "./hlsl_common.hlsl"
#include "./hlsl_function.hlsl"


///////////////////////////////// Height/Normal //////////////////////////////////////////
float Terrain_GetHeightFromHeightMap(Texture2D textureHeightMap,
                                     float xPerf,
                                     float zPerf,
                                     float terrainStart,
                                     float terrainMax)
{
    int2 texDim;
	textureHeightMap.GetDimensions(texDim.x, texDim.y);

    float x = xPerf * texDim.x;
    float z = zPerf * texDim.y;
    
    uint3 coord3 = uint3(x, z, 0);
    float4 height4 = textureHeightMap.Load(coord3);
    float height = terrainStart + terrainMax * height4.r;
    return height;
}

float3 Terrain_GetNormalFromNormalMap(Texture2D textureNormalMap,
                                      float xPerf,
                                      float zPerf)
{
    int2 texDim;
	textureNormalMap.GetDimensions(texDim.x, texDim.y);

    float x = xPerf * texDim.x;
    float z = zPerf * texDim.y;

    uint3 coord3 = uint3(x, z, 0);
    float4 normal4 = textureNormalMap.Load(coord3);
    float3 normal = normal4.xyz * 2.0 - 1.0;
    return normal;
}


///////////////////////////////// Splat //////////////////////////////////////////////////
float2 Terrain_GetSplatUV2BySplatLayer(TerrainConstants tc, 
                                       float2 uv,
                                       int nLayer)
{
    TerrainSplatConstants layer = tc.aSplats[nLayer];
    float2 xy = float2((tc.textureX - 1) / layer.splatSizeX,
                       (tc.textureZ - 1) / layer.splatSizeY);
    float2 uvSplat = uv * xy + float2(layer.splatOffsetX, layer.splatOffsetY);
    return uvSplat;
}


void Terrain_GetSampleMask(Texture2DArray texture2DArrayMask,
                           SamplerState texture2DArrayMaskSampler,
                           int index,
                           float4 uv01,
                           float4 uv23,
                           out float4 masks[4])
{
    masks[0] = texture2DArrayMask.Sample(texture2DArrayMaskSampler, float3(uv01.xy, index));
    masks[1] = texture2DArrayMask.Sample(texture2DArrayMaskSampler, float3(uv01.zw, index));
    masks[2] = texture2DArrayMask.Sample(texture2DArrayMaskSampler, float3(uv23.xy, index));
    masks[3] = texture2DArrayMask.Sample(texture2DArrayMaskSampler, float3(uv23.zw, index));
}

float4 Terrain_GetSplatControl(Texture2DArray texture2DArrayControl,
                               SamplerState texture2DArrayControlSampler,
                               int index,
                               float2 uv)
{
    float4 splatControl = texture2DArrayControl.Sample(texture2DArrayControlSampler, float3(uv, index));
    return splatControl;
}

void Terrain_SplatMapMix(Texture2DArray texture2DArrayDiffuse,
                         SamplerState texture2DArrayDiffuseSampler,
                         Texture2DArray texture2DArrayNormal,
                         SamplerState texture2DArrayNormalSampler,
                         int nBaseIndex,
                         float2 uv, 
                         float4 uvSplat01, 
                         float4 uvSplat23, 
                         float4 splatControl, 
                         float4 diffuseRemapScale[4],
                         float normalRemapScale[4],
                         out float weight, 
                         out float4 mixedDiffuse,
                         out float3 mixedNormal)
{
    weight = dot(splatControl, 1.0h);

    //diffuse
    float4 diffuse0 = texture2DArrayDiffuse.Sample(texture2DArrayDiffuseSampler, float3(uvSplat01.xy, nBaseIndex * 4 + 0));
    float4 diffuse1 = texture2DArrayDiffuse.Sample(texture2DArrayDiffuseSampler, float3(uvSplat01.zw, nBaseIndex * 4 + 1));
    float4 diffuse2 = texture2DArrayDiffuse.Sample(texture2DArrayDiffuseSampler, float3(uvSplat23.xy, nBaseIndex * 4 + 2));
    float4 diffuse3 = texture2DArrayDiffuse.Sample(texture2DArrayDiffuseSampler, float3(uvSplat23.zw, nBaseIndex * 4 + 3));

    mixedDiffuse = 0.0h;
    mixedDiffuse += diffuse0 * half4(diffuseRemapScale[0].rgb * splatControl.rrr, 1.0h);
    mixedDiffuse += diffuse1 * half4(diffuseRemapScale[1].rgb * splatControl.ggg, 1.0h);
    mixedDiffuse += diffuse2 * half4(diffuseRemapScale[2].rgb * splatControl.bbb, 1.0h);
    mixedDiffuse += diffuse3 * half4(diffuseRemapScale[3].rgb * splatControl.aaa, 1.0h);

    //normal
    float4 normal0 = texture2DArrayNormal.Sample(texture2DArrayNormalSampler, float3(uvSplat01.xy, nBaseIndex * 4 + 0));
    float4 normal1 = texture2DArrayNormal.Sample(texture2DArrayNormalSampler, float3(uvSplat01.zw, nBaseIndex * 4 + 1));
    float4 normal2 = texture2DArrayNormal.Sample(texture2DArrayNormalSampler, float3(uvSplat23.xy, nBaseIndex * 4 + 2));
    float4 normal3 = texture2DArrayNormal.Sample(texture2DArrayNormalSampler, float3(uvSplat23.zw, nBaseIndex * 4 + 3));
    
    float3 nrm = 0.0f;
    nrm += splatControl.r * Func_UnpackNormalRGB(normal0, normalRemapScale[0]);
    nrm += splatControl.g * Func_UnpackNormalRGB(normal1, normalRemapScale[1]);
    nrm += splatControl.b * Func_UnpackNormalRGB(normal2, normalRemapScale[2]);
    nrm += splatControl.a * Func_UnpackNormalRGB(normal3, normalRemapScale[3]);
    nrm.z += 1e-5f;
    
    mixedNormal = normalize(nrm.xyz);
}



#endif