/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-07-30
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../hlsl_input.hlsl"
#include "../hlsl_common.hlsl"


[[vk::binding(0)]]cbuffer passConsts                    : register(b0) 
{
    PassConstants passConsts;
}


#define MAX_COORDINATEAXIS_OBJECT_COUNT 19
[[vk::binding(1)]]cbuffer coordinateAxisObjectConsts    : register(b1) 
{
    CoordinateAxisObjectConstants coordinateAxisObjectConsts[MAX_COORDINATEAXIS_OBJECT_COUNT];
}


float4 main(VSInput_Pos4Color4 input, 
            uint viewIndex : SV_ViewID) : SV_TARGET
{
    CoordinateAxisObjectConstants axis = coordinateAxisObjectConsts[floor(input.inWorldPos.w + 0.5)];
    return axis.color;
}