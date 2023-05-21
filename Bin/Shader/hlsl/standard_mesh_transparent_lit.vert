/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-12-22
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

struct VSInput
{
    [[vk::location(0)]]float3 inPosition    : POSITION0;
    [[vk::location(1)]]float4 inColor       : COLOR0;
    [[vk::location(2)]]float3 inNormal      : NORMAL0;
    [[vk::location(3)]]float2 inTexCoord    : TEXCOORD0;
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

[[vk::binding(1)]]cbuffer objectConsts            : register(b1) 
{
    ObjectConstants objectConsts[MAX_OBJECT_COUNT];
}


//TextureConstants
#define MAX_TEXTURE_COUNT 16
struct TextureConstants
{
    float texWidth;
    float texHeight;
    float texDepth;
    float reserve0;

    float texSpeedU;
    float texSpeedV;
    float texSpeedW;
    float reserve1;

    float texChunkMaxX;
    float texChunkMaxY;
    float texChunkIndexX;
    float texChunkIndexY;
};
//MaterialConstants
#define MAX_MATERIAL_COUNT 64
struct MaterialConstants
{
    float4 factorAmbient;
    float4 factorDiffuse;
    float4 factorSpecular;

    float shininess;
    float alpha;
    float lighting;
    float indexTextureArray;

    TextureConstants aTexLayers[MAX_TEXTURE_COUNT];
};

[[vk::binding(2)]]cbuffer materialConsts          : register(b2) 
{
    MaterialConstants materialConsts[MAX_MATERIAL_COUNT];
}


//InstanceConstants
#define MAX_INSTANCE_COUNT 1024
struct InstanceConstants
{
    int indexObject;
    int indexMaterial;
};

[[vk::binding(3)]]cbuffer instanceConsts          : register(b3) 
{
    InstanceConstants instanceConsts[MAX_INSTANCE_COUNT];
}


struct VSOutput
{
	float4 outPosition                            : SV_POSITION;
    [[vk::location(0)]] float4 outWorldPos        : POSITION0; //xyz: World Pos; w: instanceIndex
    [[vk::location(1)]] float4 outColor           : COLOR0;
    [[vk::location(2)]] float3 outWorldNormal     : NORMAL0;
    [[vk::location(3)]] float2 outTexCoord        : TEXCOORD0;
};


VSOutput main(VSInput input, 
              uint viewIndex : SV_ViewID,
              uint instanceIndex : SV_InstanceID)
{
    VSOutput output = (VSOutput)0;

    TransformConstants trans = passConsts.g_Transforms[viewIndex];
    ObjectConstants obj = objectConsts[instanceIndex];
    
    output.outWorldPos = mul(obj.g_MatWorld, float4(input.inPosition, 1.0));
    output.outPosition = mul(trans.mat4Proj, mul(trans.mat4View, output.outWorldPos));
    output.outWorldPos.xyz /= output.outWorldPos.w;
    output.outWorldPos.w = instanceIndex;
    output.outColor = input.inColor;
    output.outWorldNormal = mul((float3x3)obj.g_MatWorld, input.inNormal);
    output.outTexCoord = input.inTexCoord;

    return output;
}