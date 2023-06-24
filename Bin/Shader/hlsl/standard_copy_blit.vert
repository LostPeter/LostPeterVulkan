/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-24
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

struct VSInput
{
    [[vk::location(0)]]float3 inPosition    : POSITION0;
    [[vk::location(1)]]float4 inColor       : COLOR0;
    [[vk::location(2)]]float2 inTexCoord    : TEXCOORD0;
};


//CopyBlitObjectConstants
struct CopyBlitObjectConstants
{
    float offsetX;
    float offsetY;
    float scaleX;
    float scaleY;
};

[[vk::binding(0)]]cbuffer copyBlitObjectConsts          : register(b0) 
{
    CopyBlitObjectConstants copyBlitObjectConsts;
}


struct VSOutput
{
	float4 outPosition                      : SV_POSITION;
    [[vk::location(0)]] float4 outColor     : COLOR0;
    [[vk::location(1)]] float2 outTexCoord  : TEXCOORD0;
};


VSOutput main(VSInput input)
{
    VSOutput output = (VSOutput)0;
    
    float x = input.inPosition.x * copyBlitObjectConsts.scaleX + copyBlitObjectConsts.offsetX;
    float y = input.inPosition.y * copyBlitObjectConsts.scaleY + copyBlitObjectConsts.offsetY;
    output.outPosition = float4(x, y, 0.0, 1.0);
    output.outColor = input.inColor;
    output.outTexCoord = input.inTexCoord;

    return output;
}