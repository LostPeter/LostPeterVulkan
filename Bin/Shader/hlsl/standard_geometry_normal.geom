/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2023-04-02
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

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
	float4x4 g_MatView;
	float4x4 g_MatView_Inv;
	float4x4 g_MatProj;
	float4x4 g_MatProj_Inv;
	float4x4 g_MatViewProj;
	float4x4 g_MatViewProj_Inv;

	float3 g_EyePosW;
	float g_cbPerObjectPad1;
	float g_NearZ;
	float g_FarZ;
	float g_TotalTime;
	float g_DeltaTime;

	float2 g_RenderTargetSize;
	float2 g_RenderTargetSize_Inv;

	float4 g_AmbientLight;

	LightConstants g_MainLight;
	LightConstants g_AdditionalLights[MAX_LIGHT_COUNT];
};

[[vk::binding(0)]]cbuffer passConsts                : register(b0)
{
	PassConstants passConsts;
}


//ObjectConstants
#define MAX_OBJECT_COUNT 1024
struct ObjectConstants
{
    float4x4 g_MatWorld;
};

[[vk::binding(1)]]cbuffer objectConsts              : register(b1) 
{
    ObjectConstants objectConsts[MAX_OBJECT_COUNT];
}


//GeometryConstants
struct GeometryConstants
{
	float width;
    float height;
    float length;
	float reserve;
};

[[vk::binding(4)]]cbuffer geometryConsts            : register(b4)
{
    GeometryConstants geometryConsts[MAX_OBJECT_COUNT];
}


struct VSOutput
{
    [[vk::location(0)]] float4 outPosition          : POSITION0; //xyz: Object Pos; w: instanceIndex
    [[vk::location(1)]] float3 outNormal            : NORMAL0;
};

struct GSOutput
{
    float4 outPosition                              : SV_POSITION;
    [[vk::location(0)]] float3 outColor             : COLOR0;
};

[maxvertexcount(6)]
void main(triangle VSOutput input[3], inout LineStream<GSOutput> output)
{
    uint instanceIndex = input[0].outPosition.w;
    ObjectConstants objInstance = objectConsts[instanceIndex];
    GeometryConstants geometryConst = geometryConsts[instanceIndex];
	float normalLength = geometryConst.length;
	for (int i = 0; i < 3; i++)
	{
		float3 pos = input[i].outPosition.xyz;
		float3 normal = input[i].outNormal.xyz;

        //0
        {
            GSOutput gsOutput0 = (GSOutput)0;
            float4 finalPos0 = mul(objInstance.g_MatWorld, float4(pos, 1.0));
            gsOutput0.outPosition = mul(passConsts.g_MatProj, mul(passConsts.g_MatView, finalPos0));
            gsOutput0.outColor = float3(1.0, 0.0, 0.0);

            output.Append(gsOutput0);
        }
		
        //1
        {
            GSOutput gsOutput1 = (GSOutput)0;
            float4 finalPos1 = mul(objInstance.g_MatWorld, float4(pos + normal * normalLength, 1.0));
            gsOutput1.outPosition = mul(passConsts.g_MatProj, mul(passConsts.g_MatView, finalPos1));
            gsOutput1.outColor = float3(0.0, 0.0, 1.0);

            output.Append(gsOutput1);
        }

		output.RestartStrip();
	}
}