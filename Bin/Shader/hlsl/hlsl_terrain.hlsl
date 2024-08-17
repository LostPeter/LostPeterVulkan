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



#endif