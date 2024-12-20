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

#include "../hlsl_input.hlsl"


[[vk::binding(4)]] Texture2D texture2D            : register(t1);
[[vk::binding(4)]] SamplerState texture2DSampler  : register(s1);

[[vk::input_attachment_index(1)]][[vk::binding(5)]] SubpassInput colorAttach_Red;
[[vk::input_attachment_index(2)]][[vk::binding(6)]] SubpassInput colorAttach_Green;
[[vk::input_attachment_index(3)]][[vk::binding(7)]] SubpassInput colorAttach_Blue;

float4 main(VSInput_Color4TexCood2 input) : SV_TARGET
{
    float4 colorRed = colorAttach_Red.SubpassLoad();
    float4 colorGreen = colorAttach_Green.SubpassLoad();
    float4 colorBlue = colorAttach_Blue.SubpassLoad();
    
    float3 outColor = colorRed.xyz + colorGreen.xyz + colorBlue.xyz;            
    return float4(outColor, 1.0);
}