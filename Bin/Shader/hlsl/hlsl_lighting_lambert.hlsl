#ifndef _HLSL_LIGHTING_LAMBERT_
#define _HLSL_LIGHTING_LAMBERT_

#include "./hlsl_common.hlsl"


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


#endif