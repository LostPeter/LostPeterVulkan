/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-11-13
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

struct VSOutput
{
    [[vk::location(0)]] float4 inColor          : COLOR0;
};

float4 main(VSOutput input) : SV_TARGET
{
    float3 outColor = input.inColor.rgb;
    
    return float4(outColor.xyz, 1.0);
}