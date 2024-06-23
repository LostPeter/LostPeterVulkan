/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-01-14
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
    [[vk::location(3)]] float2 inTexCoord       : TEXCOORD0;
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

    float4x4 depthMVP;  // depthMVP
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





float4 main(VSOutput input, uint viewIndex : SV_ViewID) : SV_TARGET
{
    float3 outColor;

    MaterialConstants mat = materialConsts[floor(input.inWorldPos.w + 0.5)];
    float3 N = normalize(input.inWorldNormal);

    float3 colorLight;
    //Main Light
    CameraConstants cam = passConsts.g_Cameras[viewIndex];
    float3 colorMainLight = calculate_Light(passConsts.g_AmbientLight.rgb,
                                            passConsts.g_MainLight,
                                            mat,
                                            input.inWorldPos.xyz,
                                            cam.posEyeWorld, 
                                            N);
    colorLight = colorMainLight;

    //Additional Light



    //Texture
    float3 colorTexture = texture2DArray.Sample(texture2DArraySampler, float3(input.inTexCoord, mat.aTexLayers[0].indexTextureArray)).rgb;
    //VertexColor
    float3 colorVertex = input.inColor.rgb;

    //Final Color
    outColor = colorLight * colorTexture * colorVertex;

    return float4(outColor, 1.0);
}