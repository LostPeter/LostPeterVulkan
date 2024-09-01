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

#include "./hlsl_constant.hlsl"


///////////////////////////////// Cull //////////////////////////////////////////
bool Cull_Frustum(float4 frustumPlanes[MAX_PLANE_COUNT], 
                  float4 frustumParam[4], //0: cameraPos; 1: minFrustumPlanes; 2: maxFrustumPlanes
                  float3 position, 
                  float3 extent)
{
    float3 minPos = position - extent;
    float3 maxPos = position + extent;
    float outOfRange = dot(frustumParam[1].xyz > maxPos, 1) + dot(frustumParam[2].xyz < minPos, 1);
    if (outOfRange > 0.5)
        return true;
    [unroll]
    for (uint i = 0; i < MAX_PLANE_COUNT; ++i)
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




#endif