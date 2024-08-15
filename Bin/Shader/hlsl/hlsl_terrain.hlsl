#ifndef _HLSL_TERRAIN_
#define _HLSL_TERRAIN_


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



#endif