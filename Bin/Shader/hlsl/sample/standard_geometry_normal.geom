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

#include "../hlsl_input.hlsl"
#include "../hlsl_common.hlsl"


[[vk::binding(0)]]cbuffer passConsts                : register(b0)
{
	PassConstants passConsts;
}


[[vk::binding(1)]]cbuffer objectConsts              : register(b1) 
{
    ObjectConstants objectConsts[MAX_OBJECT_COUNT];
}


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
void main(triangle VSOutput input[3], 
          inout LineStream<GSOutput> output, 
          uint viewIndex : SV_ViewID)
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