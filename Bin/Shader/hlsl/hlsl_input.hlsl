#ifndef _HLSL_INPUT_
#define _HLSL_INPUT_


///////////////////////////////// TexCood2 //////////////////////////////////////////////
struct VSOutput_TexCood2
{
	float4 outPosition                          : SV_POSITION;
    [[vk::location(0)]] float2 outTexCoord      : TEXCOORD0;
};

struct VSInput_TexCood2
{
    [[vk::location(0)]] float2 inTexCoord       : TEXCOORD0;
};


///////////////////////////////// Pos2Color4 ////////////////////////////////////////////
struct VSInput_Pos2Color4
{
    [[vk::location(0)]]float2 inPosition        : POSITION0;
    [[vk::location(1)]]float4 inColor           : COLOR0;
};

struct VSOutput_Color4
{
	float4 outPosition                          : SV_POSITION;
    [[vk::location(0)]] float4 outColor         : COLOR0;
};

struct VSInput_Color4
{
    [[vk::location(0)]] float4 inColor          : COLOR0;
};


///////////////////////////////// Pos2Color4TexCood2 ////////////////////////////////////
struct VSInput_Pos2Color4TexCood2
{
    [[vk::location(0)]]float2 inPosition        : POSITION0;
    [[vk::location(1)]]float4 inColor           : COLOR0;
    [[vk::location(2)]]float2 inTexCoord        : TEXCOORD0;
};

struct VSOutput_Color4TexCood2
{
	float4 outPosition                          : SV_POSITION;
    [[vk::location(0)]] float4 outColor         : COLOR0;
    [[vk::location(1)]] float2 outTexCoord      : TEXCOORD0;
};

struct VSInput_Color4TexCood2
{
    [[vk::location(0)]] float4 inColor          : COLOR0;
    [[vk::location(1)]] float2 inTexCoord       : TEXCOORD0;
};


///////////////////////////////// Pos3Color4 ////////////////////////////////////////////
struct VSInput_Pos3Color4
{
    [[vk::location(0)]]float3 inPosition        : POSITION0;
    [[vk::location(1)]]float4 inColor           : COLOR0;
};

struct VSOutput_Pos4Color4
{
	float4 outPosition                          : SV_POSITION;
    [[vk::location(0)]] float4 outWorldPos      : POSITION0;
    [[vk::location(1)]] float4 outColor         : COLOR0;
};

struct VSInput_Pos4Color4
{
    [[vk::location(0)]] float4 inWorldPos       : POSITION0;
    [[vk::location(1)]] float4 inColor          : COLOR0;
};


///////////////////////////////// Pos3Color4TexCood2 ////////////////////////////////////
struct VSInput_Pos3Color4TexCood2
{
    [[vk::location(0)]]float3 inPosition        : POSITION0;
    [[vk::location(1)]]float4 inColor           : COLOR0;
    [[vk::location(2)]]float2 inTexCoord        : TEXCOORD0;
};

struct VSOutput_Pos4Color4TexCood2
{
	float4 outPosition                          : SV_POSITION;
    [[vk::location(0)]] float4 outWorldPos      : POSITION0;
    [[vk::location(1)]] float4 outColor         : COLOR0;
    [[vk::location(2)]] float2 outTexCoord      : TEXCOORD0;
};

struct VSInput_Pos4Color4TexCood2
{
    [[vk::location(0)]] float4 inWorldPos       : POSITION0;
    [[vk::location(1)]] float4 inColor          : COLOR0;
    [[vk::location(2)]] float2 inTexCoord       : TEXCOORD0;
};


///////////////////////////////// Pos3Color4Normal3TexCood2 /////////////////////////////
struct VSInput_Pos3Color4Normal3TexCood2
{
    [[vk::location(0)]]float3 inPosition            : POSITION0;
    [[vk::location(1)]]float4 inColor               : COLOR0;
    [[vk::location(2)]]float3 inNormal              : NORMAL0;
    [[vk::location(3)]]float2 inTexCoord            : TEXCOORD0;
};

struct VSOutput_Pos4Color4Normal3TexCood2
{
	float4 outPosition                              : SV_POSITION;
    [[vk::location(0)]] float4 outWorldPos          : POSITION0; //xyz: World Pos; w: instanceIndex
    [[vk::location(1)]] float4 outColor             : COLOR0;
    [[vk::location(2)]] float3 outWorldNormal       : NORMAL0;
    [[vk::location(3)]] float2 outTexCoord          : TEXCOORD0;
};

struct VSInput_Pos4Color4Normal3TexCood2
{
    [[vk::location(0)]] float4 inWorldPos           : POSITION0; //xyz: World Pos; w: instanceIndex
    [[vk::location(1)]] float4 inColor              : COLOR0;
    [[vk::location(2)]] float3 inWorldNormal        : NORMAL0;
    [[vk::location(3)]] float2 inTexCoord           : TEXCOORD0;
};

