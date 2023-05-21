/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-05-20
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

struct VSOutput
{
	float4 outPosition                      : SV_POSITION;
    [[vk::location(0)]] float2 outTexCoord  : TEXCOORD0;
};


VSOutput main(uint VertexIndex : SV_VertexID)
{
    VSOutput output = (VSOutput)0;
    output.outTexCoord = float2((VertexIndex << 1) & 2, VertexIndex & 2);
    output.outPosition = float4(output.outTexCoord * 2.0f - 1.0f, 0.0f, 1.0f);
    
    return output;
}