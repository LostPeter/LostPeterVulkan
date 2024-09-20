/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-06-30
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../hlsl_input.hlsl"
#include "../hlsl_common.hlsl"
#include "../hlsl_lighting_lambert.hlsl"
#include "../hlsl_shadow.hlsl"


[[vk::binding(0)]]cbuffer passConsts                    : register(b0) 
{
    PassConstants passConsts;
}


[[vk::binding(2)]]cbuffer materialConsts                : register(b2) 
{
    MaterialConstants materialConsts[MAX_MATERIAL_COUNT];
}


[[vk::binding(4)]] Texture2D textureDiffuse             : register(t1);
[[vk::binding(4)]] SamplerState textureDiffuseSampler   : register(s1);

[[vk::binding(5)]] Texture2D textureDepth               : register(t2);
[[vk::binding(5)]] SamplerState textureDepthSampler     : register(s2);


[[vk::constant_id(0)]] const int enablePCF = 0;
#define ambient 0.1


float4 main(VSInput_Pos4Color4Normal3TexCood2ShadowCoord4 input, 
            uint viewIndex : SV_ViewID) : SV_TARGET
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

    //Shadow
    float shadow = 1.0;
    if (mat.receiveshadow)
    {
        float4 coord = input.inShadowCoord / input.inShadowCoord.w;
        coord.y = 1.0 - coord.y;
        shadow = (enablePCF == 1) ? filterPCF(textureDepth, textureDepthSampler, coord, ambient) : calculate_Depth(textureDepth, textureDepthSampler, coord, float2(0.0, 0.0), ambient);
    }
    
    //Texture
    float3 colorTexture = textureDiffuse.Sample(textureDiffuseSampler, input.inTexCoord).rgb;
    //VertexColor
    float3 colorVertex = input.inColor.rgb;

    //Final Color
    outColor = colorLight * colorTexture * colorVertex;
    
    return float4(outColor * shadow, 1.0);
}