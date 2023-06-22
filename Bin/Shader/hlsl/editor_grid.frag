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

struct VSOutput
{
    [[vk::location(0)]] float3 inWorldPos       : POSITION0;
    [[vk::location(1)]] float4 inColor          : COLOR0;
    [[vk::location(2)]] float2 inTexCoord       : TEXCOORD0;
   
};


//TransformConstants
struct TransformConstants
{
    float4x4 mat4View;
    float4x4 mat4View_Inv;
    float4x4 mat4Proj;
    float4x4 mat4Proj_Inv;
    float4x4 mat4ViewProj;
    float4x4 mat4ViewProj_Inv;
};
//CameraConstants
struct CameraConstants
{
    float3 posEyeWorld;    
    float fNearZ;
    float fFarZ;
    float fReserved1;
    float fReserved2;
    float fReserved3;
};
//LightConstants
#define MAX_LIGHT_COUNT 16
struct LightConstants
{
    float4 common;      // x: type; y: enable(1 or 0); z: 0-11; w: spotPower
    float3 position;    // directional/point/spot
    float falloffStart; // point/spot light only
    float3 direction;   // directional/spot light only
    float falloffEnd;   // point/spot light only
    float4 ambient;     // ambient
    float4 diffuse;     // diffuse
    float4 specular;    // specular
};
//PassConstants
struct PassConstants
{
    //TransformConstants
    TransformConstants g_Transforms[2]; //0: Eye Left(Main); 1: Eye Right
    //CameraConstants
    CameraConstants g_Cameras[2]; //0: Eye Left(Main); 1: Eye Right
    
    //TimeConstants
    float g_TotalTime;
    float g_DeltaTime;
    float g_Pad1;
    float g_Pad2;

    //RenderTarget
    float2 g_RenderTargetSize;
    float2 g_RenderTargetSize_Inv;

    //Material
    float4 g_AmbientLight;
    
    //Light
    LightConstants g_MainLight;
    LightConstants g_AdditionalLights[MAX_LIGHT_COUNT];
};

[[vk::binding(0)]]cbuffer passConsts                : register(b0) 
{
    PassConstants passConsts;
}


//GridObjectConstants
struct GridObjectConstants
{
    float4x4 g_MatWorld;
    float4 color;
};

[[vk::binding(1)]]cbuffer gridObjectConsts          : register(b1) 
{
    GridObjectConstants gridObjectConsts;
}


float4 Grid(VSOutput input, float divisions, float4 color)
{
    float lineWidth = 1.0;

    float2 vCoord = input.inTexCoord.xy * divisions;

    float2 vGrid = abs(frac(vCoord - 0.5) - 0.5) / fwidth(vCoord);
    float fLine = min(vGrid.x, vGrid.y);
    float fLineResult = lineWidth - min(fLine, lineWidth);

    return float4(color.xyz * fLineResult, 0.05 * fLineResult);
}

float4 main(VSOutput input, uint viewIndex : SV_ViewID) : SV_TARGET
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
    float fDistanceToCamera = max(distance(input.inWorldPos, vPseudoViewPos) - abs(viewPos.y), 0);
    
    float fAlphaDecreaseDistance = 20.0f;
    float fDecreaseDistance = 30.0f;
    if (fDistanceToCamera > fAlphaDecreaseDistance)
    {
        float normalizedDistanceToCamera = clamp(fDistanceToCamera - fAlphaDecreaseDistance, 0.0f, fDecreaseDistance) / fDecreaseDistance;
        outColor.a *= clamp(1.0f - normalizedDistanceToCamera, 0.0f, 1.0f);
    }
    outColor.a *= color.a;

    return outColor;
}