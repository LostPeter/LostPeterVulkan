/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-07-31
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


[[vk::binding(1)]]cbuffer lineFlat3DObjectConsts    : register(b1) 
{
    LineFlat3DObjectConstants lineFlat3DObjectConsts[MAX_OBJECT_LINEFLAT_3D_COUNT];
}


VSOutput_Pos4Color4 main(VSInput_Pos3Color4 input, 
                         uint viewIndex : SV_ViewID,
                         uint instanceIndex : SV_InstanceID)
{
    VSOutput_Pos4Color4 output = (VSOutput_Pos4Color4)0;

    TransformConstants trans = passConsts.g_Transforms[viewIndex];
    LineFlat3DObjectConstants obj = lineFlat3DObjectConsts[instanceIndex];

    float4 outWorldPos = mul(obj.g_MatWorld, float4(input.inPosition, 1.0));
    output.outPosition = mul(trans.mat4Proj, mul(trans.mat4View, outWorldPos));
    output.outWorldPos.xyz = outWorldPos.xyz / outWorldPos.w;
    output.outWorldPos.w = instanceIndex;
    output.outColor = input.inColor;
    
    return output;
}