/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-04-07
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../hlsl_input.hlsl"


VSOutput_Pos4Color4Normal3TexCood2 main(VSInput_Pos3Color4Normal3TexCood2 input, 
                                        uint instanceIndex : SV_InstanceID)
{
    VSOutput_Pos4Color4Normal3TexCood2 output = (VSOutput_Pos4Color4Normal3TexCood2)0;
    output.outPosition = float4(input.inPosition.xyz, instanceIndex);
    output.outColor = input.inColor;
    output.outWorldNormal = input.inNormal;
    output.outTexCoord = input.inTexCoord;

    return output;
}