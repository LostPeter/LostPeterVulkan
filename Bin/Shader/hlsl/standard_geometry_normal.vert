/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2023-04-03
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

struct VSInput
{
    [[vk::location(0)]]float3 inPosition    : POSITION0;
    [[vk::location(1)]]float4 inColor       : COLOR0;
    [[vk::location(2)]]float3 inNormal      : NORMAL0;
    [[vk::location(3)]]float2 inTexCoord    : TEXCOORD0;
};

struct VSOutput
{
    [[vk::location(0)]] float4 outPosition  : POSITION0; //xyz: Object Pos; w: instanceIndex
    [[vk::location(1)]] float3 outNormal    : NORMAL0;
};


VSOutput main(VSInput input, uint instanceIndex : SV_InstanceID)
{
    VSOutput output = (VSOutput)0;
    output.outPosition = float4(input.inPosition.xyz, instanceIndex);
    output.outNormal = input.inNormal;

    return output;
}