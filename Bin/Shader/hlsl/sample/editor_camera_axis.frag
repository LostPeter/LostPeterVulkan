/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-23
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


#define MAX_CAMERAAXIS_OBJECT_COUNT 6
[[vk::binding(1)]]cbuffer cameraAxisObjectConsts    : register(b1) 
{
    CameraAxisObjectConstants cameraAxisObjectConsts[MAX_CAMERAAXIS_OBJECT_COUNT];
}


float4 main(VSInput_Pos4Color4TexCood2 input, 
            uint viewIndex : SV_ViewID) : SV_TARGET
{
    CameraAxisObjectConstants axis = cameraAxisObjectConsts[floor(input.inWorldPos.w + 0.5)];
    return axis.color;
}