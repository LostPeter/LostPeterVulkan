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

#define PI 3.141592
#define PI_HALF 1.570796
static const float oscillateDelta = 0.05;

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
    float g_Pad1;
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

[[vk::binding(0)]]cbuffer passConsts               : register(b0) 
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

[[vk::binding(4)]]cbuffer geometryConsts           : register(b4)
{
    GeometryConstants geometryConsts[MAX_OBJECT_COUNT];
}


struct VSOutput
{
    [[vk::location(0)]] float4 outPosition          : POSITION0; //xyz: Object Pos; w: instanceIndex
    [[vk::location(1)]] float4 outColor             : COLOR0;
    [[vk::location(2)]] float3 outNormal            : NORMAL0;
    [[vk::location(3)]] float2 outTexCoord          : TEXCOORD0;
};

struct GSOutput
{
    float4 outPosition                              : SV_POSITION;
    [[vk::location(0)]] float4 outWorldPos          : POSITION0; //xyz: World Pos; w: instanceIndex
    [[vk::location(1)]] float4 outColor             : COLOR0;
    [[vk::location(2)]] float3 outWorldNormal       : NORMAL0;
    [[vk::location(3)]] float2 outTexCoord          : TEXCOORD0;
};

GSOutput createGSOut()
{
    GSOutput output;
    output.outPosition = float4(0, 0, 0, 0);
    output.outWorldPos = float4(0, 0, 0, 0);
    output.outColor = float4(1, 1, 1, 1);
    output.outWorldNormal = float3(0, 0, 0);
    output.outTexCoord = float2(0, 0);

    return output;
}

[maxvertexcount(30)]
void main(point VSOutput input[1], inout TriangleStream<GSOutput> output)
{
    float4 root = input[0].outPosition;
    uint instanceIndex = root.w;
    ObjectConstants objInstance = objectConsts[instanceIndex];
    GeometryConstants geometryConst = geometryConsts[instanceIndex];

    const int vertexCount = 12;

    float random = sin(PI * frac(root.x) + PI_HALF * frac(root.z));
    float width = geometryConst.width + (random / 50);
    float height = geometryConst.height + (random / 5);

    GSOutput v[vertexCount] = {
        createGSOut(), createGSOut(), createGSOut(), createGSOut(),
        createGSOut(), createGSOut(), createGSOut(), createGSOut(),
        createGSOut(), createGSOut(), createGSOut(), createGSOut()
    };

    //UV
    float currentV = 0;
    float offsetV = 1.f / ((vertexCount / 2) - 1);

    //Height
    float currentHeightOffset = 0;
    float currentVertexHeight = 0;

    //Wind
    float windCoEff = 0;
    for (int i = 0; i < vertexCount; i++)
    {
        v[i].outWorldNormal = float3(0, 0, 1);

        if (fmod(i , 2) == 0)
        {
            v[i].outPosition = float4(root.x - width , root.y + currentVertexHeight, root.z, 1);
            v[i].outTexCoord = float2(0, currentV);
        }
        else
        {
            v[i].outPosition = float4(root.x + width , root.y + currentVertexHeight, root.z, 1);
            v[i].outTexCoord = float2(1, currentV);

            currentV += offsetV;
            currentVertexHeight = currentV * height;
        }

        float2 wind = float2(sin(passConsts.g_DeltaTime * PI * 5), sin(passConsts.g_DeltaTime * PI * 5));
        wind.x += (sin(passConsts.g_DeltaTime + root.x / 25) + sin((passConsts.g_DeltaTime + root.x / 15) + 50)) * 0.5;
        wind.y += cos(passConsts.g_DeltaTime + root.z / 80);
        wind *= lerp(0.7, 1.0, 1.0 - random);

        float oscillationStrength = 2.5f;
        float sinSkewCoeff = random;
        float lerpCoeff = (sin(oscillationStrength * passConsts.g_DeltaTime + sinSkewCoeff) + 1.0) / 2;
        float2 leftWindBound = wind * (1.0 - oscillateDelta);
        float2 rightWindBound = wind * (1.0 + oscillateDelta);

        wind = lerp(leftWindBound, rightWindBound, lerpCoeff);

        float randomAngle = lerp(-PI, PI, random);
        float randomMagnitude = lerp(0, 1., random);
        float2 randomWindDir = float2(sin(randomAngle), cos(randomAngle));
        wind += randomWindDir * randomMagnitude;

        float windForce = length(wind);

        v[i].outPosition.xz += wind.xy * windCoEff;
        v[i].outPosition.y -= windForce * windCoEff * 0.8;
        float4 finalPos = mul(objInstance.g_MatWorld, float4(v[i].outPosition.xyz, 1.0));
        v[i].outPosition = mul(passConsts.g_MatProj, mul(passConsts.g_MatView, finalPos));
        v[i].outWorldPos.xyz = v[i].outPosition.xyz / v[i].outPosition.w;
        v[i].outWorldPos.w = instanceIndex;
        v[i].outWorldNormal = mul((float3x3)objInstance.g_MatWorld, v[i].outWorldNormal);

        if (fmod(i, 2) == 1)
        {
            windCoEff += offsetV;
        }
    }

    for (int p = 0; p < (vertexCount - 2); p++)
    {
        output.Append(v[p]);
        output.Append(v[p + 2]);
        output.Append(v[p + 1]);
    }
}