/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-04-16
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

struct VSOutput
{
    [[vk::location(0)]] float4 inWorldPos       : POSITION0; //xyz: World Pos; w: instanceIndex
    [[vk::location(1)]] float4 inColor          : COLOR0;
    [[vk::location(2)]] float3 inWorldNormal    : NORMAL0;
    [[vk::location(3)]] float3 inWorldTangent   : TANGENT0;
    [[vk::location(4)]] float2 inTexCoord       : TEXCOORD0;
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

[[vk::binding(1)]]cbuffer objectConsts             : register(b1) 
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
    float indexTextureArray;

    float texSpeedU;
    float texSpeedV;
    float texSpeedW;
    float reserve0;

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
    float reserve0;

    TextureConstants aTexLayers[MAX_TEXTURE_COUNT];
};

[[vk::binding(2)]]cbuffer materialConsts            : register(b2) 
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

[[vk::binding(3)]]cbuffer instanceConsts            : register(b3) 
{
    InstanceConstants instanceConsts[MAX_INSTANCE_COUNT];
}


[[vk::binding(4)]] Texture2D texDiffuse             : register(t1);
[[vk::binding(4)]] SamplerState texDiffuseSampler   : register(s1);

[[vk::binding(5)]] Texture2D texNormalMap           : register(t2);
[[vk::binding(5)]] SamplerState texNormalMapSampler : register(s2);



float3 unpackNormalXYZ(float2 uv)
{
    float3 tangentNormal = texNormalMap.Sample(texNormalMapSampler, uv).rgb;
    return normalize(tangentNormal * 2.0 - 1.0);
}
float3 unpackNormalXY(float2 uv)
{
    float2 packedNormal = texNormalMap.Sample(texNormalMapSampler, uv).rg;
    float3 normal;
    normal.xy = packedNormal.xy * 2 - 1;
    normal.z = sqrt(1 - saturate(dot(normal.xy, normal.xy)));
    return normal;
}


float3 calculateNormal(VSOutput input)
{
    float3 tangentNormal = unpackNormalXYZ(input.inTexCoord);

    float3 N = normalize(input.inWorldNormal);
    float3 T = normalize(input.inWorldTangent);
    float3 B = normalize(cross(N, T));
    float3x3 TBN = transpose(float3x3(T, B, N));

    return normalize(mul(TBN, tangentNormal));
}


float3 calculate_Light_Ambient(float3 ambientGlobal, 
                               float3 ambientMaterial, 
                               float3 ambientLight)
{
    return ambientGlobal * ambientMaterial * ambientLight;
}
float3 calculate_Light_Diffuse_Lambert(float3 diffuseMaterial, 
                                       float3 diffuseLight,
                                       float3 L,
                                       float3 N)
{
    return diffuseMaterial * diffuseLight * max(dot(N, L), 0);
}
float3 calculate_Specular_Phong(float3 specularMaterial, 
                                float3 specularLight,
                                float shininess,
                                float3 posWorld,
                                float3 posEye,
                                float3 L,
                                float3 N)
{
    float3 V = normalize(posEye - posWorld);
    float3 R = normalize(2 * max(dot(N, L), 0) * N - L);
    
    return specularMaterial * specularLight * pow(max(dot(V, R), 0), shininess);
}
float3 calculate_Specular_BlinnPhong(float3 specularMaterial, 
                                    float3 specularLight,
                                    float shininess,
                                    float3 posWorld,
                                    float3 posEye,
                                    float3 L,
                                    float3 N)
{
    float3 V = normalize(posEye - posWorld);
    float3 H = normalize(L + V);

    return specularMaterial * specularLight * pow(max(dot(N, H), 0), shininess);
}

float3 calculate_Light(float3 ambientGlobal,
                       LightConstants lightCB,
                       MaterialConstants matCB,
                       float3 posWorld,
                       float3 posEye,
                       float3 N)
{
    if (lightCB.common.z == 0 || matCB.lighting != 1)
        return float3(1, 1, 1);
    
    float3 L;
    if (lightCB.common.x == 0)
    {
        L = - lightCB.direction;
    }
    else
    {
        float3 posLight = lightCB.position;
        L = normalize(posLight - posWorld);
    }

    //Ambient
    float3 colorAmbient = float3(0,0,0); 
    if (lightCB.common.z == 1 ||
        lightCB.common.z == 5 ||
        lightCB.common.z == 6 ||
        lightCB.common.z == 7 ||
        lightCB.common.z == 10 ||
        lightCB.common.z == 11)
    {
        colorAmbient = calculate_Light_Ambient(ambientGlobal,
                                               matCB.factorAmbient.rgb,
                                               lightCB.ambient.rgb);
    }
    
    //Diffuse
    float3 colorDiffuse = float3(0,0,0); 
    if (lightCB.common.z == 2 ||
        lightCB.common.z == 5 ||
        lightCB.common.z == 8 ||
        lightCB.common.z == 9 ||
        lightCB.common.z == 10 ||
        lightCB.common.z == 11)
    {
        colorDiffuse = calculate_Light_Diffuse_Lambert(matCB.factorDiffuse.rgb,
                                                       lightCB.diffuse.rgb,
                                                       L,
                                                       N);
    }
    
    //Specular
    float3 colorSpecular = float3(0,0,0);
    if (lightCB.common.z == 3 ||
        lightCB.common.z == 6 ||
        lightCB.common.z == 8 ||
        lightCB.common.z == 10)
    {
        //Phong
        colorSpecular = calculate_Specular_Phong(matCB.factorSpecular.rgb, 
                                                 lightCB.specular.rgb,
                                                 matCB.shininess,
                                                 posWorld,
                                                 posEye,
                                                 L,
                                                 N);
    }
    else if (lightCB.common.z == 4 ||
             lightCB.common.z == 7 ||
             lightCB.common.z == 9 ||
             lightCB.common.z == 11)
    {
        //BlinnPhong
        colorSpecular = calculate_Specular_BlinnPhong(matCB.factorSpecular.rgb,
                                                      lightCB.specular.rgb,
                                                      matCB.shininess,
                                                      posWorld,
                                                      posEye,
                                                      L,
                                                      N);
    }

    return colorAmbient + colorDiffuse + colorSpecular;
}


float4 main(VSOutput input) : SV_TARGET
{
    float3 outColor;

    MaterialConstants mat = materialConsts[(uint)input.inWorldPos.w];
    //Normal Map
    float3 N = calculateNormal(input);
    
    float3 colorLight;
    //Main Light
    float3 colorMainLight = calculate_Light(passConsts.g_AmbientLight.rgb,
                                            passConsts.g_MainLight,
                                            mat,
                                            input.inWorldPos.xyz,
                                            passConsts.g_EyePosW,
                                            N);
    colorLight = colorMainLight;

    //Additional Light



    //Texture
    float3 colorTexture = texDiffuse.Sample(texDiffuseSampler, input.inTexCoord).rgb;
    //VertexColor
    float3 colorVertex = input.inColor.rgb;

    //Final Color
    outColor = colorLight * colorTexture * colorVertex;
    
    return float4(outColor, 1.0);
}