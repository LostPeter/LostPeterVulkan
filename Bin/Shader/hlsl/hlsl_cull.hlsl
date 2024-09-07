/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-09-01
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _HLSL_CULL_
#define _HLSL_CULL_

#include "./hlsl_common.hlsl"




///////////////////////////////// Cull //////////////////////////////////////////
bool Cull_Frustum(float4 frustumPlanes[MAX_FRUSTUM_PLANE_COUNT], //6 Frustum Planes
                  float4 frustumParam[4], //Param common (0: CameraPos; 1: Min FrustumPlanes; 2: Max FrustumPlanes; 3: Hiz ScreenRes)
                  float3 position,
                  float3 extent)
{
    float3 minPos = position - extent;
    float3 maxPos = position + extent;
    float outOfRange = dot(frustumParam[1].xyz > maxPos, 1) + dot(frustumParam[2].xyz < minPos, 1);
    if (outOfRange > 0.5)
        return true;
    [unroll]
    for (uint i = 0; i < MAX_FRUSTUM_PLANE_COUNT; ++i)
    {
        float4 plane = frustumPlanes[i];
        float3 absNormal = abs(plane.xyz);
        if ((dot(position, plane.xyz) - dot(absNormal, extent)) > -plane.w)
        {
            return true;
        }
    }
    return false;
}



float Cull_DepthHiz_MinZ0(Texture2D<half> textureDepthHiz,
                          SamplerState texture2DSamplerDepthHiz,
                          float4x4 mat4VPLast, //VP Last
                          float4 frustumParam[4], //Param common (0: CameraPos; 1: Min FrustumPlanes; 2: Max FrustumPlanes; 3: Hiz ScreenRes)
                          float3 position, 
                          float3 extent,
                          bool isUVStartAtTop)
{
    float minZ = 0;
    float2 maxXY = 0;
    float2 minXY = 1;
    for (uint i = 0; i < 8; ++i)
    {
        float3 boxCorner = position + extent * c_aAggressiveExtentArray[i];
        float4 clipPos = mul(mat4VPLast, float4(boxCorner, 1));
        clipPos /= clipPos.w;
        minXY = min(clipPos.xy, minXY);
        maxXY = max(clipPos.xy, maxXY);

        minZ = max(minZ, clipPos.z);
    }

    float4 boxUVs = float4(minXY, maxXY);
    boxUVs = saturate(boxUVs * 0.5 + 0.5);
    float2 size = (boxUVs.zw - boxUVs.xy) * frustumParam[3].xy;
    float mip = log2(max(size.x, size.y));
    if (mip > frustumParam[3].z)
    {
        return 1;
    }

    mip = ceil(mip);
    mip = min(mip, frustumParam[3].w);
    float  level_lower = max(mip - 1, 0);
    float2 scale = exp2(-level_lower) * frustumParam[3].xy;
    float2 a = floor(boxUVs.xy * scale);
    float2 b = ceil(boxUVs.zw * scale);
    float2 dims = b - a;

    if (dims.x <= 2 && dims.y <= 2)
        mip = level_lower;

    if (isUVStartAtTop)
    {
        boxUVs = float4(boxUVs.x, 1 - boxUVs.y, boxUVs.z, 1 - boxUVs.w);
    }

    float4 depth = float4(textureDepthHiz.SampleLevel(texture2DSamplerDepthHiz, boxUVs.xy, mip),
                          textureDepthHiz.SampleLevel(texture2DSamplerDepthHiz, boxUVs.zy, mip),
                          textureDepthHiz.SampleLevel(texture2DSamplerDepthHiz, boxUVs.xw, mip),
                          textureDepthHiz.SampleLevel(texture2DSamplerDepthHiz, boxUVs.zw, mip));
    depth.xy = min(depth.xy, depth.zw);
    depth.x = min(depth.x, depth.y);
    return minZ >= depth.x;
}


float Cull_DepthHiz_MinZ1(Texture2D<half> textureDepthHiz,
                          SamplerState texture2DSamplerDepthHiz,
                          float4x4 mat4VPLast, //VP Last
                          float4 frustumParam[4], //Param common (0: CameraPos; 1: Min FrustumPlanes; 2: Max FrustumPlanes; 3: Hiz ScreenRes)
                          float3 position, 
                          float3 extent,
                          bool isUVStartAtTop)
{
    float minZ = 1;
    float2 maxXY = 0;
    float2 minXY = 1;
    for (uint i = 0; i < 8; ++i)
    {
        float3 boxCorner = position + extent * c_aAggressiveExtentArray[i];
        float4 clipPos = mul(mat4VPLast, float4(boxCorner, 1));
        clipPos /= clipPos.w;
        minXY = min(clipPos.xy, minXY);
        maxXY = max(clipPos.xy, maxXY);

        minZ = min(minZ, clipPos.z);
    }

    float4 boxUVs = float4(minXY, maxXY);
    boxUVs = saturate(boxUVs * 0.5 + 0.5);
    float2 size = (boxUVs.zw - boxUVs.xy) * frustumParam[3].xy;
    float mip = log2(max(size.x, size.y));
    if (mip > frustumParam[3].z)
    {
        return 1;
    }

    mip = ceil(mip);
    mip = min(mip, frustumParam[3].w);
    float  level_lower = max(mip - 1, 0);
    float2 scale = exp2(-level_lower) * frustumParam[3].xy;
    float2 a = floor(boxUVs.xy * scale);
    float2 b = ceil(boxUVs.zw * scale);
    float2 dims = b - a;

    if (dims.x <= 2 && dims.y <= 2)
        mip = level_lower;

    if (isUVStartAtTop)
    {
        boxUVs = float4(boxUVs.x, 1 - boxUVs.y, boxUVs.z, 1 - boxUVs.w);
    }

    float4 depth = float4(textureDepthHiz.SampleLevel(texture2DSamplerDepthHiz, boxUVs.xy, mip),
                          textureDepthHiz.SampleLevel(texture2DSamplerDepthHiz, boxUVs.zy, mip),
                          textureDepthHiz.SampleLevel(texture2DSamplerDepthHiz, boxUVs.xw, mip),
                          textureDepthHiz.SampleLevel(texture2DSamplerDepthHiz, boxUVs.zw, mip));

    depth.xy = max(depth.xy, depth.zw);
    depth.x = max(depth.x, depth.y);
    return minZ <= depth.x;
}


#endif