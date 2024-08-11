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


VSOutput_Pos4Color4 main(VSInput_Pos3Color4 input, 
                         uint viewIndex : SV_ViewID,
                         uint instanceIndex : SV_InstanceID)
{
    VSOutput_Pos4Color4 output = (VSOutput_Pos4Color4)0;

    TransformConstants trans = passConsts.g_Transforms[viewIndex];
    LineFlat2DObjectConstants obj = lineFlat2DObjectConsts[instanceIndex];

    output.outPosition = float4(input.inPosition.xy, 0.0, 1.0);
    output.outWorldPos.xyz = float3(0,0,0);
    output.outWorldPos.w = instanceIndex;
    output.outColor = input.inColor;
    
    return output;
}