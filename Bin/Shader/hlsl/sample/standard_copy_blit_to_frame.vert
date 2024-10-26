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

#include "../hlsl_input.hlsl"
#include "../hlsl_common.hlsl"


[[vk::binding(0)]]cbuffer copyBlitObjectConsts          : register(b0) 
{
    CopyBlitObjectConstants copyBlitObjectConsts;
}


VSOutput_Color4TexCood2 main(VSInput_Pos3Color4TexCood2 input)
{
    VSOutput_Color4TexCood2 output = (VSOutput_Color4TexCood2)0;
    
    float x = input.inPosition.x * copyBlitObjectConsts.scaleX + copyBlitObjectConsts.offsetX;
    float y = input.inPosition.y * copyBlitObjectConsts.scaleY + copyBlitObjectConsts.offsetY;
    output.outPosition = float4(x, y, 0.0, 1.0);
    output.outColor = input.inColor;
    output.outTexCoord = input.inTexCoord;

    return output;
}