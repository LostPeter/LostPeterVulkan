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


///////////////////////////////// Height/Normal //////////////////////////////////////////
float Terrain_GetHeightFromHeightMap(Texture2D textureHeightMap,
                                     float posX,
                                     float posZ,
                                     float terrainStart,
                                     float terrainMax)
{
    int2 texDim;
	textureHeightMap.GetDimensions(texDim.x, texDim.y);

    float x = posX + texDim.x / 2.0;
    float z = texDim.y - (posZ + texDim.y / 2.0);

    uint3 coord3 = uint3(x, z, 0);
    float4 height4 = textureHeightMap.Load(coord3);
    float height = terrainStart + terrainMax * height4.r;
    return height;
}

float3 Terrain_GetNormalFromNormalMap(Texture2D textureNormalMap,
                                      float posX,
                                      float posZ)
{
    int2 texDim;
	textureNormalMap.GetDimensions(texDim.x, texDim.y);

    float x = posX + texDim.x / 2.0;
    float z = texDim.y - (posZ + texDim.y / 2.0);

    uint3 coord3 = uint3(x, z, 0);
    float4 normal4 = textureNormalMap.Load(coord3);
    float3 normal = normal4.xyz * 2.0 - 1.0;
    return normal;
}



#endif