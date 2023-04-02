/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2023-04-02
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

// struct VSOutput
// {
// 	float4 outPosition                            : SV_POSITION;
//     [[vk::location(0)]] float4 outWorldPos        : POSITION0; //xyz: World Pos; w: instanceIndex
//     [[vk::location(1)]] float4 outColor           : COLOR0;
//     [[vk::location(2)]] float3 outWorldNormal     : NORMAL0;
//     [[vk::location(3)]] float2 outTexCoord        : TEXCOORD0;
// };

// //LightConstants
// #define MAX_LIGHT_COUNT 16
// struct LightConstants
// {
//     float4 common;      // x: type; y: enable(1 or 0); z: 0-11; w: spotPower
//     float3 position;    // directional/point/spot
//     float falloffStart; // point/spot light only
//     float3 direction;   // directional/spot light only
//     float falloffEnd;   // point/spot light only
//     float4 ambient;     // ambient
//     float4 diffuse;     // diffuse
//     float4 specular;    // specular
// };
// //PassConstants
// struct PassConstants
// {
//     float4x4 g_MatView;
// 	float4x4 g_MatView_Inv;
//     float4x4 g_MatProj;
//     float4x4 g_MatProj_Inv;
//     float4x4 g_MatViewProj;
//     float4x4 g_MatViewProj_Inv;

//     float3 g_EyePosW;
//     float g_cbPerObjectPad1;
//     float g_NearZ;
//     float g_FarZ;
//     float g_TotalTime;
//     float g_DeltaTime;

//     float2 g_RenderTargetSize;
//     float2 g_RenderTargetSize_Inv;

//     float4 g_AmbientLight;
    
//     LightConstants g_MainLight;
//     LightConstants g_AdditionalLights[MAX_LIGHT_COUNT];
// };

// [[vk::binding(0)]]cbuffer passConsts               : register(b0) 
// {
//     PassConstants passConsts;
// }


// struct GSOutput
// {
// 	float4 Pos                                     : SV_POSITION;
//     [[vk::location(0)]] float3 Color               : COLOR0;
// };

// [maxvertexcount(30)]
// void main(point VSOutput input[1], inout TriangleStream<GSOutput> output)
// {

// }