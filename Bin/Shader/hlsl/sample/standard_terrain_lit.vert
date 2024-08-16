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


[[vk::binding(0)]]cbuffer passConsts                : register(b0) 
{
    PassConstants passConsts;
}

#define MAX_TERRAIN_OBJECT_COUNT 8196
[[vk::binding(1)]]cbuffer terrainObjectConsts       : register(b1) 
{
    TerrainObjectConstants terrainObjectConsts[MAX_TERRAIN_OBJECT_COUNT];
}


[[vk::binding(4)]]cbuffer terrainConsts             : register(b4) 
{
    TerrainConstants terrainConsts;
}


[[vk::binding(5)]] Texture2D textureHeightMap       : register(t1);
[[vk::binding(6)]] Texture2D textureNormalMap       : register(t2);


VSOutput_Terrain main(VSInput_Pos3Color4Normal3TexCood2 input, 
                      uint viewIndex : SV_ViewID,
                      uint instanceIndex : SV_InstanceID)
{
    VSOutput_Terrain output = (VSOutput_Terrain)0;

    TransformConstants trans = passConsts.g_Transforms[viewIndex];
    TerrainObjectConstants obj = terrainObjectConsts[instanceIndex];
    
    float posX = input.inPosition.x + obj.offsetX;
    float posZ = input.inPosition.z + obj.offsetZ;
    float height = Terrain_GetHeightFromHeightMap(textureHeightMap,
                                                  posX,
                                                  posZ,
                                                  terrainConsts.heightStart,
                                                  terrainConsts.heightMax);
    float3 normal = Terrain_GetNormalFromNormalMap(textureNormalMap,
                                                   posX,
                                                   posZ);
    float2 uv = float2(posX * terrainConsts.textureX_Inverse,
                       posZ * terrainConsts.textureZ_Inverse);

    output.outWorldPos = mul(terrainConsts.matWorld, float4(posX, 0.0, posZ, 1.0));
    output.outWorldPos.y = height;
    output.outPosition = mul(trans.mat4Proj, mul(trans.mat4View, output.outWorldPos));
    output.outWorldPos.xyz /= output.outWorldPos.w;
    output.outWorldPos.w = instanceIndex;
    output.outColor = input.inColor;
    output.outWorldNormal = mul((float3x3)terrainConsts.matWorld, normal);
    output.outTexCoord.xy = uv;
    output.outTexCoord.zw = float2(0, 0);
    
    return output;
}