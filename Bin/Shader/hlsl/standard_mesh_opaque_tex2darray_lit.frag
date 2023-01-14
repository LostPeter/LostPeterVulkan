/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2022-12-22
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

struct VSOutput
{
    [[vk::location(0)]] float4 inColor          : COLOR0;
    [[vk::location(1)]] float2 inTexCoord       : TEXCOORD0;
    [[vk::location(2)]] float4 inWorldPos       : TEXCOORD1; //xyz: World Pos; w: instanceIndex
    [[vk::location(3)]] float3 inWorldNormal    : TEXCOORD2;
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


//MaterialConstants
#define MAX_MATERIAL_COUNT 128
struct MaterialConstants
{
    float4 factorAmbient;
    float4 factorDiffuse;
    float4 factorSpecular;

    float shininess;
    float alpha;
    float lighting;
    float indexTextureArray;

    float4x4 matTransform;
};

[[vk::binding(2)]]cbuffer materialConsts            : register(b2) 
{
    MaterialConstants materialConsts[MAX_MATERIAL_COUNT];
}


[[vk::binding(4)]] Texture2DArray texture2DArray        : register(t1);
[[vk::binding(4)]] SamplerState texture2DArraySampler   : register(s1);


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
    float3 N = normalize(input.inWorldNormal);

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
    float3 colorTexture = texture2DArray.Sample(texture2DArraySampler, float3(input.inTexCoord, mat.indexTextureArray)).rgb;
    //VertexColor
    float3 colorVertex = input.inColor.rgb;

    //Final Color
    outColor = colorLight * colorTexture * colorVertex;

    return float4(outColor, 1.0);
}