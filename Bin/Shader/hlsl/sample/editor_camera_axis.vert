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


#define MAX_CAMERAAXIS_OBJECT_COUNT 7
[[vk::binding(1)]]cbuffer cameraAxisObjectConsts    : register(b1) 
{
    CameraAxisObjectConstants cameraAxisObjectConsts[MAX_CAMERAAXIS_OBJECT_COUNT];
}


VSOutput_Pos4Color4TexCood2 main(VSInput_Pos3Color4TexCood2 input, 
                                 uint viewIndex : SV_ViewID,
                                 uint instanceIndex : SV_InstanceID)
{
    VSOutput_Pos4Color4TexCood2 output = (VSOutput_Pos4Color4TexCood2)0;

    TransformConstants trans = passConsts.g_Transforms[viewIndex];
    CameraAxisObjectConstants obj = cameraAxisObjectConsts[instanceIndex];

    float4 outWorldPos = mul(obj.g_MatWorld, float4(input.inPosition, 1.0));
    output.outPosition = mul(trans.mat4Proj, mul(trans.mat4View, outWorldPos));
    output.outWorldPos.xyz = outWorldPos.xyz / outWorldPos.w;
    output.outWorldPos.w = instanceIndex;
    output.outColor = input.inColor;
    output.outTexCoord = input.inTexCoord;
    
    return output;
}