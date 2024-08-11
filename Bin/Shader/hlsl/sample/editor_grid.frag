/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-18
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../hlsl_input.hlsl"
#include "../hlsl_common.hlsl"


[[vk::binding(0)]]cbuffer passConsts                : register(b0) 
{
    PassConstants passConsts;
}


[[vk::binding(1)]]cbuffer gridObjectConsts          : register(b1) 
{
    GridObjectConstants gridObjectConsts;
}


float4 Grid(VSInput_Pos4Color4TexCood2 input, float divisions, float4 color)
{
    float lineWidth = 2.0;

    float2 vCoord = input.inTexCoord.xy * divisions;

    float2 vGrid = abs(frac(vCoord - 0.5) - 0.5) / fwidth(vCoord);
    float fLine = min(vGrid.x, vGrid.y);
    float fLineResult = lineWidth - min(fLine, lineWidth);

    return float4(color.xyz * fLineResult, 0.05 * fLineResult);
}

float4 main(VSInput_Pos4Color4TexCood2 input, 
            uint viewIndex : SV_ViewID) : SV_TARGET
{
    float fDivs;
    float divisions = 1000.0;
    float step = 100.0;
    float subdivisions = 4.0;

    CameraConstants cam = passConsts.g_Cameras[viewIndex];
    float3 viewPos = cam.posEyeWorld;
    float4 color = gridObjectConsts.color;

	fDivs = divisions / pow(2, round((abs(viewPos.y) - step / subdivisions) / step));
	float4 vGrid1 = Grid(input, fDivs, color) + Grid(input, fDivs / subdivisions, color);

	fDivs = divisions / pow(2, round((abs(viewPos.y + 50) - step / subdivisions) / step));
	float4 vGrid2 = Grid(input, fDivs, color) + Grid(input, fDivs / subdivisions, color);

	float fAlpha = fmod(abs(viewPos.y), step);
	fAlpha = 0.0;

    float4 outColor = lerp(vGrid1, vGrid2, fAlpha);

    float3 vPseudoViewPos = float3(viewPos.x, input.inWorldPos.y, viewPos.z);
    float fDistanceToCamera = max(distance(input.inWorldPos.xyz, vPseudoViewPos) - abs(viewPos.y), 0);
    
    float fAlphaDecreaseDistance = 128.0f;
    float fDecreaseDistance = 1024.0f;
    if (fDistanceToCamera > fAlphaDecreaseDistance)
    {
        float normalizedDistanceToCamera = clamp(fDistanceToCamera - fAlphaDecreaseDistance, 0.0f, fDecreaseDistance) / fDecreaseDistance;
        outColor.a *= clamp(1.0f - normalizedDistanceToCamera, 0.0f, 1.0f);
    }
    outColor.a *= color.a;

    return outColor;
}