/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-04-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../hlsl_input.hlsl"
#include "../hlsl_common.hlsl"

struct VSOutput
{
    [[vk::location(0)]] float4 outPosition  : POSITION0; //xyz: Object Pos; w: instanceIndex
    [[vk::location(1)]] float3 outNormal    : NORMAL0;
};


VSOutput main(VSInput_Pos3Color4Normal3TexCood2 input, 
              uint instanceIndex : SV_InstanceID)
{
    VSOutput output = (VSOutput)0;
    output.outPosition = float4(input.inPosition.xyz, instanceIndex);
    output.outNormal = input.inNormal;

    return output;
}