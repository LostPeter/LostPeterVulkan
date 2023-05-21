/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-04-02
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

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
void main(triangle VSOutput input[3], inout LineStream<GSOutput> output, uint viewIndex : SV_ViewID)
{
	TransformConstants trans = passConsts.g_Transforms[viewIndex];
    uint instanceIndex = input[0].outPosition.w;
    ObjectConstants obj = objectConsts[instanceIndex];
    GeometryConstants geometryConst = geometryConsts[instanceIndex];
	float normalLength = geometryConst.length;
	for (int i = 0; i < 3; i++)
	{
		float3 pos = input[i].outPosition.xyz;
		float3 normal = input[i].outNormal.xyz;

        //0
        {
            GSOutput gsOutput0 = (GSOutput)0;
            float4 finalPos0 = mul(obj.g_MatWorld, float4(pos, 1.0));
            gsOutput0.outPosition = mul(trans.mat4Proj, mul(trans.mat4View, finalPos0));
            gsOutput0.outColor = float3(1.0, 0.0, 0.0);

            output.Append(gsOutput0);
        }
		
        //1
        {
            GSOutput gsOutput1 = (GSOutput)0;
            float4 finalPos1 = mul(obj.g_MatWorld, float4(pos + normal * normalLength, 1.0));
            gsOutput1.outPosition = mul(trans.mat4Proj, mul(trans.mat4View, finalPos1));
            gsOutput1.outColor = float3(0.0, 0.0, 1.0);

            output.Append(gsOutput1);
        }

		output.RestartStrip();
	}
}