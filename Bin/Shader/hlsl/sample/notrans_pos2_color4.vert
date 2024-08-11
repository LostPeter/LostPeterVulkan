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

#include "../hlsl_input.hlsl"


VSOutput_Color4 main(VSInput_Pos2Color4 input)
{
    VSOutput_Color4 output = (VSOutput_Color4)0;
    output.outPosition = float4(input.inPosition, 0.0, 1.0);
    output.outColor = input.inColor;

    return output;
}