struct VSOutput_Pos4Color4Normal3TexCood3
{
	float4 outPosition                              : SV_POSITION;
    [[vk::location(0)]] float4 outWorldPos          : POSITION0; //xyz: World Pos; w: instanceIndex
    [[vk::location(1)]] float4 outColor             : COLOR0;
    [[vk::location(2)]] float3 outWorldNormal       : NORMAL0;
    [[vk::location(3)]] float3 outTexCoord          : TEXCOORD0;
};

struct VSInput_Pos4Color4Normal3TexCood3
{
    [[vk::location(0)]] float4 inWorldPos           : POSITION0; //xyz: World Pos; w: instanceIndex
    [[vk::location(1)]] float4 inColor              : COLOR0;
    [[vk::location(2)]] float3 inWorldNormal        : NORMAL0;
    [[vk::location(3)]] float3 inTexCoord           : TEXCOORD0;
};

struct VSOutput_Pos4Color4Normal3TexCood2ShadowCoord4
{
	float4 outPosition                              : SV_POSITION;
    [[vk::location(0)]] float4 outWorldPos          : POSITION0; //xyz: World Pos; w: instanceIndex
    [[vk::location(1)]] float4 outColor             : COLOR0;
    [[vk::location(2)]] float3 outWorldNormal       : NORMAL0;
    [[vk::location(3)]] float2 outTexCoord          : TEXCOORD0;
    [[vk::location(4)]] float4 outShadowCoord       : TEXCOORD1;
};

struct VSInput_Pos4Color4Normal3TexCood2ShadowCoord4
{
    [[vk::location(0)]] float4 inWorldPos           : POSITION0; //xyz: World Pos; w: instanceIndex
    [[vk::location(1)]] float4 inColor              : COLOR0;
    [[vk::location(2)]] float3 inWorldNormal        : NORMAL0;
    [[vk::location(3)]] float2 inTexCoord           : TEXCOORD0;
    [[vk::location(4)]] float4 inShadowCoord        : TEXCOORD1;
};


///////////////////////////////// Pos3Color4Normal3Tangent3TexCood2 /////////////////////
struct VSInput_Pos3Color4Normal3Tangent3TexCood2
{
    [[vk::location(0)]]float3 inPosition            : POSITION0;
    [[vk::location(1)]]float4 inColor               : COLOR0;
    [[vk::location(2)]]float3 inNormal              : NORMAL0;
    [[vk::location(3)]]float3 inTangent             : TANGENT0;
    [[vk::location(4)]]float2 inTexCoord            : TEXCOORD0;
};

struct VSOutput_Pos4Color4Normal3Tangent3TexCood2
{
	float4 outPosition                              : SV_POSITION;
    [[vk::location(0)]] float4 outWorldPos          : POSITION0; //xyz: World Pos; w: instanceIndex
    [[vk::location(1)]] float4 outColor             : COLOR0;
    [[vk::location(2)]] float3 outWorldNormal       : NORMAL0;
    [[vk::location(3)]] float3 outWorldTangent      : TANGENT0;
    [[vk::location(4)]] float2 outTexCoord          : TEXCOORD0;
};

struct VSInput_Pos4Color4Normal3Tangent3TexCood2
{
    [[vk::location(0)]] float4 inWorldPos           : POSITION0; //xyz: World Pos; w: instanceIndex
    [[vk::location(1)]] float4 inColor              : COLOR0;
    [[vk::location(2)]] float3 inWorldNormal        : NORMAL0;
    [[vk::location(3)]] float3 inWorldTangent       : TANGENT0;
    [[vk::location(4)]] float2 inTexCoord           : TEXCOORD0;
};

struct VSOutput_Pos4Color4Normal3Tangent3TexCood2TSPos3TSEyePos3
{
	float4 outPosition                              : SV_POSITION;
    [[vk::location(0)]] float4 outWorldPos          : POSITION0; //xyz: World Pos; w: instanceIndex
    [[vk::location(1)]] float4 outColor             : COLOR0;
    [[vk::location(2)]] float3 outWorldNormal       : NORMAL0;
    [[vk::location(3)]] float3 outWorldTangent      : TANGENT0;
    [[vk::location(4)]] float2 outTexCoord          : TEXCOORD0;
    [[vk::location(5)]] float3 outTSPos             : TEXCOORD1; 
    [[vk::location(6)]] float3 outTSEyePos          : TEXCOORD2;
};

struct VSInput_Pos4Color4Normal3Tangent3TexCood2TSPos3TSEyePos3
{
    [[vk::location(0)]] float4 inWorldPos           : POSITION0; //xyz: World Pos; w: instanceIndex
    [[vk::location(1)]] float4 inColor              : COLOR0;
    [[vk::location(2)]] float3 inWorldNormal        : NORMAL0;
    [[vk::location(3)]] float3 inWorldTangent       : TANGENT0;
    [[vk::location(4)]] float2 inTexCoord           : TEXCOORD0;
    [[vk::location(5)]] float3 inTSPos              : TEXCOORD1; 
    [[vk::location(6)]] float3 inTSEyePos           : TEXCOORD2;
};




#endif