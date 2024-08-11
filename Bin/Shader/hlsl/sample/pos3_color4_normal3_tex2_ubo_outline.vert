/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-12-11
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../hlsl_input.hlsl"
#include "../hlsl_common.hlsl"


[[vk::binding(0)]]cbuffer passConsts               : register(b0) 
{
    PassConstants passConsts;
}


#define MAX_OUTLINE_OBJECT_COUNT 128
[[vk::binding(1)]]cbuffer objectConsts              : register(b1) 
{
    OutlineObjectConstants objectConsts[MAX_OUTLINE_OBJECT_COUNT];
}


VSOutput_Color4 main(VSInput_Pos3Color4Normal3TexCood2 input, 
                     uint viewIndex : SV_ViewID,
                     uint instanceIndex : SV_InstanceID)
{
    VSOutput_Color4 output = (VSOutput_Color4)0;

    TransformConstants trans = passConsts.g_Transforms[viewIndex];
    OutlineObjectConstants obj = objectConsts[instanceIndex];

    float4 pos = float4(input.inPosition.xyz + input.inNormal * obj.outlineWidth, 1.0);
    output.outPosition = mul(trans.mat4Proj, mul(trans.mat4View, mul(obj.g_MatWorld, pos)));
    output.outColor = objectConsts[instanceIndex].outlineColor;

    return output;
}