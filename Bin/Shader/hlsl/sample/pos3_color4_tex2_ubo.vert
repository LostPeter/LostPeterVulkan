/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-11-16
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


[[vk::binding(1)]]cbuffer objectConsts            : register(b1) 
{
    ObjectConstants objectConsts[MAX_OBJECT_COUNT];
}


[[vk::binding(2)]]cbuffer materialConsts          : register(b2) 
{
    MaterialConstants materialConsts[MAX_MATERIAL_COUNT];
}


[[vk::binding(3)]]cbuffer instanceConsts          : register(b3) 
{
    InstanceConstants instanceConsts[MAX_INSTANCE_COUNT];
}


VSOutput_Color4TexCood2 main(VSInput_Pos3Color4TexCood2 input, 
                             uint viewIndex : SV_ViewID,
                             uint instanceIndex : SV_InstanceID)
{
    VSOutput_Color4TexCood2 output = (VSOutput_Color4TexCood2)0;

    TransformConstants trans = passConsts.g_Transforms[viewIndex];
    ObjectConstants obj = objectConsts[instanceIndex];

    output.outPosition = mul(trans.mat4Proj, mul(trans.mat4View, mul(obj.g_MatWorld, float4(input.inPosition, 1.0))));
    output.outColor = input.inColor;
    output.outTexCoord = input.inTexCoord;

    return output;
}