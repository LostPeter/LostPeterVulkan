/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-08-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../hlsl_input.hlsl"
#include "../hlsl_common.hlsl"


[[vk::binding(0)]]cbuffer passConsts                : register(b0) 
{
    PassConstants passConsts;
}


#define MAX_LINEFLAT2D_OBJECT_COUNT 2048
[[vk::binding(1)]]cbuffer lineFlat2DObjectConsts    : register(b1) 
{
    LineFlat2DObjectConstants lineFlat2DObjectConsts[MAX_LINEFLAT2D_OBJECT_COUNT];
}

float4 main(VSInput_Pos4Color4 input, 
            uint viewIndex : SV_ViewID) : SV_TARGET
{
    LineFlat2DObjectConstants axis = lineFlat2DObjectConsts[floor(input.inWorldPos.w + 0.5)];
    float4 color = axis.color * input.inColor;
    return color;
}