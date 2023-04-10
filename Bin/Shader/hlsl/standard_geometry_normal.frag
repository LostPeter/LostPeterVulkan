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

struct GSOutput
{
    [[vk::location(0)]] float3 inColor          : COLOR0;
};

float4 main(GSOutput input) : SV_TARGET
{
    return float4(input.inColor, 1.0);
}