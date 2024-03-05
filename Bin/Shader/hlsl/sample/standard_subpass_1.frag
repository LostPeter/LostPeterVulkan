/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-03-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

[[vk::input_attachment_index(0), vk::binding(0)]] SubpassInput colorAttach_Red;
[[vk::input_attachment_index(1), vk::binding(1)]] SubpassInput colorAttach_Green;
[[vk::input_attachment_index(2), vk::binding(2)]] SubpassInput colorAttach_Blue;

float4 main() : SV_TARGET
{
    float4 colorRed = colorAttach_Red.SubpassLoad();
    float4 colorGreen = colorAttach_Green.SubpassLoad();
    float4 colorBlue = colorAttach_Blue.SubpassLoad();
    
    float3 outColor = colorRed.xyz + colorGreen.xyz + colorBlue.xyz;            
    return float4(outColor, 1.0);
}